# Build Instructions - GameBlaster Pro v3.1

## Quick Start

This document provides step-by-step instructions to build the decompiled GameBlaster Pro source code.

## Prerequisites

### Required Software

1. **Android Studio Hedgehog (2023.1.1) or later**
   - Download from: https://developer.android.com/studio
   - Includes Android SDK and basic build tools

2. **Java Development Kit (JDK) 17**
   - Bundled with Android Studio
   - Or install separately from: https://adoptium.net/

3. **Android SDK**
   - API Level 36 (Android 14)
   - Install via Android Studio SDK Manager

4. **Android NDK r26b or later**
   - Required for native library compilation
   - Install via Android Studio SDK Manager
   - Path: Tools → SDK Manager → SDK Tools → NDK

5. **CMake 3.18.1 or later**
   - Required for native library build system
   - Install via Android Studio SDK Manager
   - Path: Tools → SDK Manager → SDK Tools → CMake

### System Requirements

- **OS:** Windows 10/11, macOS 10.14+, or Linux (Ubuntu 18.04+)
- **RAM:** Minimum 8GB (16GB recommended)
- **Storage:** 10GB free space
- **CPU:** 64-bit processor

## Installation Steps

### Option 1: Build with Android Studio (Recommended)

1. **Open Android Studio**
   ```
   Launch Android Studio
   ```

2. **Import Project**
   ```
   File → Open → Navigate to project directory → Open
   ```

3. **Install Required SDKs**
   - Android Studio will prompt to install missing components
   - Accept all installation prompts
   - Wait for SDK downloads to complete

4. **Sync Gradle**
   ```
   File → Sync Project with Gradle Files
   ```
   - First sync may take 5-10 minutes
   - Downloads dependencies automatically

5. **Build Native Libraries**
   ```
   Build → Make Project
   ```
   - Compiles C++ code using NDK
   - Generates .so files for all ABIs

6. **Build APK**
   ```
   Build → Build Bundle(s) / APK(s) → Build APK(s)
   ```
   - Debug APK: `app/build/outputs/apk/debug/app-debug.apk`
   - Release APK: Requires signing configuration

### Option 2: Command Line Build

1. **Set ANDROID_HOME**
   ```bash
   # Linux/macOS
   export ANDROID_HOME=$HOME/Android/Sdk
   export PATH=$PATH:$ANDROID_HOME/platform-tools
   
   # Windows
   set ANDROID_HOME=C:\Users\%USERNAME%\AppData\Local\Android\Sdk
   set PATH=%PATH%;%ANDROID_HOME%\platform-tools
   ```

2. **Navigate to Project**
   ```bash
   cd /path/to/GameBlasterPro
   ```

3. **Build Debug APK**
   ```bash
   ./gradlew assembleDebug
   ```
   Output: `app/build/outputs/apk/debug/app-debug.apk`

4. **Build Release APK**
   ```bash
   ./gradlew assembleRelease
   ```
   Note: Requires signing configuration (see below)

5. **Install on Device**
   ```bash
   # Connect device via USB with debugging enabled
   ./gradlew installDebug
   ```

6. **Run Tests**
   ```bash
   ./gradlew test
   ```

## Build Variants

### Debug Build

- **Features:**
  - No obfuscation
  - Debug symbols included
  - Larger APK size (~20MB)
  - Can be installed alongside release version

- **Command:**
  ```bash
  ./gradlew assembleDebug
  ```

### Release Build

- **Features:**
  - ProGuard obfuscation enabled
  - Resources shrunk
  - Smaller APK size (~12MB)
  - Requires signing configuration

- **Command:**
  ```bash
  ./gradlew assembleRelease
  ```

## APK Signing (Required for Release)

### Generate Keystore

```bash
keytool -genkey -v -keystore gameblaster.keystore \
  -alias gameblaster -keyalg RSA -keysize 2048 \
  -validity 10000
```

### Configure Signing in app/build.gradle

Add to `android` block:

```gradle
signingConfigs {
    release {
        storeFile file("path/to/gameblaster.keystore")
        storePassword "your_store_password"
        keyAlias "gameblaster"
        keyPassword "your_key_password"
    }
}

buildTypes {
    release {
        signingConfig signingConfigs.release
        // ... rest of release configuration
    }
}
```

### Build Signed APK

```bash
./gradlew assembleRelease
```

Output: `app/build/outputs/apk/release/app-release.apk`

## Native Library Build

The project includes 4 native libraries that are compiled automatically:

1. **libclient.so** (4.6MB) - Main client library
2. **lib2f8c0b3257fcc345.so** (2.1MB) - Burri loader
3. **libe6bmfqax5v.so** (846KB) - Auxiliary crypto library
4. **libmsaoaidsec.so** (670KB) - Security library

### Manual Native Build

```bash
# Navigate to JNI directory
cd app/src/main/jni

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
make -j4

# Libraries output to: app/build/intermediates/cmake/
```

### Supported ABIs

- arm64-v8a (primary)
- armeabi-v7a
- x86
- x86_64

To build specific ABI only, modify `app/build.gradle`:

```gradle
defaultConfig {
    ndk {
        abiFilters 'arm64-v8a'  // Build only arm64
    }
}
```

## Troubleshooting

### Common Issues

