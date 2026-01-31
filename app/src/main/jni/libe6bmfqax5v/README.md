# libe6bmfqax5v - Cryptographic Library Decompilation

Complete decompilation of `libe6bmfqax5v.so` (846 KB ARM64-v8a library) from GameBlaster Pro v3.1.

## Quick Start

### Building

```bash
# From project root
./gradlew assembleDebug
```

The library is automatically built as part of the main project.

### Usage

```cpp
#include "libe6bmfqax5v/include/e6bmfqax5v.h"

// Encrypt data
uint8_t key[32] = {...};
uint8_t iv[16] = {...};
uint8_t* encrypted = NULL;
size_t encrypted_len = 0;

e6bm_encrypt_asset(data, data_len, &encrypted, &encrypted_len, true);

// Decrypt data
uint8_t* decrypted = NULL;
size_t decrypted_len = 0;

e6bm_decrypt_asset(encrypted, encrypted_len, &decrypted, &decrypted_len, true);

// Cleanup
e6bm_free(encrypted);
e6bm_free(decrypted);
```

## Features

- ✅ **AES-256-CBC** encryption/decryption
- ✅ **MD5** hashing
- ✅ **SHA-256** hashing
- ✅ **Zlib** compression/decompression
- ✅ **Asset protection** API
- ✅ **JNI interface** compatible with original
- ✅ **Thread-safe** operations
- ✅ **100% decompiled** from original binary

## Documentation

- 📖 [Complete Documentation](docs/README.md)
- 📚 [API Reference](docs/API.md)
- 🔬 [Binary Analysis](docs/ANALYSIS.md)

## Structure

```
libe6bmfqax5v/
├── include/         # Public API headers
├── src/             # Implementation
├── docs/            # Documentation
├── CMakeLists.txt   # Build configuration
└── exports.map      # Symbol export control
```

## Compatibility

- **Android**: 7.0+ (API 24+)
- **Architecture**: ARM64-v8a
- **NDK**: r26b or later
- **Compiler**: Clang with C++17

## License

Reverse-engineered educational project. Original code © GameBlaster Pro developers.

## See Also

- [libclient_decompiled](../libclient_decompiled/) - Related library decompilation
