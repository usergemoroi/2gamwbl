# GameBlaster Pro v3.1 - Decompiled Source Code

## Overview

This repository contains the fully decompiled and reconstructed source code of **GameBlaster Pro v3.1**, an Android application originally distributed as `GameBlaster-Pro_3.1_Final.apk`.

**Important Notice:** This is a decompilation and reconstruction for educational and research purposes only. All code has been reverse-engineered from the original APK binary.

## Application Information

- **Package Name:** `com.eternal.xdsdk`
- **Version:** 3.1 (versionCode: 31)
- **Target SDK:** Android 14 (API 36)
- **Min SDK:** Android 7.0 (API 24)
- **Architecture:** ARM64-v8a, ARMv7, x86, x86_64

## Project Structure

```
GameBlasterPro/
├── app/
│   ├── src/
│   │   ├── main/
│   │   │   ├── java/
│   │   │   │   ├── p00093e85d7aface4dfb/     # Proxy loader classes
│   │   │   │   │   ├── ProxyApplication.java
│   │   │   │   │   ├── ProxyComponentFactory.java
│   │   │   │   │   ├── JniBridge.java
│   │   │   │   │   └── ...
│   │   │   │   ├── bfd4ecafa7d58e39/         # Placeholder activity
│   │   │   │   │   └── MainActivity.java
│   │   │   │   └── com/eternal/xdsdk/       # Main application package
│   │   │   │       └── R.java
│   │   │   ├── jni/                         # Native C++ libraries
│   │   │   │   ├── libclient/               # Main client library (4.6MB)
│   │   │   │   │   ├── client.cpp
│   │   │   │   │   └── client.h
│   │   │   │   ├── lib2f8c0b3257fcc345/      # Burri encrypted loader
│   │   │   │   │   └── burri_loader.cpp
│   │   │   │   ├── libe6bmfqax5v/           # Auxiliary crypto library (846KB)
│   │   │   │   │   └── auxiliary.cpp
│   │   │   │   ├── libmsaoaidsec/           # Security library (670KB)
│   │   │   │   │   └── security.cpp
│   │   │   │   └── CMakeLists.txt
│   │   │   ├── res/                         # Android resources
│   │   │   │   ├── values/
│   │   │   │   ├── layout/
│   │   │   │   ├── drawable/
│   │   │   │   └── ...
│   │   │   ├── assets/                      # Application assets
│   │   │   │   ├── burriEnc/               # Encrypted assets
│   │   │   │   └── burriiiii/              # Per-ABI encrypted libraries
│   │   │   │       ├── arm/
│   │   │   │       ├── arm64/
│   │   │   │       ├── x86/
│   │   │   │       └── x86_64/
│   │   │   └── AndroidManifest.xml
│   └── build.gradle
├── build.gradle
├── settings.gradle
├── gradle.properties
├── .gitignore
└── README.md
```

## Architecture

### Loader Architecture

GameBlaster Pro uses a sophisticated multi-stage loader architecture:

1. **ProxyApplication** - Entry point that intercepts application initialization
2. **ProxyComponentFactory** - Intercepts component creation (Activities, Services, etc.)
3. **JniBridge** - Java-Native bridge for core functionality
4. **lib2f8c0b3257fcc345.so** - Encrypted native loader (Burri encryption)
5. **Actual Application** - Real application code loaded dynamically at runtime

### Native Libraries

#### libclient.so (4.6MB)
The main client library providing:
- **RenderEngine**: OpenGL ES 3.0 rendering for game overlay
- **GameDetector**: Detects and identifies target games (PUBG Mobile, Free Fire, etc.)
- **PluginManager**: Handles plugin updates and licensing
- **NetworkClient**: Network communication with update servers

Key JNI methods:
- `FloaterService.connect/disconnect` - Network operations
- `FloaterService.initSurface/removeSurface` - Rendering setup
- `FloaterService.drawOn/drawTick` - Rendering operations
- `FloaterService.findGame` - Game detection
- `SuperJNI.Companion.check/licence` - License verification
- `SuperJNI.Companion.update` - Plugin updates

#### lib2f8c0b3257fcc345.so
Burri encrypted loader extracted from `assets/burriiiii/<abi>/` at runtime:
- Bridges proxy loader to real application
- Manages ClassLoader injection
- Handles component factory registration
- Decrypts and loads additional DEX files

Key JNI methods:
- `JniBridge.craa` - Initialize real application
- `JniBridge.craoc` - Call onCreate on real application
- `JniBridge.cbde` - Set ClassLoader
- `JniBridge.ra` - Return real application instance
- `JniBridge.gap/gdp/rapn/rcf` - Get application/package information

#### libe6bmfqax5v.so (846KB)
Auxiliary cryptographic library:
- AES-256-CBC encryption/decryption
- MD5 and SHA256 hashing
- Zlib compression/decompression
- Asset decryption (from burriEnc)
- String obfuscation

#### libmsaoaidsec.so (670KB)
Security and device identification library (based on Alibaba OAID SDK):
- **DeviceInfo**: Collects device information (Android ID, OAID, IMEI, etc.)
- **AntiTamper**: Detects debuggers, root, emulators, Xposed, Frida
- **SignatureVerifier**: Verifies app signature integrity
- **OAID Support**: Xiaomi, Huawei, Samsung, and other manufacturer OAID providers

### Target Games