#### 1. "SDK location not found"

**Solution:**
Create `local.properties` in project root:
```properties
sdk.dir=/path/to/Android/Sdk
ndk.dir=/path/to/Android/Sdk/ndk/26.1.10909125
```

#### 2. "CMake not found"

**Solution:**
Install CMake via Android Studio:
```
Tools → SDK Manager → SDK Tools → CMake (check box) → Apply
```

#### 3. "NDK not configured"

**Solution:**
Install NDK via Android Studio:
```
Tools → SDK Manager → SDK Tools → NDK (Side by side) → Apply
```

#### 4. "Unable to find vcruntime140_1.dll" (Windows)

**Solution:**
Install Visual C++ Redistributable:
https://aka.ms/vs/17/release/vc_redist.x64.exe

#### 5. "Execution failed for task ':app:externalNativeBuildDebug'"

**Solution:**
```bash
# Clean native build
./gradlew clean
rm -rf app/.cxx/
rm -rf app/build/

# Rebuild
./gradlew assembleDebug
```

#### 6. "Out of memory error" during build

**Solution:**
Increase Gradle memory in `gradle.properties`:
```properties
org.gradle.jvmargs=-Xmx8192m -Dfile.encoding=UTF-8
```

#### 7. "Unsupported class file major version 61"

**Solution:**
Ensure Java 17 is being used:
```bash
# Check Java version
java -version

# Should output: openjdk version "17.x.x"
```

## Verification

### Verify Build Success

After successful build, check outputs:

```bash
# Check APK exists
ls -lh app/build/outputs/apk/debug/app-debug.apk

# Check native libraries
ls -lh app/build/intermediates/merged_native_libs/debug/out/lib/arm64-v8a/

# Expected libraries:
# - libclient.so
# - lib2f8c0b3257fcc345.so
# - libe6bmfqax5v.so
# - libmsaoaidsec.so
```

### Compare with Original APK

```bash
# Extract both APKs
unzip -q app-debug.apk -d debug_extracted
unzip -q GameBlaster-Pro_3.1_Final.apk -d original_extracted

# Compare structure
diff -r debug_extracted original_extracted

# Compare native library sizes
ls -lh debug_extracted/lib/arm64-v8a/
ls -lh original_extracted/lib/arm64-v8a/
```

## Performance Optimization

### Release Build Optimizations

The release build automatically applies:

1. **ProGuard** - Code obfuscation and optimization
2. **Resource Shrinking** - Removes unused resources
3. **Native Code Optimization** - `-O3` compiler flag
4. **Symbol Stripping** - Removes debug symbols from .so files

### Custom Optimizations

To enable additional optimizations, modify `app/build.gradle`:

```gradle
buildTypes {
    release {
        minifyEnabled true
        shrinkResources true
        
        // Enable full R8 optimization
        proguardFiles getDefaultProguardFile('proguard-android-optimize.txt'), 
                      'proguard-rules.pro'
        
        // Native optimizations
        ndk {
            debugSymbolLevel 'SYMBOL_TABLE'  // Or 'FULL' for debugging
        }
        
        // Enable crash reporting
        android.defaultConfig.ndk.debugSymbolLevel = 'FULL'
    }
}
```

## Clean Build

To perform a completely clean build:

```bash
# Clean all build artifacts
./gradlew clean

# Remove additional build directories
rm -rf .gradle/
rm -rf app/.cxx/
rm -rf app/build/
rm -rf build/

# Remove Gradle cache (optional)
rm -rf ~/.gradle/caches/

# Rebuild
./gradlew assembleDebug
```

## Build Time Expectations

Typical build times on modern hardware:

- **First build:** 5-10 minutes (downloads dependencies)
- **Incremental build:** 30-60 seconds
- **Clean build:** 2-3 minutes
- **Native library build:** 1-2 minutes

## Advanced Topics

### Multi-APK Build (Split APKs)

To build separate APKs for each ABI:

```gradle
android {
    splits {
        abi {
            enable true
            reset()
            include 'arm64-v8a', 'armeabi-v7a', 'x86', 'x86_64'
            universalApk false
        }
    }
}
```

### App Bundle (AAB) for Play Store

```bash
./gradlew bundleRelease
```

Output: `app/build/outputs/bundle/release/app-release.aab`

### Continuous Integration

Example GitHub Actions workflow:

```yaml
name: Build APK
on: [push]
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - uses: actions/setup-java@v3
        with:
          java-version: '17'
      - run: ./gradlew assembleDebug
      - uses: actions/upload-artifact@v3
        with:
          name: app-debug.apk
          path: app/build/outputs/apk/debug/app-debug.apk
```

## Support

For build issues:

1. Check this document
2. Review `DECOMPILATION_NOTES.md`
3. Check Android Studio build output
4. Review Gradle logs: `./gradlew assembleDebug --info`

## Security Warning

⚠️ **Important:**

This is a decompiled application. Building and running it may:
- Violate terms of service of target games
- Trigger anti-cheat systems
- Pose security risks to your device

**Use only in isolated/sandboxed environments for research purposes.**

## License

This decompilation is provided AS-IS for educational and research purposes only.

---

**Last Updated:** January 31, 2026
**Build System Version:** Gradle 8.2.2, AGP 8.2.2
