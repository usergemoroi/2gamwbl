/*
 * libe6bmfqax5v - Asset Encryption/Decryption
 * High-level API for encrypting and decrypting game assets
 */

#include "../include/e6bmfqax5v.h"
#include "../include/crypto.h"
#include "../include/utils.h"
#include <stdlib.h>
#include <string.h>

// ============================================================================
// Asset Encryption
// ============================================================================

int e6bm_encrypt_asset(const uint8_t* plain_data, size_t plain_len,
                       uint8_t** encrypted_data, size_t* encrypted_len,
                       bool compress) {
    if (plain_data == NULL || encrypted_data == NULL || encrypted_len == NULL) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    E6BM_LOG_INFO("Encrypting asset: %zu bytes (compress=%d)", plain_len, compress);
    
    const uint8_t* data_to_encrypt = plain_data;
    size_t data_to_encrypt_len = plain_len;
    uint8_t* compressed_data = NULL;
    
    // Step 1: Compress if requested
    if (compress) {
        size_t compressed_len = e6bm_zlib_compress_bound(plain_len);
        compressed_data = (uint8_t*)malloc(compressed_len);
        if (compressed_data == NULL) {
            return E6BM_ERROR_MEMORY;
        }
        
        int ret = e6bm_zlib_compress_data(plain_data, plain_len,
                                          compressed_data, &compressed_len, 9);
        if (ret != E6BM_SUCCESS) {
            free(compressed_data);
            return ret;
        }
        
        data_to_encrypt = compressed_data;
        data_to_encrypt_len = compressed_len;
        
        E6BM_LOG_DEBUG("Compressed %zu -> %zu bytes", plain_len, compressed_len);
    }
    
    // Step 2: Calculate padded size (PKCS#7)
    size_t padding = E6BM_AES_BLOCK_SIZE - (data_to_encrypt_len % E6BM_AES_BLOCK_SIZE);
    size_t padded_len = data_to_encrypt_len + padding;
    
    // Step 3: Allocate output buffer (header + encrypted data)
    size_t total_len = sizeof(e6bm_asset_header_t) + padded_len;
    uint8_t* output = (uint8_t*)malloc(total_len);
    if (output == NULL) {
        if (compressed_data) free(compressed_data);
        return E6BM_ERROR_MEMORY;
    }
    
    // Step 4: Fill header
    e6bm_asset_header_t* header = (e6bm_asset_header_t*)output;
    header->magic = E6BM_ASSET_MAGIC;
    header->version = E6BM_ASSET_VERSION;
    header->flags = E6BM_ASSET_FLAG_ENCRYPTED;
    if (compress) {
        header->flags |= E6BM_ASSET_FLAG_COMPRESSED;
    }
    header->original_size = (uint32_t)plain_len;
    header->compressed_size = (uint32_t)data_to_encrypt_len;
    header->encrypted_size = (uint32_t)padded_len;
    
    // Generate random IV for this asset
    e6bm_generate_iv(header->iv, sizeof(header->iv));
    
    // Calculate checksum of original data
    e6bm_sha256_compute(plain_data, plain_len, header->checksum);
    
    // Step 5: Apply padding
    uint8_t* padded_data = (uint8_t*)malloc(padded_len);
    if (padded_data == NULL) {
        free(output);
        if (compressed_data) free(compressed_data);
        return E6BM_ERROR_MEMORY;
    }
    
    memcpy(padded_data, data_to_encrypt, data_to_encrypt_len);
    for (size_t i = data_to_encrypt_len; i < padded_len; i++) {
        padded_data[i] = (uint8_t)padding;
    }
    
    // Step 6: Encrypt
    e6bm_aes_context_t aes_ctx;
    e6bm_aes_init(&aes_ctx, (const uint8_t*)E6BM_ENCRYPTION_KEY, 256, header->iv);
    
    size_t encrypted_size = padded_len;
    int ret = e6bm_aes_cbc_encrypt(&aes_ctx, padded_data, padded_len,
                                   output + sizeof(e6bm_asset_header_t),
                                   &encrypted_size);
    
    // Cleanup
    e6bm_secure_memzero(&aes_ctx, sizeof(aes_ctx));
    e6bm_secure_memzero(padded_data, padded_len);
    free(padded_data);
    if (compressed_data) {
        e6bm_secure_memzero(compressed_data, data_to_encrypt_len);
        free(compressed_data);
    }
    
    if (ret != E6BM_SUCCESS) {
        free(output);
        return ret;
    }
    
    *encrypted_data = output;
    *encrypted_len = total_len;
    
    E6BM_LOG_INFO("Asset encrypted successfully: %zu -> %zu bytes", plain_len, total_len);
    
    return E6BM_SUCCESS;
}

// ============================================================================
// Asset Decryption
// ============================================================================

