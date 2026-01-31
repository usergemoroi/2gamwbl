/*
 * libe6bmfqax5v - Type Definitions
 * 
 * Defines all structures, enumerations, and type aliases used throughout the library
 */

#ifndef E6BMFQAX5V_TYPES_H
#define E6BMFQAX5V_TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Context Structures
// ============================================================================

// AES context for encryption/decryption operations
typedef struct {
    uint32_t round_keys[60];      // Expanded key schedule
    uint8_t key[32];              // Original key
    uint8_t iv[16];               // Initialization vector
    uint32_t key_bits;            // Key size in bits (128, 192, or 256)
    uint32_t num_rounds;          // Number of AES rounds (10, 12, or 14)
    bool is_encrypt;              // Encryption or decryption mode
} e6bm_aes_context_t;

// MD5 context
typedef struct {
    uint32_t state[4];            // Hash state (A, B, C, D)
    uint64_t count;               // Number of bits processed
    uint8_t buffer[64];           // Input buffer
} e6bm_md5_context_t;

// SHA-256 context
typedef struct {
    uint32_t state[8];            // Hash state (H0-H7)
    uint64_t count;               // Number of bits processed
    uint8_t buffer[64];           // Input buffer
} e6bm_sha256_context_t;

// Zlib compression context
typedef struct {
    void* zlib_stream;            // zlib z_stream structure
    int level;                    // Compression level (0-9)
    bool initialized;             // Initialization flag
} e6bm_zlib_context_t;

// Main cryptographic context
struct e6bm_context {
    e6bm_aes_context_t* aes_ctx;
    e6bm_md5_context_t* md5_ctx;
    e6bm_sha256_context_t* sha256_ctx;
    e6bm_zlib_context_t* zlib_ctx;
    uint32_t flags;
    pthread_mutex_t mutex;
};

// ============================================================================
// Thread-Local Storage
// ============================================================================

// Thread-local data structure
typedef struct {
    void* crypto_context;         // Per-thread crypto context
    uint8_t* temp_buffer;         // Temporary buffer for operations
    size_t temp_buffer_size;      // Size of temporary buffer
    uint32_t thread_id;           // Thread identifier
    uint32_t flags;               // Thread-specific flags
} e6bm_thread_local_t;

// ============================================================================
// Dynamic Function Resolution
// ============================================================================

// Function pointer table for dynamic resolution
typedef struct {
    void* (*malloc_func)(size_t);
    void (*free_func)(void*);
    void* (*realloc_func)(void*, size_t);
    void* (*memcpy_func)(void*, const void*, size_t);
    void* (*memset_func)(void*, int, size_t);
    int (*memcmp_func)(const void*, const void*, size_t);
    
    // Crypto function pointers (for dynamic loading)
    void* aes_encrypt_func;
    void* aes_decrypt_func;
    void* md5_hash_func;
    void* sha256_hash_func;
    
    // Compression function pointers
    void* zlib_compress_func;
    void* zlib_decompress_func;
    
    bool initialized;
} e6bm_function_table_t;

// ============================================================================
// Configuration and Flags
// ============================================================================

// Library configuration flags
#define E6BM_FLAG_INITIALIZED       0x00000001
#define E6BM_FLAG_USE_HW_CRYPTO     0x00000002
#define E6BM_FLAG_DEBUG_MODE        0x00000004
#define E6BM_FLAG_STRICT_VALIDATION 0x00000008
#define E6BM_FLAG_USE_COMPRESSION   0x00000010
#define E6BM_FLAG_THREAD_SAFE       0x00000020

// Thread-local flags
#define E6BM_TLS_FLAG_INITIALIZED   0x00000001
#define E6BM_TLS_FLAG_BUSY          0x00000002
#define E6BM_TLS_FLAG_ERROR         0x00000004

// ============================================================================
// Asset Encryption Metadata
// ============================================================================

// Asset encryption header (prepended to encrypted data)
typedef struct {
    uint32_t magic;               // Magic number: 0x36426D45 ("E6bm")
    uint32_t version;             // Format version
    uint32_t flags;               // Encryption flags
    uint32_t original_size;       // Original uncompressed size
    uint32_t compressed_size;     // Compressed size (before encryption)
    uint32_t encrypted_size;      // Final encrypted size
    uint8_t iv[16];               // Initialization vector for this asset
    uint8_t checksum[32];         // SHA-256 checksum of original data
} __attribute__((packed)) e6bm_asset_header_t;

#define E6BM_ASSET_MAGIC 0x36426D45
#define E6BM_ASSET_VERSION 1

// Asset encryption flags
#define E6BM_ASSET_FLAG_COMPRESSED  0x00000001
#define E6BM_ASSET_FLAG_ENCRYPTED   0x00000002
#define E6BM_ASSET_FLAG_VERIFIED    0x00000004

// ============================================================================
// Memory Pool (for efficient allocation)
// ============================================================================

typedef struct e6bm_memory_block {
    void* data;
    size_t size;
    bool in_use;
    struct e6bm_memory_block* next;
} e6bm_memory_block_t;

typedef struct {
    e6bm_memory_block_t* blocks;
    size_t total_size;
    size_t used_size;
    pthread_mutex_t mutex;
} e6bm_memory_pool_t;

// ============================================================================
// Callback Types
// ============================================================================

// Progress callback for long operations
typedef void (*e6bm_progress_callback_t)(size_t current, size_t total, void* userdata);

// Error callback
typedef void (*e6bm_error_callback_t)(e6bm_error_t error, const char* message, void* userdata);

// ============================================================================
// Internal State
// ============================================================================

// Global library state
typedef struct {
    bool initialized;
    uint32_t init_count;          // Reference count for init/cleanup
    pthread_key_t tls_key;        // Thread-local storage key
    pthread_once_t init_once;     // One-time initialization control
    e6bm_function_table_t* func_table;
    e6bm_memory_pool_t* memory_pool;
    pthread_mutex_t global_mutex;
    
    // Configuration
    uint32_t flags;
    uint8_t global_key[32];       // Global encryption key
    uint8_t global_iv[16];        // Global IV
    
    // Statistics
    uint64_t total_encrypted;
    uint64_t total_decrypted;
    uint64_t total_compressed;
    uint64_t total_decompressed;
} e6bm_global_state_t;

// ============================================================================
// Constants
// ============================================================================

// Buffer sizes
#define E6BM_MAX_BUFFER_SIZE        (16 * 1024 * 1024)  // 16 MB
#define E6BM_DEFAULT_BUFFER_SIZE    (64 * 1024)         // 64 KB
#define E6BM_MIN_BUFFER_SIZE        1024                // 1 KB

// Limits
#define E6BM_MAX_KEY_SIZE           32
#define E6BM_MAX_IV_SIZE            16
#define E6BM_MAX_THREADS            256

// Timeouts (milliseconds)
#define E6BM_MUTEX_TIMEOUT          5000
#define E6BM_OPERATION_TIMEOUT      30000

#ifdef __cplusplus
}
#endif

#endif // E6BMFQAX5V_TYPES_H
