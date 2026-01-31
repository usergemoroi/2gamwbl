/*
 * libe6bmfqax5v - Cryptographic Functions
 * 
 * Low-level cryptographic primitives and implementations
 */

#ifndef E6BMFQAX5V_CRYPTO_H
#define E6BMFQAX5V_CRYPTO_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// AES-256-CBC Implementation
// ============================================================================

/**
 * Initialize AES context
 */
int e6bm_aes_init(e6bm_aes_context_t* ctx, const uint8_t* key, 
                  uint32_t key_bits, const uint8_t* iv);

/**
 * AES key expansion
 */
void e6bm_aes_key_expansion(e6bm_aes_context_t* ctx, const uint8_t* key);

/**
 * Encrypt single AES block (16 bytes)
 */
void e6bm_aes_encrypt_block(const e6bm_aes_context_t* ctx, 
                            const uint8_t* input, uint8_t* output);

/**
 * Decrypt single AES block (16 bytes)
 */
void e6bm_aes_decrypt_block(const e6bm_aes_context_t* ctx,
                            const uint8_t* input, uint8_t* output);

/**
 * AES-CBC encrypt (full buffer)
 */
int e6bm_aes_cbc_encrypt(const e6bm_aes_context_t* ctx,
                         const uint8_t* input, size_t input_len,
                         uint8_t* output, size_t* output_len);

/**
 * AES-CBC decrypt (full buffer)
 */
int e6bm_aes_cbc_decrypt(const e6bm_aes_context_t* ctx,
                         const uint8_t* input, size_t input_len,
                         uint8_t* output, size_t* output_len);

/**
 * Apply PKCS#7 padding
 */
int e6bm_pkcs7_pad(const uint8_t* input, size_t input_len,
                   uint8_t* output, size_t* output_len, size_t block_size);

/**
 * Remove PKCS#7 padding
 */
int e6bm_pkcs7_unpad(const uint8_t* input, size_t input_len,
                     uint8_t* output, size_t* output_len);

// ============================================================================
// AES Internal Functions
// ============================================================================

// S-box and inverse S-box
extern const uint8_t e6bm_aes_sbox[256];
extern const uint8_t e6bm_aes_inv_sbox[256];

// Rcon (round constants)
extern const uint8_t e6bm_aes_rcon[11];

// AES transformations
void e6bm_aes_sub_bytes(uint8_t* state);
void e6bm_aes_inv_sub_bytes(uint8_t* state);
void e6bm_aes_shift_rows(uint8_t* state);
void e6bm_aes_inv_shift_rows(uint8_t* state);
void e6bm_aes_mix_columns(uint8_t* state);
void e6bm_aes_inv_mix_columns(uint8_t* state);
void e6bm_aes_add_round_key(uint8_t* state, const uint32_t* round_key);

// Galois field multiplication
uint8_t e6bm_gf_mul(uint8_t a, uint8_t b);

// ============================================================================
// MD5 Implementation
// ============================================================================

/**
 * Initialize MD5 context
 */
void e6bm_md5_init(e6bm_md5_context_t* ctx);

/**
 * Update MD5 with data
 */
void e6bm_md5_update(e6bm_md5_context_t* ctx, const uint8_t* data, size_t len);

/**
 * Finalize MD5 and produce hash
 */
void e6bm_md5_final(e6bm_md5_context_t* ctx, uint8_t* hash);

/**
 * Compute MD5 hash (one-shot)
 */
int e6bm_md5_compute(const uint8_t* input, size_t input_len, uint8_t* output);

// MD5 internal functions
void e6bm_md5_transform(uint32_t state[4], const uint8_t block[64]);
void e6bm_md5_encode(uint8_t* output, const uint32_t* input, size_t len);
void e6bm_md5_decode(uint32_t* output, const uint8_t* input, size_t len);

// MD5 round functions
#define E6BM_MD5_F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define E6BM_MD5_G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define E6BM_MD5_H(x, y, z) ((x) ^ (y) ^ (z))
#define E6BM_MD5_I(x, y, z) ((y) ^ ((x) | (~z)))

#define E6BM_MD5_ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

// ============================================================================
// SHA-256 Implementation
// ============================================================================

/**
 * Initialize SHA-256 context
 */
void e6bm_sha256_init(e6bm_sha256_context_t* ctx);

/**
 * Update SHA-256 with data
 */