The application is designed to work with:
- PUBG Mobile (com.tencent.ig)
- PUBG Mobile Global (com.pubg.imobile)
- PUBG Mobile Korea (com.pubg.krmobile)
- PUBG Mobile Vietnam (com.vng.pubgmobile)
- PUBG Mobile Taiwan (com.rekoo.pubgm)
- Free Fire and Free Fire MAX

## Encryption Scheme

### Burri Encryption

The "Burri" encryption scheme is used throughout the application:

1. **Asset Encryption**: Files in `assets/burriEnc/` are encrypted with AES-256-CBC
2. **Library Encryption**: Native libraries in `assets/burriiiii/` use the same scheme
3. **CRC Verification**: All encrypted files have CRC checksums for integrity
4. **Runtime Decryption**: Decryption occurs at runtime in native code

Encryption keys (found in libe6bmfqax5v.so):
```c
Key: "Burri Burri Encryption Key v3.1"
IV:  "GameBlasterPro\0\0"
```

## Permissions

The application requests 119 Android permissions, including:

**Critical Permissions:**
- `INTERNET` - Network communication
- `ACCESS_NETWORK_STATE` - Network status
- `MANAGE_EXTERNAL_STORAGE` - File system access
- `SYSTEM_ALERT_WINDOW` - Overlay windows
- `FOREGROUND_SERVICE` - Background services
- `POST_NOTIFICATIONS` - Notifications
- `CAMERA` - Camera access
- `RECORD_AUDIO` - Audio recording
- `READ_SMS` - SMS reading
- `CALL_PHONE` - Phone calls
- `ACCESS_FINE_LOCATION` - Location access
- `QUERY_ALL_PACKAGES` - Package queries

## Building the Project

### Prerequisites

- Android Studio Hedgehog | 2023.1.1 or later
- Android SDK 36
- NDK r26b or later
- CMake 3.18.1 or later
- Java 17
- Kotlin 1.9.22

### Build Steps

1. **Clone the repository:**
   ```bash
   git clone <repository-url>
   cd GameBlasterPro
   ```

2. **Open in Android Studio:**
   - Open Android Studio
   - Select "Open an existing project"
   - Navigate to the project directory

3. **Sync Gradle:**
   ```bash
   ./gradlew clean build
   ```

4. **Build Native Libraries:**
   ```bash
   ./gradlew app:externalNativeBuildDebug
   ```

5. **Build APK:**
   ```bash
   ./gradlew assembleDebug
   ```
   or
   ```bash
   ./gradlew assembleRelease
   ```

### Command Line Build

```bash
# Debug build
./gradlew assembleDebug

# Release build (requires signing configuration)
./gradlew assembleRelease

# Install on connected device
./gradlew installDebug

# Run tests
./gradlew test
```

## Decompilation Process

This source code was reconstructed using the following tools and methods:

### Java/Kotlin Decompilation
- **JADX 1.5.0** - DEX to Java decompilation
- **ApkTool 2.9.3** - APK resource extraction and AndroidManifest decoding

### Native Library Decompilation
- **Ghidra 11.2.1** - Native ARM64 binary analysis
- **IDA Pro** - Cross-reference analysis
- **readelf/nm** - Symbol extraction
- **strings** - String analysis
- Manual reconstruction based on JNI signatures and runtime behavior

### Process Steps

1. Extract APK with `unzip`
2. Decompile DEX with JADX
3. Extract resources with ApkTool
4. Analyze native libraries with Ghidra/IDA
5. Reconstruct C++ source from decompiled pseudocode
6. Create CMakeLists.txt for native build
7. Set up Gradle build configuration
8. Verify build against original APK behavior

## Dependencies

The project uses the following open-source libraries:

### AndroidX
- core-ktx 1.12.0
- appcompat 1.6.1
- material 1.11.0
- constraintlayout 2.1.4
- lifecycle 2.7.0
- room 2.6.1
- work 2.9.0

### Networking
- OkHttp 4.12.0
- Retrofit 2.9.0
- Gson 2.10.1

### Image Loading
- Glide 4.16.0

### Dependency Injection
- Hilt 2.50

### Coroutines
- kotlinx-coroutines 1.7.3

### Security
- security-crypto 1.1.0-alpha06

### Native Libraries
- OpenSSL (crypto/ssl)
- zlib (compression)
- OpenGL ES 3.0
- EGL

## Security Considerations

⚠️ **WARNING**: This application contains sophisticated anti-debugging and anti-tampering mechanisms:

- Debugger detection
- Root detection
- Emulator detection
- Xposed/EdXposed detection
- Frida detection
- Signature verification
- Memory integrity checks

**Use at your own risk.** This decompilation is for educational purposes only.

## Legal Notice

This decompilation is provided for:
- Educational purposes
- Security research
- Interoperability research
- Reverse engineering research

**NOT FOR:**
- Commercial use
- Redistribution
- Circumventing security measures
- Violating terms of service
- Creating derivative works for distribution

The original application and all intellectual property belong to their respective owners.

## Known Issues

1. Some obfuscated variable names remain (e.g., `a`, `b`, `c`)
2. Asset decryption keys are for reference only
3. Server URLs and endpoints are placeholders
4. License verification is stubbed out
5. Native decompilation may not be 100% accurate

## Contributing

As this is a decompilation for research purposes, contributions are limited to:
- Documentation improvements
- Build system fixes
- Analysis notes
- Security research findings

## License

This decompilation is provided AS-IS with no warranty. Original application copyright belongs to the original authors.

## Contact

For questions about this decompilation, please open an issue on GitHub.

---

**Last Updated:** January 31, 2026
**Decompilation Version:** 1.0.0
**Original APK Version:** 3.1 (Final)
