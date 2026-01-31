/*
 * libe6bmfqax5v.so - Main Header
 * 
 * Decompiled from GameBlaster Pro v3.1 Final APK
 * Original library: lib/arm64-v8a/libe6bmfqax5v.so (846 KB)
 * 
 * Purpose: Cryptographic utilities and encryption/decryption functions
 * Used by: ProxyApplication and asset decryption system
 * 
 * Dependencies:
 *   - libm.so (Math library)
 *   - libdl.so (Dynamic linking)
 *   - libc.so (Standard C library)
 */

#ifndef E6BMFQAX5V_H
#define E6BMFQAX5V_H

#include <jni.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Library version information
#define E6BM_VERSION_MAJOR 3
#define E6BM_VERSION_MINOR 1
#define E6BM_VERSION_PATCH 0
#define E6BM_VERSION_STRING "3.1.0"

// Encryption constants (from project knowledge)
#define E6BM_ENCRYPTION_KEY "Burri Burri Encryption Key v3.1"
#define E6BM_ENCRYPTION_IV "GameBlasterPro\0\0"
#define E6BM_AES_KEY_SIZE 32
#define E6BM_AES_BLOCK_SIZE 16
#define E6BM_AES_IV_SIZE 16

// Error codes
typedef enum {
    E6BM_SUCCESS = 0,
    E6BM_ERROR_INVALID_PARAM = -1,
    E6BM_ERROR_MEMORY = -2,
    E6BM_ERROR_CRYPTO = -3,
    E6BM_ERROR_COMPRESSION = -4,
    E6BM_ERROR_IO = -5,
    E6BM_ERROR_NOT_INITIALIZED = -6,
    E6BM_ERROR_ALREADY_INITIALIZED = -7
} e6bm_error_t;

// Crypto algorithm types
typedef enum {
    E6BM_ALGO_AES_256_CBC = 0,
    E6BM_ALGO_AES_128_CBC,
    E6BM_ALGO_MD5,
    E6BM_ALGO_SHA256
} e6bm_crypto_algo_t;

// Compression types
typedef enum {
    E6BM_COMPRESS_NONE = 0,
    E6BM_COMPRESS_ZLIB,
    E6BM_COMPRESS_GZIP
} e6bm_compress_t;

// Context structure (opaque)
typedef struct e6bm_context e6bm_context_t;

// Internal state structure
typedef struct {
    bool initialized;
    uint32_t flags;
    void* crypto_ctx;
    void* thread_local_data;
    pthread_key_t tls_key;
} e6bm_state_t;

// ============================================================================
// JNI Interface Functions
// ============================================================================

/**
 * JNI library initialization function
 * Called automatically when library is loaded via System.loadLibrary()
 * 
 * @param vm JavaVM instance
 * @param reserved Reserved parameter (unused)
 * @return JNI version (JNI_VERSION_1_6)
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved);

/**
 * SDK activation function (called from Java)
 * Performs initialization and sets up cryptographic contexts
 * 
 * @param env JNI environment
 * @param thiz Java object reference
 * @param param1 Configuration parameter 1
 * @param param2 Configuration parameter 2
 * @return Status code (0 = success)
 */
JNIEXPORT jint JNICALL Mundo_Activate_SDK(JNIEnv* env, jobject thiz,
                                          jlong param1, jlong param2);

// ============================================================================
// Core Cryptographic Functions
// ============================================================================

/**
 * Initialize cryptographic context
 * 
 * @param ctx Context to initialize
 * @param algo Algorithm type
 * @param key Encryption key
 * @param key_len Key length in bytes
 * @param iv Initialization vector
 * @param iv_len IV length in bytes
 * @return Error code
 */
int e6bm_crypto_init(e6bm_context_t* ctx, e6bm_crypto_algo_t algo,
                     const uint8_t* key, size_t key_len,
                     const uint8_t* iv, size_t iv_len);

/**
 * AES-256-CBC encryption
 * 
 * @param input Input plaintext data
 * @param input_len Input data length
 * @param output Output buffer for ciphertext
 * @param output_len Output buffer size (in/out)
 * @param key Encryption key (32 bytes)
 * @param iv Initialization vector (16 bytes)
 * @return Error code
 */
int e6bm_aes256_encrypt(const uint8_t* input, size_t input_len,
                        uint8_t* output, size_t* output_len,
                        const uint8_t* key, const uint8_t* iv);

