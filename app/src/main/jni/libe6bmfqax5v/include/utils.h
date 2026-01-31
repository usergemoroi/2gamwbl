/*
 * libe6bmfqax5v - Utility Functions
 * 
 * Helper functions for memory management, compression, and other utilities
 */

#ifndef E6BMFQAX5V_UTILS_H
#define E6BMFQAX5V_UTILS_H

#include "types.h"
#include <zlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Memory Management
// ============================================================================

/**
 * Allocate memory with alignment
 */
void* e6bm_aligned_malloc(size_t size, size_t alignment);

/**
 * Free aligned memory
 */
void e6bm_aligned_free(void* ptr);

/**
 * Memory pool initialization
 */
int e6bm_mempool_init(e6bm_memory_pool_t* pool, size_t initial_size);

/**
 * Memory pool cleanup
 */
void e6bm_mempool_cleanup(e6bm_memory_pool_t* pool);

/**
 * Allocate from memory pool
 */
void* e6bm_mempool_alloc(e6bm_memory_pool_t* pool, size_t size);

/**
 * Free to memory pool
 */
void e6bm_mempool_free(e6bm_memory_pool_t* pool, void* ptr);

// ============================================================================
// Compression Functions (Zlib)
// ============================================================================

/**
 * Initialize zlib context
 */
int e6bm_zlib_init(e6bm_zlib_context_t* ctx, int level);

/**
 * Cleanup zlib context
 */
void e6bm_zlib_cleanup(e6bm_zlib_context_t* ctx);

/**
 * Compress data with zlib
 */
int e6bm_zlib_compress_data(const uint8_t* input, size_t input_len,
                            uint8_t* output, size_t* output_len,
                            int level);

/**
 * Decompress zlib data
 */
int e6bm_zlib_decompress_data(const uint8_t* input, size_t input_len,
                              uint8_t* output, size_t* output_len);

/**
 * Get maximum compressed size for given input
 */
size_t e6bm_zlib_compress_bound(size_t input_len);

// ============================================================================
// String Utilities
// ============================================================================

/**
 * Safe string copy
 */
size_t e6bm_strlcpy(char* dst, const char* src, size_t size);

/**
 * Safe string concatenation
 */
size_t e6bm_strlcat(char* dst, const char* src, size_t size);

/**
 * String to hex conversion
 */
void e6bm_bin2hex(const uint8_t* bin, size_t bin_len, char* hex);

/**
 * Hex to string conversion
 */
int e6bm_hex2bin(const char* hex, uint8_t* bin, size_t bin_len);

// ============================================================================
// Base64 Encoding/Decoding
// ============================================================================

/**
 * Calculate base64 encoded size
 */
size_t e6bm_base64_encode_size(size_t input_len);

/**
 * Calculate base64 decoded size
 */
size_t e6bm_base64_decode_size(const char* encoded);

/**
 * Base64 encode
 */
int e6bm_base64_encode(const uint8_t* input, size_t input_len,
                       char* output, size_t output_size);

/**
 * Base64 decode
 */
int e6bm_base64_decode(const char* input, uint8_t* output, size_t* output_len);

// ============================================================================
// File I/O Utilities
// ============================================================================

/**
 * Read entire file into memory
 */
int e6bm_read_file(const char* path, uint8_t** data, size_t* size);

/**
 * Write data to file
 */
int e6bm_write_file(const char* path, const uint8_t* data, size_t size);

/**
 * Check if file exists
 */
bool e6bm_file_exists(const char* path);

/**
 * Get file size
 */
ssize_t e6bm_file_size(const char* path);

// ============================================================================
// Logging and Debugging
// ============================================================================

// Log levels
typedef enum {
    E6BM_LOG_LEVEL_NONE = 0,
    E6BM_LOG_LEVEL_ERROR,
    E6BM_LOG_LEVEL_WARN,
    E6BM_LOG_LEVEL_INFO,
    E6BM_LOG_LEVEL_DEBUG,
    E6BM_LOG_LEVEL_TRACE
} e6bm_log_level_t;

/**
 * Set logging level
 */
void e6bm_set_log_level(e6bm_log_level_t level);

/**
 * Get current log level
 */
e6bm_log_level_t e6bm_get_log_level(void);

/**
 * Log message
 */
void e6bm_log(e6bm_log_level_t level, const char* fmt, ...);

