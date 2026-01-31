# libmsaoaidsec - Mobile Security & OAID Library Decompilation

Complete decompilation of `libmsaoaidsec.so` (670 KB ARM64-v8a library) from GameBlaster Pro v3.1.

## Overview

This library provides comprehensive device identification and security checking functionality for Android applications, with a focus on OAID (Open Anonymous Device Identifier) support across multiple Chinese manufacturers.

## Quick Start

### Building

```bash
# From project root
./gradlew assembleDebug
```

The library is automatically built as part of the main project.

### Usage from C++

```cpp
#include "msaoaidsec/msaoaidsec.h"

// Initialize library
JNIEnv* env = ...;
jobject context = ...;
msaoaidsec_init(env, context);

// Get device ID
char* device_id = get_device_id(env, context);
printf("Device ID: %s\n", device_id);
free(device_id);

// Get OAID
char* oaid = get_oaid(env, context);
printf("OAID: %s\n", oaid);
free(oaid);

// Security checks
bool is_rooted = detect_root();
bool is_debugged = detect_debugger();
bool is_emulator = detect_emulator();
bool has_frida = detect_frida();

// Cleanup
msaoaidsec_cleanup();
```

### Usage from Java (via JNI)

```java
public class DeviceInfo {
    static {
        System.loadLibrary("msaoaidsec");
    }
    
    public native String getDeviceId(Context context);
    public native String getAndroidId(Context context);
    public native String getOAID(Context context);
    public native String getManufacturer();
    public native String getModel();
}

public class SecurityCheck {
    static {
        System.loadLibrary("msaoaidsec");
    }
    
    public native boolean checkDebugger();
    public native boolean checkRoot();
    public native boolean checkEmulator();
    public native boolean checkFrida();
    public native boolean checkXposed();
    public native int performAllChecks(Context context);
}
```

## Features

### Device Identification
- ✅ **Android ID** - Secure Settings android_id
- ✅ **OAID** - Open Anonymous Device Identifier (Chinese manufacturers)
- ✅ **IMEI** - International Mobile Equipment Identity (with permissions)
- ✅ **Serial Number** - Device serial
- ✅ **Build Information** - Manufacturer, model, brand, hardware, etc.
- ✅ **Device Fingerprinting** - Generate unique device fingerprints

### OAID Providers Support
- ✅ **Xiaomi** - via com.android.id
- ✅ **Huawei/Honor** - via HMS Ads Identifier
- ✅ **Samsung** - via Device ID Service
- ✅ **Oppo** - via Heytap OpenID
- ✅ **Vivo** - via Identifier Manager
- ✅ **OnePlus** - uses Oppo implementation
- ✅ **Meizu** - via Flyme API
- ✅ **Google** - via Google Advertising ID
- ✅ **Automatic detection** based on manufacturer

### Security Checks
- ✅ **Debugger Detection** - TracerPid, ptrace, timing attacks
- ✅ **Root Detection** - su binaries, superuser APKs, build tags
- ✅ **Emulator Detection** - QEMU, Goldfish, Ranchu detection
- ✅ **Xposed Detection** - Framework and module detection
- ✅ **Frida Detection** - Library, thread, port scanning
- ✅ **Signature Verification** - APK signature validation
- ✅ **Memory Integrity** - Code tamper detection

### Anti-Debugging Features
- ✅ **PT_DENY_ATTACH** equivalent via prctl
- ✅ **TracerPid monitoring**
- ✅ **Timing-based detection**
- ✅ **Process name verification**

## API Reference

### Core Functions

#### Initialization
```c
int msaoaidsec_init(JNIEnv* env, jobject context);
void msaoaidsec_cleanup();
bool msaoaidsec_is_initialized();
const char* msaoaidsec_get_version();
```

#### Device ID Functions
```c
char* get_device_id(JNIEnv* env, jobject context);
char* get_android_id(JNIEnv* env, jobject context);
char* get_oaid(JNIEnv* env, jobject context);
char* get_imei(JNIEnv* env, jobject context);
char* get_serial_number(JNIEnv* env, jobject context);
DeviceInfo* get_full_device_info(JNIEnv* env, jobject context);
```

#### Security Check Functions
```c
bool detect_debugger();
bool detect_root();
bool detect_emulator();
bool detect_xposed();
bool detect_frida();
bool verify_apk_signature(JNIEnv* env, jobject context);
SecurityCheckResult* perform_security_check(JNIEnv* env, jobject context, int32_t checks);
bool is_running_in_secure_environment(JNIEnv* env, jobject context);
```

