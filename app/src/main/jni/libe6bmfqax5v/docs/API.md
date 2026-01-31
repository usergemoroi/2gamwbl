# libe6bmfqax5v API Reference

## Table of Contents

1. [Core Functions](#core-functions)
2. [Cryptographic Functions](#cryptographic-functions)
3. [Compression Functions](#compression-functions)
4. [Utility Functions](#utility-functions)
5. [JNI Interface](#jni-interface)
6. [Error Codes](#error-codes)
7. [Data Types](#data-types)

---

## Core Functions

### e6bm_get_version

```c
const char* e6bm_get_version(void);
```

Returns the library version string.

**Returns**: Version string (e.g., "3.1.0")

**Example**:
```c
printf("Library version: %s\n", e6bm_get_version());
```

### e6bm_get_error_string

```c
const char* e6bm_get_error_string(e6bm_error_t error);
```

Converts an error code to a human-readable string.

**Parameters**:
- `error`: Error code

**Returns**: Error message string

---

## Cryptographic Functions

### AES-256-CBC

#### e6bm_aes256_encrypt

```c
int e6bm_aes256_encrypt(const uint8_t* input, size_t input_len,
                        uint8_t* output, size_t* output_len,
                        const uint8_t* key, const uint8_t* iv);
```

Encrypts data using AES-256-CBC.

**Parameters**:
- `input`: Input plaintext data
- `input_len`: Input data length (must be multiple of 16)
- `output`: Output buffer for ciphertext
- `output_len`: [in/out] Output buffer size / actual encrypted size
- `key`: 32-byte encryption key
- `iv`: 16-byte initialization vector

**Returns**: `E6BM_SUCCESS` or error code

**Example**:
```c
uint8_t key[32] = {...};
uint8_t iv[16] = {...};
uint8_t plaintext[64] = "Secret message";
uint8_t ciphertext[64];
size_t out_len = 64;

int ret = e6bm_aes256_encrypt(plaintext, 64, ciphertext, &out_len, key, iv);
if (ret != E6BM_SUCCESS) {
    fprintf(stderr, "Encryption failed: %s\n", e6bm_get_error_string(ret));
}
```

#### e6bm_aes256_decrypt

```c
int e6bm_aes256_decrypt(const uint8_t* input, size_t input_len,
                        uint8_t* output, size_t* output_len,
                        const uint8_t* key, const uint8_t* iv);
```

Decrypts data using AES-256-CBC.

**Parameters**: Same as `e6bm_aes256_encrypt`

**Returns**: `E6BM_SUCCESS` or error code

### Hash Functions

#### e6bm_md5_hash

```c
int e6bm_md5_hash(const uint8_t* input, size_t input_len, uint8_t* output);
```

Computes MD5 hash (128-bit).

**Parameters**:
- `input`: Input data
- `input_len`: Input data length
- `output`: 16-byte output buffer for hash

**Returns**: `E6BM_SUCCESS` or error code

**Example**:
```c
uint8_t data[] = "Hello, World!";
uint8_t hash[16];

e6bm_md5_hash(data, sizeof(data), hash);

// Print hash in hex
for (int i = 0; i < 16; i++) {
    printf("%02x", hash[i]);
}
printf("\n");
```

#### e6bm_sha256_hash

```c
int e6bm_sha256_hash(const uint8_t* input, size_t input_len, uint8_t* output);
```

Computes SHA-256 hash (256-bit).

**Parameters**:
- `input`: Input data
- `input_len`: Input data length
- `output`: 32-byte output buffer for hash

**Returns**: `E6BM_SUCCESS` or error code

---

## Compression Functions

### e6bm_zlib_compress

```c
int e6bm_zlib_compress(const uint8_t* input, size_t input_len,
                       uint8_t* output, size_t* output_len, int level);
```

Compresses data using zlib.

**Parameters**:
- `input`: Input data
- `input_len`: Input data length
- `output`: Output buffer
- `output_len`: [in/out] Output buffer size / actual compressed size
- `level`: Compression level (0-9, where 9 is best compression)

**Returns**: `E6BM_SUCCESS` or error code

**Example**:
```c
uint8_t input[1024] = {...};
size_t output_len = e6bm_zlib_compress_bound(1024);
uint8_t* output = malloc(output_len);

int ret = e6bm_zlib_compress(input, 1024, output, &output_len, 9);
printf("Compressed %zu -> %zu bytes (%.1f%%)\n", 
       1024, output_len, (output_len * 100.0) / 1024);
```

### e6bm_zlib_decompress

```c
int e6bm_zlib_decompress(const uint8_t* input, size_t input_len,
                         uint8_t* output, size_t* output_len);
```

Decompresses zlib data.

**Parameters**:
- `input`: Compressed input data
- `input_len`: Compressed data length
- `output`: Output buffer for decompressed data
- `output_len`: [in/out] Output buffer size / actual decompressed size

**Returns**: `E6BM_SUCCESS` or error code

---

## Asset Encryption API

### e6bm_encrypt_asset

```c
int e6bm_encrypt_asset(const uint8_t* plain_data, size_t plain_len,
                       uint8_t** encrypted_data, size_t* encrypted_len,
                       bool compress);
```

High-level asset encryption with optional compression.

**Parameters**:
- `plain_data`: Plain asset data
- `plain_len`: Plain data length
- `encrypted_data`: [out] Pointer to receive allocated encrypted data (caller must free)
- `encrypted_len`: [out] Encrypted data length
- `compress`: Apply zlib compression before encryption

**Returns**: `E6BM_SUCCESS` or error code

**Features**:
- Optional zlib compression
- Random IV generation per asset
- SHA-256 integrity checksum
- Asset metadata header

**Example**:
```c
uint8_t asset_data[10000] = {...};
uint8_t* encrypted = NULL;
size_t encrypted_len = 0;

int ret = e6bm_encrypt_asset(asset_data, 10000, &encrypted, &encrypted_len, true);
if (ret == E6BM_SUCCESS) {
    // Use encrypted data...
    save_to_file("asset.enc", encrypted, encrypted_len);
    
    // Free when done
    e6bm_free(encrypted);
}
```

### e6bm_decrypt_asset

```c
int e6bm_decrypt_asset(const uint8_t* encrypted_data, size_t encrypted_len,
                       uint8_t** plain_data, size_t* plain_len,
                       bool decompress);
```

High-level asset decryption with optional decompression.

**Parameters**:
- `encrypted_data`: Encrypted asset data
- `encrypted_len`: Encrypted data length
- `plain_data`: [out] Pointer to receive allocated decrypted data (caller must free)
- `plain_len`: [out] Decrypted data length
- `decompress`: Apply zlib decompression after decryption

**Returns**: `E6BM_SUCCESS` or error code

**Example**:
```c
uint8_t* encrypted = load_from_file("asset.enc", &encrypted_len);
uint8_t* decrypted = NULL;
size_t decrypted_len = 0;

int ret = e6bm_decrypt_asset(encrypted, encrypted_len, &decrypted, &decrypted_len, true);
if (ret == E6BM_SUCCESS) {
    // Use decrypted data...
    process_asset(decrypted, decrypted_len);
    
    // Free when done
    e6bm_free(decrypted);
}
free(encrypted);
```

---

## Utility Functions

### Memory Management

#### e6bm_malloc / e6bm_free

```c
void* e6bm_malloc(size_t size);
void e6bm_free(void* ptr);
```

Allocate and free memory. Use these for data allocated by the library.

### Logging

#### e6bm_set_log_level

```c
void e6bm_set_log_level(e6bm_log_level_t level);
```

Sets the logging verbosity level.

**Levels**:
- `E6BM_LOG_LEVEL_NONE`: No logging
- `E6BM_LOG_LEVEL_ERROR`: Errors only
- `E6BM_LOG_LEVEL_WARN`: Warnings and errors
- `E6BM_LOG_LEVEL_INFO`: Informational messages (default)
- `E6BM_LOG_LEVEL_DEBUG`: Debug messages
- `E6BM_LOG_LEVEL_TRACE`: Trace messages

### Security Utilities

#### e6bm_secure_memzero

```c
void e6bm_secure_memzero(void* ptr, size_t len);
```

Securely clears memory (prevents compiler optimization).

**Use**: Clear sensitive data (keys, passwords) after use.

#### e6bm_random_bytes

```c
int e6bm_random_bytes(uint8_t* buffer, size_t len);
```

Generates cryptographically secure random bytes using `/dev/urandom`.

**Example**:
```c
uint8_t key[32];
e6bm_random_bytes(key, 32);  // Generate 256-bit key
```

---

## JNI Interface

### JNI_OnLoad

```c
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved);
```

Automatically called when library is loaded.

**Returns**: JNI_VERSION_1_6

### Mundo_Activate_SDK

```c
JNIEXPORT jint JNICALL Mundo_Activate_SDK(JNIEnv* env, jobject thiz,
                                          jlong param1, jlong param2);
```

SDK activation function called from Java.

**Parameters**:
- `env`: JNI environment
- `thiz`: Java object reference
- `param1`: Configuration flags
- `param2`: Reserved

**Returns**: 0 on success, negative error code on failure

**Java Usage**:
```java
public native int Mundo_Activate_SDK(long param1, long param2);
```

---

## Error Codes

```c
typedef enum {
    E6BM_SUCCESS = 0,                      // Operation successful
    E6BM_ERROR_INVALID_PARAM = -1,         // Invalid parameter
    E6BM_ERROR_MEMORY = -2,                // Memory allocation failed
    E6BM_ERROR_CRYPTO = -3,                // Cryptographic operation failed
    E6BM_ERROR_COMPRESSION = -4,           // Compression/decompression failed
    E6BM_ERROR_IO = -5,                    // I/O error
    E6BM_ERROR_NOT_INITIALIZED = -6,       // Library not initialized
    E6BM_ERROR_ALREADY_INITIALIZED = -7    // Already initialized
} e6bm_error_t;
```

---

## Data Types

### e6bm_asset_header_t

Asset encryption header structure:

```c
typedef struct {
    uint32_t magic;              // Magic number: 0x36426D45
    uint32_t version;            // Format version
    uint32_t flags;              // Encryption flags
    uint32_t original_size;      // Original uncompressed size
    uint32_t compressed_size;    // Compressed size
    uint32_t encrypted_size;     // Final encrypted size
    uint8_t iv[16];              // Initialization vector
    uint8_t checksum[32];        // SHA-256 checksum
} __attribute__((packed)) e6bm_asset_header_t;
```

**Flags**:
- `E6BM_ASSET_FLAG_COMPRESSED` (0x01): Data is compressed
- `E6BM_ASSET_FLAG_ENCRYPTED` (0x02): Data is encrypted
- `E6BM_ASSET_FLAG_VERIFIED` (0x04): Checksum verified

---

## Constants

```c
#define E6BM_VERSION_STRING "3.1.0"
#define E6BM_ENCRYPTION_KEY "Burri Burri Encryption Key v3.1"
#define E6BM_ENCRYPTION_IV "GameBlasterPro\0\0"
#define E6BM_AES_KEY_SIZE 32
#define E6BM_AES_BLOCK_SIZE 16
#define E6BM_AES_IV_SIZE 16
#define E6BM_ASSET_MAGIC 0x36426D45
```

---

## Thread Safety

- **Thread-safe functions**: All public API functions are thread-safe
- **Context isolation**: Each thread has its own crypto context
- **Mutex protection**: Global state protected by mutex
- **TLS**: Thread-local storage for per-thread data

---

## Performance Considerations

1. **Buffer Size**: Pre-allocate output buffers with sufficient size
2. **Compression**: Test if compression actually reduces size
3. **Batch Operations**: Process multiple assets in parallel
4. **Memory Pooling**: Reuse buffers for repeated operations

---

## Migration Guide

### From Original Library

The decompiled library is API-compatible with the original:

```c
// Old code
System.loadLibrary("e6bmfqax5v");
Mundo_Activate_SDK(0, 0);

// Still works with decompiled version
```

### Adding New Features

To extend the library:

1. Add function declarations to appropriate header
2. Implement in corresponding .cpp file
3. Update CMakeLists.txt if adding new files
4. Document in this API reference

---

## Examples

See [examples/](../examples/) directory for complete working examples:

- `basic_encrypt.cpp` - Basic encryption/decryption
- `asset_protection.cpp` - Asset encryption workflow
- `hash_verification.cpp` - File integrity checking
- `jni_integration.cpp` - JNI integration example