/**
 * AES-256-CBC decryption
 * 
 * @param input Input ciphertext data
 * @param input_len Input data length
 * @param output Output buffer for plaintext
 * @param output_len Output buffer size (in/out)
 * @param key Decryption key (32 bytes)
 * @param iv Initialization vector (16 bytes)
 * @return Error code
 */
int e6bm_aes256_decrypt(const uint8_t* input, size_t input_len,
                        uint8_t* output, size_t* output_len,
                        const uint8_t* key, const uint8_t* iv);

/**
 * Calculate MD5 hash
 * 
 * @param input Input data
 * @param input_len Input data length
 * @param output Output buffer (16 bytes)
 * @return Error code
 */
int e6bm_md5_hash(const uint8_t* input, size_t input_len, uint8_t* output);

/**
 * Calculate SHA-256 hash
 * 
 * @param input Input data
 * @param input_len Input data length
 * @param output Output buffer (32 bytes)
 * @return Error code
 */
int e6bm_sha256_hash(const uint8_t* input, size_t input_len, uint8_t* output);

// ============================================================================
// Compression Functions
// ============================================================================

/**
 * Compress data using zlib
 * 
 * @param input Input data
 * @param input_len Input data length
 * @param output Output buffer
 * @param output_len Output buffer size (in/out)
 * @param level Compression level (0-9)
 * @return Error code
 */
int e6bm_zlib_compress(const uint8_t* input, size_t input_len,
                       uint8_t* output, size_t* output_len, int level);

/**
 * Decompress zlib data
 * 
 * @param input Input compressed data
 * @param input_len Input data length
 * @param output Output buffer
 * @param output_len Output buffer size (in/out)
 * @return Error code
 */
int e6bm_zlib_decompress(const uint8_t* input, size_t input_len,
                         uint8_t* output, size_t* output_len);

// ============================================================================
// Asset Encryption/Decryption (High-level API)
// ============================================================================

/**
 * Encrypt asset data (used by ProxyApplication)
 * Performs AES-256-CBC encryption with optional compression
 * 
 * @param plain_data Plaintext asset data
 * @param plain_len Plaintext length
 * @param encrypted_data Output encrypted data (caller must free)
 * @param encrypted_len Output encrypted length
 * @param compress Apply compression before encryption
 * @return Error code
 */
int e6bm_encrypt_asset(const uint8_t* plain_data, size_t plain_len,
                       uint8_t** encrypted_data, size_t* encrypted_len,
                       bool compress);

/**
 * Decrypt asset data (used by ProxyApplication)
 * Performs AES-256-CBC decryption with optional decompression
 * 
 * @param encrypted_data Encrypted asset data
 * @param encrypted_len Encrypted length
 * @param plain_data Output plaintext data (caller must free)
 * @param plain_len Output plaintext length
 * @param decompress Apply decompression after decryption
 * @return Error code
 */
int e6bm_decrypt_asset(const uint8_t* encrypted_data, size_t encrypted_len,
                       uint8_t** plain_data, size_t* plain_len,
                       bool decompress);

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * Get library version string
 * @return Version string (e.g., "3.1.0")
 */
const char* e6bm_get_version(void);

/**
 * Get error message for error code
 * @param error Error code
 * @return Error message string
 */
const char* e6bm_get_error_string(e6bm_error_t error);

/**
 * Free memory allocated by library
 * @param ptr Memory pointer to free
 */
void e6bm_free(void* ptr);

// ============================================================================
// Internal Functions (not exported, used internally)
// ============================================================================

// Thread-local storage management
int e6bm_tls_init(void);
void e6bm_tls_cleanup(void);
void* e6bm_tls_get(void);
int e6bm_tls_set(void* data);

// Memory management helpers
void* e6bm_malloc(size_t size);
void* e6bm_calloc(size_t nmemb, size_t size);
void* e6bm_realloc(void* ptr, size_t size);

// Crypto context management
int e6bm_crypto_context_init(void** ctx);
void e6bm_crypto_context_free(void* ctx);

// Dynamic function resolution (for obfuscation)
typedef void* (*e6bm_resolve_func_t)(const char* name);
void* e6bm_resolve_function(const char* func_name);

#ifdef __cplusplus
}
#endif

#endif // E6BMFQAX5V_H
