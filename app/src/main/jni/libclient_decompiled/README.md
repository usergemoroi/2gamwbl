# libclient Decompiled

**Version:** 3.1.0  
**Architecture:** ARM64-v8a (AArch64)  
**Language:** C++17  
**Build System:** CMake 3.18.1+  

---

## Overview

This directory contains the fully decompiled and reconstructed source code of `libclient.so` from GameBlaster Pro v3.1. The library implements core native functionality including:

- **OpenGL ES 3.0 Rendering** - Overlay rendering engine
- **Game Detection** - Process scanning and identification
- **Network Communication** - HTTPS-based server communication
- **Plugin Management** - Dynamic loading and updates
- **Security Features** - Anti-tampering and integrity checks
- **Device Profiling** - Hardware identification

---

## Project Structure

```
libclient_decompiled/
├── include/                   # Public headers
│   ├── client.h              # Main client interface
│   ├── types.h               # Data structures and types
│   ├── constants.h           # Constants and configuration
│   ├── jni_bridge.h          # JNI helper macros and utilities
│   ├── render_engine.h       # OpenGL rendering
│   ├── game_detector.h       # Game detection
│   ├── network_client.h      # Network operations
│   ├── plugin_manager.h      # Plugin management
│   ├── device_info.h         # Device information
│   ├── anti_tamper.h         # Security checks
│   └── internal/             # Internal headers
│       ├── crypto_utils.h    # Cryptographic utilities
│       ├── string_utils.h    # String manipulation
│       └── platform_specific.h # Platform APIs
│
├── src/                      # Implementation files
│   ├── jni_interface.cpp     # JNI entry points
│   ├── client.cpp            # Main client implementation
│   ├── render_engine.cpp     # OpenGL ES implementation
│   ├── game_detector.cpp     # Game detection logic
│   ├── network_client.cpp    # Network communication
│   ├── plugin_manager.cpp    # Plugin management
│   ├── device_info.cpp       # Device info collection
│   ├── anti_tamper.cpp       # Security implementations
│   ├── crypto_utils.cpp      # Cryptography
│   ├── string_utils.cpp      # String utilities
│   ├── platform_specific.cpp # Platform-specific code
│   ├── types.cpp             # Type implementations
│   └── init.cpp              # Initialization code
│
├── docs/                     # Documentation
│   ├── ANALYSIS_REPORT.md    # Complete analysis report
│   ├── JNI_METHODS.md        # JNI method reference
│   ├── DATA_STRUCTURES.md    # Data structure documentation
│   ├── FUNCTIONS_REFERENCE.md # Function reference
│   ├── CONTROL_FLOW.md       # Control flow diagrams
│   └── DECOMPILATION_NOTES.md # Decompilation notes
│
├── CMakeLists.txt            # Build configuration
└── README.md                 # This file
```

---

## Building

### Prerequisites

- Android NDK r21 or later
- CMake 3.18.1 or later
- C++17 compatible compiler

### Build Steps

#### Using CMake Directly

```bash
cd libclient_decompiled
mkdir build && cd build
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-26 \
  -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

#### Using Android Studio / Gradle

Add to app/build.gradle:

```gradle
android {
    externalNativeBuild {
        cmake {
            path "src/main/jni/libclient_decompiled/CMakeLists.txt"
            version "3.18.1"
        }
    }
    
    defaultConfig {
        ndk {
            abiFilters 'arm64-v8a'
        }
    }
}
```

---

## Usage

### Initialization

```cpp
#include "client.h"

using namespace client;

// Initialize client
Client& client = Client::getInstance();
if (!client.initialize()) {
    // Handle error
}
```

### Rendering

```cpp
// Initialize rendering surface
ANativeWindow* window = ANativeWindow_fromSurface(env, surface);
client.initializeRendering(window, 1920, 1080);

// Start rendering
client.startRendering();

// Stop rendering
client.stopRendering();
```

### Game Detection

```cpp
GameDetector* detector = client.getGameDetector();

// Find specific game
if (detector->findGame("com.tencent.ig")) {
    // Game found
}

// Auto-detection with callback
detector->setGameDetectedCallback([](const GameInfo& info) {
    // Game detected
});
detector->startAutoDetection();
```

### Network Operations

```cpp
NetworkClient* network = client.getNetworkClient();

// Connect to server
network->connect("https://api.example.com");

// Check license
if (network->checkLicense("LICENSE-KEY")) {
    // License valid
}

// Check for updates
UpdateInfo info = network->checkForUpdates();
if (info.is_available) {
    // Update available
}
```

### Plugin Management

```cpp
PluginManager* plugins = client.getPluginManager();

// Load plugin
plugins->loadPlugin("/path/to/plugin.so");

// Enable auto-update
plugins->enableAutoUpdate(true);
```

---

## JNI Integration

### Java Side

```java
public class FloaterService {
    static {
        System.loadLibrary("client");
    }
    
