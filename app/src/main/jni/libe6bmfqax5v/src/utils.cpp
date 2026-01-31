/*
 * libe6bmfqax5v - Utility Functions Implementation
 */

#include "../include/utils.h"
#include "../include/e6bmfqax5v.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <android/log.h>

#define TAG "libe6bmfqax5v-utils"

// Global log level
static e6bm_log_level_t g_log_level = E6BM_LOG_LEVEL_INFO;

// ============================================================================
// Compression Functions (Zlib)
// ============================================================================

int e6bm_zlib_compress_data(const uint8_t* input, size_t input_len,
                            uint8_t* output, size_t* output_len,
                            int level) {
    if (input == NULL || output == NULL || output_len == NULL) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    z_stream stream;
    memset(&stream, 0, sizeof(stream));
    
    stream.next_in = (Bytef*)input;
    stream.avail_in = (uInt)input_len;
    stream.next_out = (Bytef*)output;
    stream.avail_out = (uInt)*output_len;
    
    if (deflateInit(&stream, level) != Z_OK) {
        return E6BM_ERROR_COMPRESSION;
    }
    
    int ret = deflate(&stream, Z_FINISH);
    deflateEnd(&stream);
    
    if (ret != Z_STREAM_END) {
        return E6BM_ERROR_COMPRESSION;
    }
    
    *output_len = stream.total_out;
    return E6BM_SUCCESS;
}

int e6bm_zlib_decompress_data(const uint8_t* input, size_t input_len,
                              uint8_t* output, size_t* output_len) {
    if (input == NULL || output == NULL || output_len == NULL) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    z_stream stream;
    memset(&stream, 0, sizeof(stream));
    
    stream.next_in = (Bytef*)input;
    stream.avail_in = (uInt)input_len;
    stream.next_out = (Bytef*)output;
    stream.avail_out = (uInt)*output_len;
    
    if (inflateInit(&stream) != Z_OK) {
        return E6BM_ERROR_COMPRESSION;
    }
    
    int ret = inflate(&stream, Z_FINISH);
    inflateEnd(&stream);
    
    if (ret != Z_STREAM_END) {
        return E6BM_ERROR_COMPRESSION;
    }
    
    *output_len = stream.total_out;
    return E6BM_SUCCESS;
}

size_t e6bm_zlib_compress_bound(size_t input_len) {
    return compressBound((uLong)input_len);
}

// ============================================================================
// Memory Management
// ============================================================================

void* e6bm_malloc(size_t size) {
    return malloc(size);
}

void* e6bm_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

void* e6bm_realloc(void* ptr, size_t size) {
    return realloc(ptr, size);
}

void e6bm_free(void* ptr) {
    free(ptr);
}

// ============================================================================
// String Utilities
// ============================================================================

size_t e6bm_strlcpy(char* dst, const char* src, size_t size) {
    size_t len = strlen(src);
    if (size > 0) {
        size_t n = (len >= size) ? size - 1 : len;
        memcpy(dst, src, n);
        dst[n] = '\0';
    }
    return len;
}

void e6bm_bin2hex(const uint8_t* bin, size_t bin_len, char* hex) {
    const char* hex_chars = "0123456789abcdef";
    for (size_t i = 0; i < bin_len; i++) {
        hex[i * 2] = hex_chars[(bin[i] >> 4) & 0x0F];
        hex[i * 2 + 1] = hex_chars[bin[i] & 0x0F];
    }
    hex[bin_len * 2] = '\0';
}

// ============================================================================
// Logging
// ============================================================================

void e6bm_set_log_level(e6bm_log_level_t level) {
    g_log_level = level;
}

e6bm_log_level_t e6bm_get_log_level(void) {
    return g_log_level;
}

