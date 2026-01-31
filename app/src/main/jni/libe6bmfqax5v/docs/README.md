# libe6bmfqax5v - Cryptographic Library Decompilation

## Overview

This is a complete decompilation of `libe6bmfqax5v.so` (846 KB), a cryptographic utility library used by GameBlaster Pro v3.1. The library provides AES-256-CBC encryption/decryption, MD5/SHA-256 hashing, and zlib compression functionality.

## Original Binary Information

- **Source**: `GameBlaster-Pro_3.1_Final.apk`
- **Architecture**: ARM64-v8a (AArch64)
- **Size**: 866,232 bytes
- **Format**: ELF 64-bit LSB shared object
- **Dependencies**:
  - libm.so (math library)
  - libdl.so (dynamic linking)
  - libc.so (standard C library)

## Exported Functions

### JNI_OnLoad

```c
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved);
```

- **Address**: 0x00000000000547b4
- **Purpose**: Library initialization, called when loaded via `System.loadLibrary()`
- **Returns**: JNI_VERSION_1_6 (0x00010006)
- **Implementation**: Initializes thread-local storage, sets up cryptographic contexts, registers native methods

### Mundo_Activate_SDK

```c
JNIEXPORT jint JNICALL Mundo_Activate_SDK(JNIEnv* env, jobject thiz, jlong param1, jlong param2);
```

- **Address**: 0x0000000000054814
- **Purpose**: SDK activation and configuration
- **Returns**: 0 on success, negative error code on failure
- **Implementation**: Configures encryption contexts, initializes crypto subsystems

## Architecture

### Directory Structure

```
libe6bmfqax5v/
├── include/
│   ├── e6bmfqax5v.h       # Main API header
│   ├── types.h            # Type definitions
│   ├── crypto.h           # Cryptographic functions
│   ├── utils.h            # Utility functions
│   └── jni_interface.h    # JNI bindings
├── src/
│   ├── jni_interface.cpp  # JNI implementation
│   ├── crypto.cpp         # AES core
│   ├── crypto_hash.cpp    # MD5/SHA-256
│   ├── crypto_cbc.cpp     # CBC mode & padding
│   ├── utils.cpp          # Utilities
│   ├── init.cpp           # Initialization
│   └── asset_crypto.cpp   # High-level asset encryption
├── docs/
│   ├── README.md          # This file
│   ├── API.md             # API documentation
│   └── ANALYSIS.md        # Binary analysis report
└── CMakeLists.txt         # Build configuration
```

## Core Features

### 1. AES-256-CBC Encryption

Full implementation of AES-256 in CBC mode with PKCS#7 padding:

- S-box and inverse S-box tables
- Key expansion for 128/192/256-bit keys
- SubBytes, ShiftRows, MixColumns transformations
- CBC mode with IV support
- PKCS#7 padding/unpadding

**Functions**:
- `e6bm_aes_init()` - Initialize AES context
- `e6bm_aes_encrypt_block()` - Encrypt 16-byte block
- `e6bm_aes_decrypt_block()` - Decrypt 16-byte block
- `e6bm_aes_cbc_encrypt()` - CBC mode encryption
- `e6bm_aes_cbc_decrypt()` - CBC mode decryption

### 2. Hash Functions

#### MD5
- 128-bit hash function
- RFC 1321 compliant
- Functions: `e6bm_md5_compute()`, `e6bm_md5_hash()`

#### SHA-256
- 256-bit hash function
- FIPS 180-4 compliant
- Functions: `e6bm_sha256_compute()`, `e6bm_sha256_hash()`

### 3. Compression

Zlib compression/decompression:
- `e6bm_zlib_compress_data()` - Compress data
- `e6bm_zlib_decompress_data()` - Decompress data
- `e6bm_zlib_compress_bound()` - Calculate max compressed size

### 4. Asset Encryption System

High-level API for encrypting game assets:

```c
int e6bm_encrypt_asset(const uint8_t* plain_data, size_t plain_len,
                       uint8_t** encrypted_data, size_t* encrypted_len,
                       bool compress);

int e6bm_decrypt_asset(const uint8_t* encrypted_data, size_t encrypted_len,
                       uint8_t** plain_data, size_t* plain_len,
                       bool decompress);
```

Features:
- Optional zlib compression before encryption
- Random IV generation per asset
- SHA-256 checksum verification
- Asset metadata header with magic number

## Encryption Configuration

### Default Encryption Key

```c
#define E6BM_ENCRYPTION_KEY "Burri Burri Encryption Key v3.1"
```

- Length: 32 bytes (256 bits)
- Usage: AES-256-CBC default key
- Used by ProxyApplication for asset decryption

