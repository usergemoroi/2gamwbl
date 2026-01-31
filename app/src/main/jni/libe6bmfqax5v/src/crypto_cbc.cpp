/*
 * libe6bmfqax5v - AES CBC Mode and Padding
 */

#include "../include/crypto.h"
#include <string.h>

// ============================================================================
// PKCS#7 Padding
// ============================================================================

int e6bm_pkcs7_pad(const uint8_t* input, size_t input_len,
                   uint8_t* output, size_t* output_len, size_t block_size) {
    if (input == NULL || output == NULL || output_len == NULL) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    if (block_size > 255 || block_size == 0) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    size_t padding = block_size - (input_len % block_size);
    size_t padded_len = input_len + padding;
    
    if (*output_len < padded_len) {
        *output_len = padded_len;
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    memcpy(output, input, input_len);
    
    for (size_t i = input_len; i < padded_len; i++) {
        output[i] = (uint8_t)padding;
    }
    
    *output_len = padded_len;
    return E6BM_SUCCESS;
}

int e6bm_pkcs7_unpad(const uint8_t* input, size_t input_len,
                     uint8_t* output, size_t* output_len) {
    if (input == NULL || output == NULL || output_len == NULL) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    if (input_len == 0) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    uint8_t padding = input[input_len - 1];
    
    if (padding == 0 || padding > input_len) {
        return E6BM_ERROR_CRYPTO;
    }
    
    // Verify padding
    for (size_t i = input_len - padding; i < input_len; i++) {
        if (input[i] != padding) {
            return E6BM_ERROR_CRYPTO;
        }
    }
    
    size_t unpadded_len = input_len - padding;
    
    if (*output_len < unpadded_len) {
        *output_len = unpadded_len;
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    memcpy(output, input, unpadded_len);
    *output_len = unpadded_len;
    
    return E6BM_SUCCESS;
}

// ============================================================================
// AES-CBC Mode
// ============================================================================

int e6bm_aes_cbc_encrypt(const e6bm_aes_context_t* ctx,
                         const uint8_t* input, size_t input_len,
                         uint8_t* output, size_t* output_len) {
    if (ctx == NULL || input == NULL || output == NULL || output_len == NULL) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    if (input_len % E6BM_AES_BLOCK_SIZE != 0) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    if (*output_len < input_len) {
        *output_len = input_len;
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    uint8_t iv[E6BM_AES_BLOCK_SIZE];
    memcpy(iv, ctx->iv, E6BM_AES_BLOCK_SIZE);
    
    size_t num_blocks = input_len / E6BM_AES_BLOCK_SIZE;
    
    for (size_t i = 0; i < num_blocks; i++) {
        const uint8_t* input_block = input + i * E6BM_AES_BLOCK_SIZE;
        uint8_t* output_block = output + i * E6BM_AES_BLOCK_SIZE;
        
        // XOR with IV/previous ciphertext
        uint8_t temp[E6BM_AES_BLOCK_SIZE];
        for (int j = 0; j < E6BM_AES_BLOCK_SIZE; j++) {
            temp[j] = input_block[j] ^ iv[j];
        }
        
        // Encrypt block
        e6bm_aes_encrypt_block(ctx, temp, output_block);
        
        // Update IV for next block
        memcpy(iv, output_block, E6BM_AES_BLOCK_SIZE);
    }
    
    *output_len = input_len;
    return E6BM_SUCCESS;
}

int e6bm_aes_cbc_decrypt(const e6bm_aes_context_t* ctx,
                         const uint8_t* input, size_t input_len,
                         uint8_t* output, size_t* output_len) {
    if (ctx == NULL || input == NULL || output == NULL || output_len == NULL) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    if (input_len % E6BM_AES_BLOCK_SIZE != 0) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    if (*output_len < input_len) {
        *output_len = input_len;
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    uint8_t iv[E6BM_AES_BLOCK_SIZE];
    memcpy(iv, ctx->iv, E6BM_AES_BLOCK_SIZE);
    
    size_t num_blocks = input_len / E6BM_AES_BLOCK_SIZE;
    
    for (size_t i = 0; i < num_blocks; i++) {
        const uint8_t* input_block = input + i * E6BM_AES_BLOCK_SIZE;
        uint8_t* output_block = output + i * E6BM_AES_BLOCK_SIZE;
        
        // Save ciphertext for next IV
        uint8_t next_iv[E6BM_AES_BLOCK_SIZE];
        memcpy(next_iv, input_block, E6BM_AES_BLOCK_SIZE);
        
        // Decrypt block
        uint8_t temp[E6BM_AES_BLOCK_SIZE];
        e6bm_aes_decrypt_block(ctx, input_block, temp);
        
        // XOR with IV/previous ciphertext
        for (int j = 0; j < E6BM_AES_BLOCK_SIZE; j++) {
            output_block[j] = temp[j] ^ iv[j];
        }
        
        // Update IV for next block
        memcpy(iv, next_iv, E6BM_AES_BLOCK_SIZE);
    }
    
    *output_len = input_len;
    return E6BM_SUCCESS;
}