// Logging macros
#define E6BM_LOG_ERROR(fmt, ...) e6bm_log(E6BM_LOG_LEVEL_ERROR, "[ERROR] " fmt, ##__VA_ARGS__)
#define E6BM_LOG_WARN(fmt, ...)  e6bm_log(E6BM_LOG_LEVEL_WARN,  "[WARN]  " fmt, ##__VA_ARGS__)
#define E6BM_LOG_INFO(fmt, ...)  e6bm_log(E6BM_LOG_LEVEL_INFO,  "[INFO]  " fmt, ##__VA_ARGS__)
#define E6BM_LOG_DEBUG(fmt, ...) e6bm_log(E6BM_LOG_LEVEL_DEBUG, "[DEBUG] " fmt, ##__VA_ARGS__)
#define E6BM_LOG_TRACE(fmt, ...) e6bm_log(E6BM_LOG_LEVEL_TRACE, "[TRACE] " fmt, ##__VA_ARGS__)

/**
 * Hex dump for debugging
 */
void e6bm_hexdump(const void* data, size_t size, const char* label);

// ============================================================================
// Time and Performance
// ============================================================================

/**
 * Get current time in milliseconds
 */
uint64_t e6bm_get_time_ms(void);

/**
 * Get current time in microseconds
 */
uint64_t e6bm_get_time_us(void);

/**
 * Sleep for milliseconds
 */
void e6bm_sleep_ms(uint32_t ms);

// Performance timer
typedef struct {
    uint64_t start_time;
    uint64_t end_time;
    const char* label;
} e6bm_timer_t;

/**
 * Start performance timer
 */
void e6bm_timer_start(e6bm_timer_t* timer, const char* label);

/**
 * Stop performance timer and print elapsed time
 */
void e6bm_timer_stop(e6bm_timer_t* timer);

// ============================================================================
// System Information
// ============================================================================

/**
 * Get CPU count
 */
int e6bm_get_cpu_count(void);

/**
 * Get page size
 */
size_t e6bm_get_page_size(void);

/**
 * Get system property (Android)
 */
int e6bm_get_system_property(const char* key, char* value, size_t value_len);

/**
 * Check if running on Android
 */
bool e6bm_is_android(void);

/**
 * Get Android SDK version
 */
int e6bm_get_android_sdk_version(void);

// ============================================================================
// Dynamic Symbol Resolution
// ============================================================================

/**
 * Load function from library
 */
void* e6bm_dlsym(void* handle, const char* symbol);

/**
 * Open dynamic library
 */
void* e6bm_dlopen(const char* path, int flags);

/**
 * Close dynamic library
 */
int e6bm_dlclose(void* handle);

/**
 * Get last dynamic linker error
 */
const char* e6bm_dlerror(void);

// ============================================================================
// Obfuscation Helpers
// ============================================================================

/**
 * XOR obfuscation/deobfuscation
 */
void e6bm_xor_buffer(uint8_t* buffer, size_t len, uint8_t key);

/**
 * Simple string obfuscation (for hiding string constants)
 */
void e6bm_deobfuscate_string(const uint8_t* obfuscated, size_t len, char* output);

/**
 * Calculate simple checksum
 */
uint32_t e6bm_checksum32(const uint8_t* data, size_t len);

/**
 * Calculate CRC32
 */
uint32_t e6bm_crc32(const uint8_t* data, size_t len);

// ============================================================================
// Error Handling
// ============================================================================

/**
 * Set last error code
 */
void e6bm_set_last_error(e6bm_error_t error);

/**
 * Get last error code
 */
e6bm_error_t e6bm_get_last_error(void);

/**
 * Clear last error
 */
void e6bm_clear_last_error(void);

/**
 * Error code to string
 */
const char* e6bm_error_to_string(e6bm_error_t error);

// ============================================================================
// Atomic Operations
// ============================================================================

/**
 * Atomic increment
 */
int32_t e6bm_atomic_inc(volatile int32_t* ptr);

/**
 * Atomic decrement
 */
int32_t e6bm_atomic_dec(volatile int32_t* ptr);

/**
 * Atomic compare and swap
 */
bool e6bm_atomic_cas(volatile int32_t* ptr, int32_t old_val, int32_t new_val);

// ============================================================================
// Miscellaneous
// ============================================================================

/**
 * Generate UUID/GUID
 */
void e6bm_generate_uuid(char* uuid_str, size_t size);

/**
 * Get device ID (Android-specific)
 */
int e6bm_get_device_id(char* device_id, size_t size);

/**
 * Check if debugger is attached
 */
bool e6bm_is_debugger_attached(void);

#ifdef __cplusplus
}
#endif

#endif // E6BMFQAX5V_UTILS_H