void e6bm_log(e6bm_log_level_t level, const char* fmt, ...) {
    if (level > g_log_level) {
        return;
    }
    
    va_list args;
    va_start(args, fmt);
    
    int priority;
    switch (level) {
        case E6BM_LOG_LEVEL_ERROR: priority = ANDROID_LOG_ERROR; break;
        case E6BM_LOG_LEVEL_WARN:  priority = ANDROID_LOG_WARN;  break;
        case E6BM_LOG_LEVEL_INFO:  priority = ANDROID_LOG_INFO;  break;
        case E6BM_LOG_LEVEL_DEBUG: priority = ANDROID_LOG_DEBUG; break;
        case E6BM_LOG_LEVEL_TRACE: priority = ANDROID_LOG_VERBOSE; break;
        default: priority = ANDROID_LOG_INFO; break;
    }
    
    __android_log_vprint(priority, TAG, fmt, args);
    va_end(args);
}

void e6bm_hexdump(const void* data, size_t size, const char* label) {
    const uint8_t* bytes = (const uint8_t*)data;
    
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "=== %s (%zu bytes) ===", label, size);
    
    for (size_t i = 0; i < size; i += 16) {
        char line[128];
        char* p = line;
        
        p += sprintf(p, "%04zx: ", i);
        
        for (size_t j = 0; j < 16 && i + j < size; j++) {
            p += sprintf(p, "%02x ", bytes[i + j]);
        }
        
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "%s", line);
    }
}

// ============================================================================
// Time Functions
// ============================================================================

uint64_t e6bm_get_time_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000 + (uint64_t)tv.tv_usec / 1000;
}

uint64_t e6bm_get_time_us(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000000 + (uint64_t)tv.tv_usec;
}

void e6bm_sleep_ms(uint32_t ms) {
    usleep(ms * 1000);
}

// ============================================================================
// Error Handling
// ============================================================================

static __thread e6bm_error_t g_last_error = E6BM_SUCCESS;

void e6bm_set_last_error(e6bm_error_t error) {
    g_last_error = error;
}

e6bm_error_t e6bm_get_last_error(void) {
    return g_last_error;
}

void e6bm_clear_last_error(void) {
    g_last_error = E6BM_SUCCESS;
}

const char* e6bm_get_error_string(e6bm_error_t error) {
    switch (error) {
        case E6BM_SUCCESS: return "Success";
        case E6BM_ERROR_INVALID_PARAM: return "Invalid parameter";
        case E6BM_ERROR_MEMORY: return "Memory allocation failed";
        case E6BM_ERROR_CRYPTO: return "Cryptographic operation failed";
        case E6BM_ERROR_COMPRESSION: return "Compression/decompression failed";
        case E6BM_ERROR_IO: return "I/O error";
        case E6BM_ERROR_NOT_INITIALIZED: return "Library not initialized";
        case E6BM_ERROR_ALREADY_INITIALIZED: return "Already initialized";
        default: return "Unknown error";
    }
}

const char* e6bm_get_version(void) {
    return E6BM_VERSION_STRING;
}

// ============================================================================
// Crypto Utilities
// ============================================================================

int e6bm_secure_memcmp(const void* a, const void* b, size_t len) {
    const uint8_t* pa = (const uint8_t*)a;
    const uint8_t* pb = (const uint8_t*)b;
    uint8_t result = 0;
    
    for (size_t i = 0; i < len; i++) {
        result |= pa[i] ^ pb[i];
    }
    
    return result;
}

void e6bm_secure_memzero(void* ptr, size_t len) {
    volatile uint8_t* p = (volatile uint8_t*)ptr;
    while (len--) {
        *p++ = 0;
    }
}

int e6bm_random_bytes(uint8_t* buffer, size_t len) {
    FILE* fp = fopen("/dev/urandom", "rb");
    if (fp == NULL) {
        return E6BM_ERROR_IO;
    }
    
    size_t n = fread(buffer, 1, len, fp);
    fclose(fp);
    
    if (n != len) {
        return E6BM_ERROR_IO;
    }
    
    return E6BM_SUCCESS;
}

int e6bm_generate_iv(uint8_t* iv, size_t len) {
    return e6bm_random_bytes(iv, len);
}

// ============================================================================
// System Information
// ============================================================================

int e6bm_get_cpu_count(void) {
    return (int)sysconf(_SC_NPROCESSORS_ONLN);
}

size_t e6bm_get_page_size(void) {
    return (size_t)sysconf(_SC_PAGESIZE);
}

bool e6bm_is_android(void) {
    return true;  // This is Android-specific library
}