    public native void connect(String url);
    public native void disconnect();
    public native void initSurface(Surface surface, int width, int height);
    public native void removeSurface();
    public native void drawTick();
    public native boolean findGame(String packageName);
}
```

### Native Side

```cpp
extern "C" {

JNIEXPORT void JNICALL
Java_com_eternal_xdsdk_FloaterService_connect(
    JNIEnv* env, jobject thiz, jstring url) {
    // Implementation
}

} // extern "C"
```

---

## Dependencies

### External Libraries

- **libEGL.so** - EGL display management
- **libGLESv3.so** - OpenGL ES 3.0
- **libz.so** - zlib compression
- **libandroid.so** - Android NDK APIs
- **liblog.so** - Android logging
- **libc.so** - C standard library
- **libm.so** - Math library
- **libdl.so** - Dynamic linking

### Optional Dependencies

- **OpenSSL** - For full crypto implementation
- **libcurl** - For advanced networking

---

## API Documentation

### Core Classes

#### Client
Main singleton class coordinating all components.

**Methods:**
- `getInstance()` - Get singleton instance
- `initialize()` - Initialize all components
- `shutdown()` - Cleanup resources
- `getRenderEngine()` - Access rendering engine
- `getGameDetector()` - Access game detector
- `getNetworkClient()` - Access network client

#### RenderEngine
OpenGL ES 3.0 rendering engine.

**Methods:**
- `initialize()` - Initialize OpenGL
- `createSurface()` - Create rendering surface
- `startRendering()` - Start render loop
- `stopRendering()` - Stop render loop
- `setOpacity()` - Set overlay opacity

#### GameDetector
Process scanner and game detector.

**Methods:**
- `findGame()` - Find specific game
- `isGameRunning()` - Check if game running
- `startAutoDetection()` - Enable auto-detection
- `addTargetGame()` - Add game to watch list

#### NetworkClient
HTTPS communication client.

**Methods:**
- `connect()` - Connect to server
- `checkLicense()` - Validate license
- `checkForUpdates()` - Check for updates
- `downloadFile()` - Download file

See [JNI_METHODS.md](docs/JNI_METHODS.md) for complete JNI reference.

---

## Configuration

### Constants

Edit `include/constants.h` to configure:

```cpp
namespace constants {
    constexpr const char* DEFAULT_SERVER_URL = "https://your-server.com";
    constexpr int DEFAULT_FPS_TARGET = 60;
    constexpr float DEFAULT_OPACITY = 0.8f;
}
```

### Runtime Configuration

```cpp
Client& client = Client::getInstance();

// Set paths
client.setConfigPath("/data/data/com.example/config");
client.setCachePath("/data/data/com.example/cache");

// Configure network
NetworkConfig config;
config.server_url = "https://api.example.com";
config.timeout_ms = 30000;
client.getNetworkClient()->initialize(config);
```

---

## Debugging

### Enable Logging

```cpp
// In constants.h
constexpr bool ENABLE_DEBUG_LOGS = true;
constexpr bool ENABLE_VERBOSE_LOGS = true;
```

### View Logs

```bash
adb logcat -s libclient:* RenderEngine:* GameDetector:* NetworkClient:*
```

---

## Testing

### Unit Tests
(To be implemented)

### Integration Tests
```bash
# Build test app
./gradlew assembleDebug

# Install
adb install app-debug.apk

# Run
adb shell am start -n com.eternal.xdsdk/.MainActivity
```

---

## Known Issues

1. **Stub Implementations:**
   - Some cryptography functions are stubs
   - Network protocol partially implemented
   - Plugin validation simplified

2. **Performance:**
   - Not fully optimized
   - Some allocations in hot paths
   - Thread pool not implemented

3. **Compatibility:**
   - Tested only on ARM64
   - Android 8.0+ required
   - Some devices may have issues

---

## Contributing

This is a decompiled/reconstructed project for educational purposes.

### Code Style
- Follow Google C++ Style Guide
- Use clang-format for formatting
- Add Doxygen comments for public APIs

### Pull Requests
- Ensure code compiles without warnings
- Add tests for new features
- Update documentation

---

## License

**Educational/Research Use Only**

This decompiled code is provided for:
- Security research
- Interoperability research
- Educational purposes
- Understanding mobile application architecture

**NOT for:**
- Commercial use
- Game cheating
- Circumventing security measures
- Violating terms of service

---

## Disclaimer

This code was reverse engineered from compiled binaries for educational and research purposes. The original software is copyrighted by its respective owners. This reconstruction is not affiliated with, endorsed by, or connected to the original developers.

Use at your own risk. The authors are not responsible for any misuse of this code.

---

## References

- [Android NDK Documentation](https://developer.android.com/ndk)
- [OpenGL ES 3.0 Specification](https://www.khronos.org/opengles/)
- [JNI Specification](https://docs.oracle.com/javase/8/docs/technotes/guides/jni/)
- [EGL API](https://www.khronos.org/egl)

---

## Contact

For questions about the decompilation process or technical details, see documentation in the `docs/` directory.

---

**Version:** 1.0.0  
**Last Updated:** January 31, 2026  
**Decompiled By:** AI Reverse Engineering System