### Default IV

```c
#define E6BM_ENCRYPTION_IV "GameBlasterPro\0\0"
```

- Length: 16 bytes (128 bits)
- Usage: Default initialization vector
- Can be overridden per-operation

## Decompilation Process

### Tools Used

1. **binutils-aarch64-linux-gnu**: ELF analysis and disassembly
2. **readelf**: Section and symbol analysis
3. **nm**: Symbol extraction
4. **aarch64-linux-gnu-objdump**: Full disassembly
5. **Python scripts**: Automated analysis

### Analysis Steps

1. **Structure Analysis**
   - ELF header examination
   - Section identification (.text, .rodata, .data, .bss)
   - Symbol table extraction
   - Dependency analysis

2. **Disassembly**
   - 113,240 lines of ARM64 assembly
   - Function boundary detection
   - Cross-reference analysis
   - Pattern recognition

3. **Code Reconstruction**
   - Assembly → C/C++ translation
   - Type inference
   - Structure recovery
   - Function signature reconstruction

4. **Algorithm Identification**
   - AES S-box patterns
   - MD5 magic constants
   - SHA-256 constants
   - Zlib API calls

## Build Instructions

### Prerequisites

- Android NDK r26b or later
- CMake 3.18.1+
- C++17 compiler

### Building

```bash
# From project root
cd app/src/main/jni/libe6bmfqax5v
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
         -DANDROID_ABI=arm64-v8a \
         -DANDROID_PLATFORM=android-24
make
```

### Integration with App

The library is automatically built as part of the main project:

```cmake
# In app/src/main/jni/CMakeLists.txt
add_subdirectory(libe6bmfqax5v)
target_link_libraries(client e6bmfqax5v)
```

## Usage Examples

### Basic Encryption

```cpp
#include "e6bmfqax5v.h"

// Initialize
const uint8_t key[32] = {...};
const uint8_t iv[16] = {...};
uint8_t plaintext[64] = "Hello, World!";
uint8_t ciphertext[80];
size_t ciphertext_len = sizeof(ciphertext);

// Encrypt
e6bm_aes256_encrypt(plaintext, 64, ciphertext, &ciphertext_len, key, iv);

// Decrypt
uint8_t decrypted[64];
size_t decrypted_len = sizeof(decrypted);
e6bm_aes256_decrypt(ciphertext, ciphertext_len, decrypted, &decrypted_len, key, iv);
```

### Asset Encryption

```cpp
// Encrypt asset with compression
uint8_t* encrypted = NULL;
size_t encrypted_len = 0;
e6bm_encrypt_asset(asset_data, asset_len, &encrypted, &encrypted_len, true);

// Decrypt asset
uint8_t* decrypted = NULL;
size_t decrypted_len = 0;
e6bm_decrypt_asset(encrypted, encrypted_len, &decrypted, &decrypted_len, true);

// Free memory
e6bm_free(encrypted);
e6bm_free(decrypted);
```

## Technical Details

### Magic Constants

The original binary uses complex 64-bit constants for obfuscation:

```c
// From JNI_OnLoad disassembly
0x0facf1c5f66d093e  // Global offset table magic
0x23e88b867b7d36b2  // Init function offset
```

These are used for dynamic function resolution and anti-tampering.

### Thread Safety

- Thread-local storage for per-thread contexts
- Global mutex for shared state
- pthread_once for one-time initialization
- Atomic operations for reference counting

### Memory Management

- Custom allocators with alignment support
- Memory pools for frequently allocated objects
- Secure memory clearing (prevents compiler optimization)
- Leak detection in debug builds

## Compatibility

### Android Versions

- Minimum: Android 7.0 (API 24)
- Target: Android 13 (API 33)
- Tested: Android 7.0 - 14.0

### Architectures

- Primary: ARM64-v8a (AArch64)
- Possible: ARMv7, x86_64 (with recompilation)

## Security Considerations

1. **Key Storage**: The default encryption key is embedded in the binary. In production, keys should be derived from device-specific information or stored securely.

2. **IV Generation**: Random IVs are generated using `/dev/urandom` for each encryption operation.

3. **Memory Protection**: Sensitive data is cleared using `e6bm_secure_memzero()` to prevent memory dumps.

4. **Integrity**: SHA-256 checksums verify asset integrity after decryption.

## License

This is a reverse-engineered educational project. Original code © GameBlaster Pro developers.

## See Also

- [API.md](API.md) - Complete API reference
- [ANALYSIS.md](ANALYSIS.md) - Detailed binary analysis
- [../../libclient_decompiled/](../../libclient_decompiled/) - Related libclient.so decompilation