void e6bm_sha256_update(e6bm_sha256_context_t* ctx, const uint8_t* data, size_t len);

/**
 * Finalize SHA-256 and produce hash
 */
void e6bm_sha256_final(e6bm_sha256_context_t* ctx, uint8_t* hash);

/**
 * Compute SHA-256 hash (one-shot)
 */
int e6bm_sha256_compute(const uint8_t* input, size_t input_len, uint8_t* output);

// SHA-256 internal functions
void e6bm_sha256_transform(uint32_t state[8], const uint8_t block[64]);

// SHA-256 constants
extern const uint32_t e6bm_sha256_k[64];

// SHA-256 logical functions
#define E6BM_SHA256_CH(x, y, z)  (((x) & (y)) ^ (~(x) & (z)))
#define E6BM_SHA256_MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define E6BM_SHA256_EP0(x) (e6bm_rotr32(x, 2) ^ e6bm_rotr32(x, 13) ^ e6bm_rotr32(x, 22))
#define E6BM_SHA256_EP1(x) (e6bm_rotr32(x, 6) ^ e6bm_rotr32(x, 11) ^ e6bm_rotr32(x, 25))
#define E6BM_SHA256_SIG0(x) (e6bm_rotr32(x, 7) ^ e6bm_rotr32(x, 18) ^ ((x) >> 3))
#define E6BM_SHA256_SIG1(x) (e6bm_rotr32(x, 17) ^ e6bm_rotr32(x, 19) ^ ((x) >> 10))

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * Rotate right (32-bit)
 */
static inline uint32_t e6bm_rotr32(uint32_t value, uint32_t count) {
    return (value >> count) | (value << (32 - count));
}

/**
 * Rotate left (32-bit)
 */
static inline uint32_t e6bm_rotl32(uint32_t value, uint32_t count) {
    return (value << count) | (value >> (32 - count));
}

/**
 * Byte swap (32-bit, big endian <-> little endian)
 */
static inline uint32_t e6bm_bswap32(uint32_t value) {
    return ((value & 0xFF000000) >> 24) |
           ((value & 0x00FF0000) >> 8)  |
           ((value & 0x0000FF00) << 8)  |
           ((value & 0x000000FF) << 24);
}

/**
 * Byte swap (64-bit)
 */
static inline uint64_t e6bm_bswap64(uint64_t value) {
    return ((value & 0xFF00000000000000ULL) >> 56) |
           ((value & 0x00FF000000000000ULL) >> 40) |
           ((value & 0x0000FF0000000000ULL) >> 24) |
           ((value & 0x000000FF00000000ULL) >> 8)  |
           ((value & 0x00000000FF000000ULL) << 8)  |
           ((value & 0x0000000000FF0000ULL) << 24) |
           ((value & 0x000000000000FF00ULL) << 40) |
           ((value & 0x00000000000000FFULL) << 56);
}

/**
 * Secure memory compare (constant time)
 */
int e6bm_secure_memcmp(const void* a, const void* b, size_t len);

/**
 * Secure memory clear (prevents compiler optimization)
 */
void e6bm_secure_memzero(void* ptr, size_t len);

/**
 * Generate random bytes (using /dev/urandom)
 */
int e6bm_random_bytes(uint8_t* buffer, size_t len);

/**
 * Generate random IV
 */
int e6bm_generate_iv(uint8_t* iv, size_t len);

// ============================================================================
// Hardware Acceleration (if available)
// ============================================================================

/**
 * Check if AES hardware acceleration is available
 */
bool e6bm_has_aes_hw(void);

/**
 * Check if SHA hardware acceleration is available
 */
bool e6bm_has_sha_hw(void);

/**
 * Initialize hardware crypto (if available)
 */
int e6bm_hw_crypto_init(void);

// ============================================================================
// Key Derivation
// ============================================================================

/**
 * PBKDF2-HMAC-SHA256 key derivation
 */
int e6bm_pbkdf2_hmac_sha256(const uint8_t* password, size_t password_len,
                            const uint8_t* salt, size_t salt_len,
                            uint32_t iterations,
                            uint8_t* output, size_t output_len);

/**
 * HMAC-SHA256
 */
int e6bm_hmac_sha256(const uint8_t* key, size_t key_len,
                     const uint8_t* data, size_t data_len,
                     uint8_t* output);

#ifdef __cplusplus
}
#endif

#endif // E6BMFQAX5V_CRYPTO_H