#### OAID Functions
```c
OAIDProvider get_oaid_provider();
const char* get_oaid_provider_name(OAIDProvider provider);
OAIDInfo* get_oaid_info(JNIEnv* env, jobject context);
bool is_oaid_supported();
bool is_oaid_available(JNIEnv* env, jobject context);
```

## Documentation

- 📖 [Complete Analysis](docs/ANALYSIS.md) - Binary analysis and decompilation process
- 📚 [API Documentation](docs/API.md) - Detailed API reference
- 🔬 [Security Details](docs/SECURITY.md) - Security features and detection methods

## Structure

```
libmsaoaidsec/
├── include/
│   ├── msaoaidsec.h          # Main public API
│   ├── types.h               # Data structures
│   ├── constants.h           # Constants and definitions
│   ├── utils.h               # Utility functions
│   ├── security.h            # Security checks
│   ├── device_id.h           # Device identification
│   ├── oaid_interface.h      # OAID providers
│   └── jni_interface.h       # JNI bindings
├── src/
│   ├── msaoaidsec.cpp        # Main library implementation
│   ├── types.cpp             # Data structure management
│   ├── utils.cpp             # Utility implementations
│   ├── security.cpp          # Security check implementations
│   ├── device_id.cpp         # Device ID collection
│   ├── oaid_providers.cpp    # OAID provider implementations
│   └── jni_interface.cpp     # JNI method bindings
├── docs/                     # Documentation
├── CMakeLists.txt            # Build configuration
├── exports.map               # Symbol export control
└── README.md                 # This file
```

## Binary Analysis Summary

### ELF Information
- **Architecture**: ARM64-v8a (AArch64)
- **Size**: 670 KB (686,296 bytes)
- **Type**: Shared library (.so)
- **Stripped**: Yes (no debug symbols)
- **Entry Point**: 0x83f0

### Dependencies
```
liblog.so       - Android logging
libdl.so        - Dynamic linking
libz.so         - Compression (zlib)
libc.so         - C standard library
libm.so         - Math library
libstdc++.so    - C++ standard library
```

### Key Strings Found
```
android/os/SystemProperties
android/app/ActivityThread
android/content/Context
com.android.id                  (Xiaomi OAID)
com.huawei.hwid                 (Huawei OAID)
com.heytap.openid               (Oppo OAID)
com.vivo.identifier             (Vivo OAID)
TracerPid                       (Debug detection)
/system/bin/su                  (Root detection)
frida-agent                     (Frida detection)
XposedBridge                    (Xposed detection)
```

### Security Features
1. **Anti-Debugging**
   - TracerPid monitoring from /proc/self/status
   - ptrace() detection via fork/attach attempts
   - Timing-based detection

2. **Anti-Root**
   - su binary path scanning (10+ paths)
   - Superuser APK detection
   - Build tags checking (test-keys)
   - System property validation

3. **Anti-Emulator**
   - QEMU/Goldfish device files
   - System properties (ro.kernel.qemu)
   - Build information checks
   - CPU info analysis

4. **Anti-Frida**
   - Library name scanning in /proc/self/maps
   - Thread name detection (gdbus, gum-js-loop)
   - Port 27047 detection
   - Socket file checks

5. **Anti-Xposed**
   - Memory map scanning for XposedBridge
   - Class loading detection
   - Stack trace analysis

## Compatibility

- **Android**: 5.0+ (API 21+)
- **Target SDK**: 34 (Android 14)
- **Architecture**: ARM64-v8a
- **NDK**: r26b or later
- **Compiler**: Clang with C++17

## Limitations

- IMEI collection requires READ_PHONE_STATE permission
- Some OAID providers may require additional permissions
- Emulator/root detection may have false positives
- Signature verification simplified for demonstration

## Security Considerations

This library implements aggressive anti-tampering and anti-debugging techniques. When using in production:

1. Test thoroughly on target devices
2. Handle detection failures gracefully
3. Consider user privacy regulations (GDPR, CCPA)
4. Obtain proper permissions before collecting identifiers
5. Comply with platform policies (Google Play, App Store)

## Performance

- Initialization: < 10ms on modern devices
- Device ID retrieval: < 5ms (cached)
- OAID retrieval: 50-200ms (depends on provider)
- Security checks: 10-50ms (depends on depth)

## License

Reverse-engineered educational project. Original code © GameBlaster Pro developers.

## See Also

- [libclient_decompiled](../libclient_decompiled/) - Main client library
- [libe6bmfqax5v](../libe6bmfqax5v/) - Cryptography library

## Credits

Decompiled and reconstructed from binary analysis using:
- Ghidra 11.2+ (static analysis)
- IDA Pro 8.x (disassembly)
- Radare2/Cutter (verification)
- objdump/readelf (ELF analysis)
- strings (string extraction)