int e6bm_decrypt_asset(const uint8_t* encrypted_data, size_t encrypted_len,
                       uint8_t** plain_data, size_t* plain_len,
                       bool decompress) {
    if (encrypted_data == NULL || plain_data == NULL || plain_len == NULL) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    if (encrypted_len < sizeof(e6bm_asset_header_t)) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    E6BM_LOG_INFO("Decrypting asset: %zu bytes", encrypted_len);
    
    // Step 1: Parse header
    const e6bm_asset_header_t* header = (const e6bm_asset_header_t*)encrypted_data;
    
    if (header->magic != E6BM_ASSET_MAGIC) {
        E6BM_LOG_ERROR("Invalid asset magic: 0x%08x", header->magic);
        return E6BM_ERROR_CRYPTO;
    }
    
    if (header->version != E6BM_ASSET_VERSION) {
        E6BM_LOG_ERROR("Unsupported asset version: %u", header->version);
        return E6BM_ERROR_CRYPTO;
    }
    
    if (!(header->flags & E6BM_ASSET_FLAG_ENCRYPTED)) {
        E6BM_LOG_ERROR("Asset is not encrypted");
        return E6BM_ERROR_CRYPTO;
    }
    
    bool is_compressed = (header->flags & E6BM_ASSET_FLAG_COMPRESSED) != 0;
    
    // Step 2: Decrypt
    const uint8_t* ciphertext = encrypted_data + sizeof(e6bm_asset_header_t);
    size_t ciphertext_len = header->encrypted_size;
    
    if (sizeof(e6bm_asset_header_t) + ciphertext_len > encrypted_len) {
        E6BM_LOG_ERROR("Invalid encrypted size");
        return E6BM_ERROR_CRYPTO;
    }
    
    uint8_t* decrypted = (uint8_t*)malloc(ciphertext_len);
    if (decrypted == NULL) {
        return E6BM_ERROR_MEMORY;
    }
    
    e6bm_aes_context_t aes_ctx;
    e6bm_aes_init(&aes_ctx, (const uint8_t*)E6BM_ENCRYPTION_KEY, 256, header->iv);
    
    size_t decrypted_len = ciphertext_len;
    int ret = e6bm_aes_cbc_decrypt(&aes_ctx, ciphertext, ciphertext_len,
                                   decrypted, &decrypted_len);
    
    e6bm_secure_memzero(&aes_ctx, sizeof(aes_ctx));
    
    if (ret != E6BM_SUCCESS) {
        e6bm_secure_memzero(decrypted, ciphertext_len);
        free(decrypted);
        return ret;
    }
    
    // Step 3: Remove padding
    uint8_t padding = decrypted[decrypted_len - 1];
    if (padding > E6BM_AES_BLOCK_SIZE || padding == 0) {
        E6BM_LOG_ERROR("Invalid padding: %u", padding);
        e6bm_secure_memzero(decrypted, decrypted_len);
        free(decrypted);
        return E6BM_ERROR_CRYPTO;
    }
    
    size_t unpadded_len = decrypted_len - padding;
    
    // Step 4: Decompress if needed
    uint8_t* output = NULL;
    size_t output_len = 0;
    
    if (is_compressed && decompress) {
        output_len = header->original_size;
        output = (uint8_t*)malloc(output_len);
        if (output == NULL) {
            e6bm_secure_memzero(decrypted, decrypted_len);
            free(decrypted);
            return E6BM_ERROR_MEMORY;
        }
        
        ret = e6bm_zlib_decompress_data(decrypted, unpadded_len, output, &output_len);
        
        e6bm_secure_memzero(decrypted, decrypted_len);
        free(decrypted);
        
        if (ret != E6BM_SUCCESS) {
            free(output);
            return ret;
        }
        
        E6BM_LOG_DEBUG("Decompressed %zu -> %zu bytes", unpadded_len, output_len);
    } else {
        output = decrypted;
        output_len = unpadded_len;
    }
    
    // Step 5: Verify checksum
    uint8_t computed_checksum[32];
    e6bm_sha256_compute(output, output_len, computed_checksum);
    
    if (e6bm_secure_memcmp(computed_checksum, header->checksum, 32) != 0) {
        E6BM_LOG_ERROR("Checksum verification failed");
        e6bm_secure_memzero(output, output_len);
        free(output);
        return E6BM_ERROR_CRYPTO;
    }
    
    *plain_data = output;
    *plain_len = output_len;
    
    E6BM_LOG_INFO("Asset decrypted successfully: %zu -> %zu bytes", encrypted_len, output_len);
    
    return E6BM_SUCCESS;
}

// ============================================================================
// High-level Crypto Functions
// ============================================================================

int e6bm_aes256_encrypt(const uint8_t* input, size_t input_len,
                        uint8_t* output, size_t* output_len,
                        const uint8_t* key, const uint8_t* iv) {
    if (input == NULL || output == NULL || output_len == NULL ||
        key == NULL || iv == NULL) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    e6bm_aes_context_t ctx;
    int ret = e6bm_aes_init(&ctx, key, 256, iv);
    if (ret != E6BM_SUCCESS) {
        return ret;
    }
    
    ret = e6bm_aes_cbc_encrypt(&ctx, input, input_len, output, output_len);
    e6bm_secure_memzero(&ctx, sizeof(ctx));
    
    return ret;
}

int e6bm_aes256_decrypt(const uint8_t* input, size_t input_len,
                        uint8_t* output, size_t* output_len,
                        const uint8_t* key, const uint8_t* iv) {
    if (input == NULL || output == NULL || output_len == NULL ||
        key == NULL || iv == NULL) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    e6bm_aes_context_t ctx;
    int ret = e6bm_aes_init(&ctx, key, 256, iv);
    if (ret != E6BM_SUCCESS) {
        return ret;
    }
    
    ret = e6bm_aes_cbc_decrypt(&ctx, input, input_len, output, output_len);
    e6bm_secure_memzero(&ctx, sizeof(ctx));
    
    return ret;
}
