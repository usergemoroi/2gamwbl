# libmsaoaidsec API Documentation

## Table of Contents

1. [Core Functions](#core-functions)
2. [Device Identification](#device-identification)
3. [Security Checks](#security-checks)
4. [OAID Interface](#oaid-interface)
5. [JNI Methods](#jni-methods)
6. [Data Structures](#data-structures)
7. [Error Codes](#error-codes)
8. [Usage Examples](#usage-examples)

## Core Functions

### Library Initialization

#### msaoaidsec_init

```c
int msaoaidsec_init(JNIEnv* env, jobject context);
```

**Description**: Initializes the libmsaoaidsec library.

**Parameters**:
- `env` - JNI environment pointer
- `context` - Android Context object (Activity or Application)

**Returns**:
- `ERROR_NONE` (0) on success
- `ERROR_INVALID_ARGUMENT` (-4) if env is NULL
- `ERROR_OUT_OF_MEMORY` (-5) if allocation fails
- `ERROR_INIT_FAILED` (-1) if initialization fails

**Example**:
```cpp
JNIEnv* env = ...;
jobject context = ...;

int result = msaoaidsec_init(env, context);
if (result == ERROR_NONE) {
    // Library initialized successfully
}
```

#### msaoaidsec_cleanup

```c
void msaoaidsec_cleanup();
```

**Description**: Cleans up library resources and global state.

**Example**:
```cpp
msaoaidsec_cleanup();
```

#### msaoaidsec_is_initialized

```c
bool msaoaidsec_is_initialized();
```

**Description**: Checks if the library is initialized.

**Returns**: `true` if initialized, `false` otherwise.

#### msaoaidsec_get_version

```c
const char* msaoaidsec_get_version();
```

**Description**: Returns the library version string.

**Returns**: Version string (e.g., "1.0.0").

## Device Identification

### get_device_id

```c
char* get_device_id(JNIEnv* env, jobject context);
```

**Description**: Retrieves the primary device identifier. Tries Android ID first, then OAID, then returns "unknown".

**Parameters**:
- `env` - JNI environment pointer
- `context` - Android Context object

**Returns**: Device ID string (must be freed by caller), or NULL on error.

**Example**:
```cpp
char* device_id = get_device_id(env, context);
if (device_id) {
    printf("Device ID: %s\n", device_id);
    free(device_id);
}
```

### get_android_id

```c
char* get_android_id(JNIEnv* env, jobject context);
```

**Description**: Gets the Android Secure Settings ID.

**Returns**: Android ID (64-bit hex string), or NULL on error.

**Note**: Returns NULL if value is the default "9774d56d682e549c".

### get_oaid

```c
char* get_oaid(JNIEnv* env, jobject context);
```

**Description**: Gets the Open Anonymous Device Identifier (OAID).

**Returns**: OAID string, or NULL if not available.

**Note**: Automatically detects manufacturer and uses appropriate provider.

### get_imei

```c
char* get_imei(JNIEnv* env, jobject context);
```

**Description**: Gets the IMEI (International Mobile Equipment Identity).

**Returns**: IMEI string, or NULL if not available.

**Note**: Requires `READ_PHONE_STATE` permission.

### get_serial_number

```c
char* get_serial_number(JNIEnv* env, jobject context);
```

**Description**: Gets the device serial number.

**Returns**: Serial number string, or NULL on error.

### Build Information Functions

```c
char* get_manufacturer();  // e.g., "Xiaomi", "HUAWEI"
char* get_model();         // e.g., "Mi 11", "Mate 40"
char* get_brand();         // e.g., "xiaomi", "huawei"
char* get_hardware();      // e.g., "qcom", "kirin"
char* get_device();        // e.g., "venus", "charlotte"
char* get_product();       // e.g., "venus_global", "charlotte"
char* get_fingerprint();   // Build fingerprint
char* get_build_id();      // Build ID
int32_t get_sdk_version(); // SDK version (e.g., 30 for Android 11)
int32_t get_api_level();   // Same as SDK version
```

**Note**: All string functions return dynamically allocated strings that must be freed.

### get_full_device_info

```c
DeviceInfo* get_full_device_info(JNIEnv* env, jobject context);
```

**Description**: Collects all device information in one call.

**Returns**: Pointer to DeviceInfo structure, or NULL on error.

**Example**:
```cpp
DeviceInfo* info = get_full_device_info(env, context);
if (info) {
    printf("Manufacturer: %s\n", info->manufacturer);
    printf("Model: %s\n", info->model);
    printf("Android ID: %s\n", info->android_id);
    printf("OAID: %s\n", info->oaid);
    printf("SDK Version: %d\n", info->sdk_version);
    printf("Rooted: %s\n", info->is_rooted ? "Yes" : "No");
    
    free_device_info(info);
}
```

## Security Checks

### detect_debugger

```c
bool detect_debugger();
```

**Description**: Detects if a debugger is attached to the process.

**Returns**: `true` if debugger detected, `false` otherwise.

**Detection Methods**:
1. TracerPid check via `/proc/self/status`
2. ptrace() self-attach attempt
3. Timing-based detection

### detect_root

```c
bool detect_root();
```

**Description**: Detects if the device is rooted.

**Returns**: `true` if root detected, `false` otherwise.

**Detection Methods**:
1. su binary existence check (10+ paths)
2. Superuser APK detection
3. Build tags check (test-keys)
4. System property validation (ro.secure, ro.debuggable)

### detect_emulator

```c
bool detect_emulator();
```

**Description**: Detects if running in an emulator/simulator.

**Returns**: `true` if emulator detected, `false` otherwise.

**Detection Methods**:
1. QEMU/Goldfish device files
2. System properties (ro.kernel.qemu, ro.hardware)
3. Build information (manufacturer, model, brand)
4. CPU info analysis
5. Hardware platform check

### detect_xposed

```c
bool detect_xposed();
```

**Description**: Detects if Xposed Framework is installed.

**Returns**: `true` if Xposed detected, `false` otherwise.

**Detection Methods**:
1. Memory map scanning for XposedBridge
2. Class loading attempts
3. Stack trace analysis

### detect_frida

```c
bool detect_frida();
```

**Description**: Detects if Frida instrumentation framework is present.

**Returns**: `true` if Frida detected, `false` otherwise.

**Detection Methods**:
1. Library name scanning in /proc/self/maps
2. Thread name detection (gdbus, gum-js-loop, etc.)
3. Default port detection (27047)
4. Socket file checks

### verify_apk_signature

```c
bool verify_apk_signature(JNIEnv* env, jobject context);
```

**Description**: Verifies APK signature integrity.

**Returns**: `true` if signature is valid, `false` otherwise.

**Note**: Simplified implementation in decompiled version.

### perform_security_check

```c
SecurityCheckResult* perform_security_check(JNIEnv* env, jobject context, int32_t checks);
```

**Description**: Performs selected security checks.

**Parameters**:
- `env` - JNI environment pointer
- `context` - Android Context object
- `checks` - Bitfield of SecurityCheck flags

**Returns**: Pointer to SecurityCheckResult, or NULL on error.

**Check Flags**:
```c
SECURITY_CHECK_DEBUGGER  = 0x01
SECURITY_CHECK_ROOT      = 0x02
SECURITY_CHECK_EMULATOR  = 0x04
SECURITY_CHECK_XPOSED    = 0x08
SECURITY_CHECK_FRIDA     = 0x10
SECURITY_CHECK_SIGNATURE = 0x20
SECURITY_CHECK_MEMORY    = 0x40
SECURITY_CHECK_ALL       = 0xFF
```

**Example**:
```cpp
int32_t checks = SECURITY_CHECK_DEBUGGER | SECURITY_CHECK_ROOT | SECURITY_CHECK_FRIDA;
SecurityCheckResult* result = perform_security_check(env, context, checks);
if (result) {
    if (result->debugger_detected) {
        // Handle debugger
    }
    if (result->root_detected) {
        // Handle root
    }
    if (result->frida_detected) {
        // Handle Frida
    }
    
    int32_t security_level = get_security_level(result);
    printf("Security Level: %d/100\n", security_level);
    
    free_security_check_result(result);
}
```

### is_running_in_secure_environment

```c
bool is_running_in_secure_environment(JNIEnv* env, jobject context);
```

**Description**: Performs all security checks and returns overall status.

**Returns**: `true` if environment is secure, `false` otherwise.

**Example**:
```cpp
if (!is_running_in_secure_environment(env, context)) {
    // Take action - exit, warn user, etc.
    fprintf(stderr, "Insecure environment detected!\n");
    exit(1);
}
```

## OAID Interface

### get_oaid_provider

```c
OAIDProvider get_oaid_provider();
```

**Description**: Detects the OAID provider for the current device.

**Returns**: OAIDProvider enum value.

**Possible Values**:
```c
OAID_PROVIDER_XIAOMI   = 1
OAID_PROVIDER_HUAWEI   = 2
OAID_PROVIDER_SAMSUNG  = 3
OAID_PROVIDER_OPPO     = 4
OAID_PROVIDER_VIVO     = 5
OAID_PROVIDER_ONEPLUS  = 6
OAID_PROVIDER_MEIZU    = 7
OAID_PROVIDER_ASUS     = 8
OAID_PROVIDER_LENOVO   = 9
OAID_PROVIDER_GOOGLE   = 10
OAID_PROVIDER_UNKNOWN  = 0
```

### get_oaid_provider_name

```c
const char* get_oaid_provider_name(OAIDProvider provider);
```

**Description**: Gets the human-readable name for an OAID provider.

**Returns**: Provider name string (e.g., "Xiaomi", "Huawei").

### get_oaid_info

```c
OAIDInfo* get_oaid_info(JNIEnv* env, jobject context);
```

**Description**: Gets complete OAID information.

**Returns**: Pointer to OAIDInfo structure, or NULL on error.

**Example**:
```cpp
OAIDInfo* oaid_info = get_oaid_info(env, context);
if (oaid_info) {
    if (oaid_info->is_available) {
        printf("OAID: %s\n", oaid_info->oaid);
        printf("Provider: %s\n", oaid_info->provider_name);
        printf("Limit Tracking: %s\n", oaid_info->is_limit_tracking ? "Yes" : "No");
    } else {
        printf("OAID not available: %s\n", oaid_info->error_message);
    }
    
    free_oaid_info(oaid_info);
}
```

### Provider-Specific Functions

```c
char* get_xiaomi_oaid(JNIEnv* env, jobject context);
char* get_huawei_oaid(JNIEnv* env, jobject context);
char* get_samsung_oaid(JNIEnv* env, jobject context);
char* get_oppo_oaid(JNIEnv* env, jobject context);
char* get_vivo_oaid(JNIEnv* env, jobject context);
char* get_oneplus_oaid(JNIEnv* env, jobject context);
char* get_meizu_oaid(JNIEnv* env, jobject context);
char* get_asus_oaid(JNIEnv* env, jobject context);
char* get_lenovo_oaid(JNIEnv* env, jobject context);
char* get_google_advertising_id(JNIEnv* env, jobject context);
```

**Description**: Get OAID from specific provider.

**Returns**: OAID string, or NULL if not available.

### is_oaid_supported

```c
bool is_oaid_supported();
```

**Description**: Checks if OAID is supported on this device.

**Returns**: `true` if supported, `false` otherwise.

### is_oaid_available

```c
bool is_oaid_available(JNIEnv* env, jobject context);
```

**Description**: Checks if OAID is actually available (provider installed and accessible).

**Returns**: `true` if available, `false` otherwise.

## JNI Methods

All JNI methods follow the pattern:
```c
JNIEXPORT <returnType> JNICALL 
Java_com_mobile_security_oaid_<ClassName>_<methodName>(JNIEnv* env, jobject thiz, ...);
```

### DeviceInfo Methods

```java
package com.mobile.security.oaid;

public class DeviceInfo {
    static { System.loadLibrary("msaoaidsec"); }
    
    public native String getDeviceId(Context context);
    public native String getAndroidId(Context context);
    public native String getOAID(Context context);
    public native String getIMEI(Context context);
    public native String getSerial(Context context);
    public native String getManufacturer();
    public native String getModel();
    public native String getBrand();
    public native String getHardware();
    public native String getDevice();
    public native String getFingerprint();
    public native int getSDKVersion();
}
```

### SecurityCheck Methods

```java
package com.mobile.security.oaid;

public class SecurityCheck {
    static { System.loadLibrary("msaoaidsec"); }
    
    public native boolean checkDebugger();
    public native boolean checkRoot();
    public native boolean checkEmulator();
    public native boolean checkXposed();
    public native boolean checkFrida();
    public native boolean verifySignature(Context context);
    public native int performAllChecks(Context context); // Returns 0-100
}
```

### OAIDHelper Methods

```java
package com.mobile.security.oaid;

public class OAIDHelper {
    static { System.loadLibrary("msaoaidsec"); }
    
    public native String getOAID(Context context);
    public native int getOAIDProvider();
    public native boolean isOAIDSupported();
    public native boolean isOAIDAvailable(Context context);
    public native String getProviderName(int provider);
}
```

### Library Methods

```java
package com.mobile.security.oaid;

public class Library {
    static { System.loadLibrary("msaoaidsec"); }
    
    public native int init(Context context);
    public native void cleanup();
    public native String getVersion();
    public native boolean isInitialized();
}
```

## Data Structures

### DeviceInfo

```c
typedef struct DeviceInfo {
    char* device_id;
    char* android_id;
    char* oaid;
    char* imei;
    char* serial_number;
    char* manufacturer;
    char* model;
    char* brand;
    char* hardware;
    char* build_id;
    char* fingerprint;
    int32_t sdk_version;
    int32_t api_level;
    bool is_rooted;
    bool is_emulator;
    bool is_debuggable;
} DeviceInfo;
```

**Memory Management**:
```c
DeviceInfo* info = create_device_info();
// Use info...
free_device_info(info);
```

### SecurityCheckResult

```c
typedef struct SecurityCheckResult {
    bool debugger_detected;
    bool root_detected;
    bool emulator_detected;
    bool xposed_detected;
    bool frida_detected;
    bool signature_valid;
    bool memory_tampered;
    int32_t check_count;
    uint64_t timestamp;
} SecurityCheckResult;
```

**Memory Management**:
```c
SecurityCheckResult* result = create_security_check_result();
// Use result...
free_security_check_result(result);
```

### OAIDInfo

```c
typedef struct OAIDInfo {
    char* oaid;
    char* provider_name;
    bool is_available;
    bool is_limit_tracking;
    int32_t error_code;
    char* error_message;
} OAIDInfo;
```

**Memory Management**:
```c
OAIDInfo* info = create_oaid_info();
// Use info...
free_oaid_info(info);
```

## Error Codes

```c
typedef enum ErrorCode {
    ERROR_NONE = 0,
    ERROR_INIT_FAILED = -1,
    ERROR_NOT_SUPPORTED = -2,
    ERROR_PERMISSION_DENIED = -3,
    ERROR_INVALID_ARGUMENT = -4,
    ERROR_OUT_OF_MEMORY = -5,
    ERROR_OAID_NOT_AVAILABLE = -6,
    ERROR_JNI_EXCEPTION = -7,
    ERROR_UNKNOWN = -99
} ErrorCode;
```

## Usage Examples

### Complete C++ Example

```cpp
#include "msaoaidsec/msaoaidsec.h"

void example_usage(JNIEnv* env, jobject context) {
    // Initialize library
    if (msaoaidsec_init(env, context) != ERROR_NONE) {
        fprintf(stderr, "Failed to initialize library\n");
        return;
    }
    
    // Get device information
    DeviceInfo* device_info = get_full_device_info(env, context);
    if (device_info) {
        printf("=== Device Information ===\n");
        printf("Device ID: %s\n", device_info->device_id);
        printf("Android ID: %s\n", device_info->android_id);
        printf("OAID: %s\n", device_info->oaid);
        printf("Manufacturer: %s\n", device_info->manufacturer);
        printf("Model: %s\n", device_info->model);
        printf("SDK Version: %d\n", device_info->sdk_version);
        printf("\n");
        
        free_device_info(device_info);
    }
    
    // Perform security checks
    SecurityCheckResult* security = perform_full_security_check(env, context);
    if (security) {
        printf("=== Security Status ===\n");
        printf("Debugger: %s\n", security->debugger_detected ? "DETECTED" : "OK");
        printf("Root: %s\n", security->root_detected ? "DETECTED" : "OK");
        printf("Emulator: %s\n", security->emulator_detected ? "DETECTED" : "OK");
        printf("Xposed: %s\n", security->xposed_detected ? "DETECTED" : "OK");
        printf("Frida: %s\n", security->frida_detected ? "DETECTED" : "OK");
        printf("Signature: %s\n", security->signature_valid ? "VALID" : "INVALID");
        
        int32_t level = get_security_level(security);
        printf("Security Level: %d/100\n", level);
        printf("\n");
        
        free_security_check_result(security);
    }
    
    // Get OAID information
    OAIDInfo* oaid_info = get_oaid_info(env, context);
    if (oaid_info) {
        printf("=== OAID Information ===\n");
        if (oaid_info->is_available) {
            printf("OAID: %s\n", oaid_info->oaid);
            printf("Provider: %s\n", oaid_info->provider_name);
            printf("Limit Tracking: %s\n", oaid_info->is_limit_tracking ? "Yes" : "No");
        } else {
            printf("OAID not available: %s (code: %d)\n", 
                   oaid_info->error_message, oaid_info->error_code);
        }
        printf("\n");
        
        free_oaid_info(oaid_info);
    }
    
    // Cleanup
    msaoaidsec_cleanup();
}
```

### Java Example

```java
package com.example.app;

import android.content.Context;
import android.util.Log;
import com.mobile.security.oaid.*;

public class SecurityManager {
    private static final String TAG = "SecurityManager";
    
    private DeviceInfo deviceInfo;
    private SecurityCheck securityCheck;
    private OAIDHelper oaidHelper;
    private Library library;
    
    public SecurityManager() {
        deviceInfo = new DeviceInfo();
        securityCheck = new SecurityCheck();
        oaidHelper = new OAIDHelper();
        library = new Library();
    }
    
    public boolean initialize(Context context) {
        int result = library.init(context);
        if (result != 0) {
            Log.e(TAG, "Failed to initialize library: " + result);
            return false;
        }
        
        String version = library.getVersion();
        Log.i(TAG, "Library initialized: " + version);
        return true;
    }
    
    public void collectDeviceInfo(Context context) {
        String deviceId = deviceInfo.getDeviceId(context);
        String androidId = deviceInfo.getAndroidId(context);
        String oaid = deviceInfo.getOAID(context);
        String manufacturer = deviceInfo.getManufacturer();
        String model = deviceInfo.getModel();
        int sdkVersion = deviceInfo.getSDKVersion();
        
        Log.i(TAG, "Device ID: " + deviceId);
        Log.i(TAG, "Android ID: " + androidId);
        Log.i(TAG, "OAID: " + oaid);
        Log.i(TAG, "Manufacturer: " + manufacturer);
        Log.i(TAG, "Model: " + model);
        Log.i(TAG, "SDK Version: " + sdkVersion);
    }
    
    public boolean performSecurityChecks(Context context) {
        boolean isDebugger = securityCheck.checkDebugger();
        boolean isRoot = securityCheck.checkRoot();
        boolean isEmulator = securityCheck.checkEmulator();
        boolean isXposed = securityCheck.checkXposed();
        boolean isFrida = securityCheck.checkFrida();
        
        if (isDebugger) {
            Log.w(TAG, "Debugger detected!");
            return false;
        }
        
        if (isRoot) {
            Log.w(TAG, "Root detected!");
            return false;
        }
        
        if (isEmulator) {
            Log.w(TAG, "Emulator detected!");
            return false;
        }
        
        if (isXposed) {
            Log.w(TAG, "Xposed detected!");
            return false;
        }
        
        if (isFrida) {
            Log.w(TAG, "Frida detected!");
            return false;
        }
        
        int securityLevel = securityCheck.performAllChecks(context);
        Log.i(TAG, "Security level: " + securityLevel + "/100");
        
        return securityLevel >= 80; // Accept if >= 80%
    }
    
    public void cleanup() {
        library.cleanup();
    }
}
```

## Thread Safety

⚠️ **Warning**: The library is NOT fully thread-safe in its current implementation. Global state variables may have race conditions under concurrent access.

**Recommendations**:
1. Initialize the library once from the main thread
2. Serialize access to security check functions
3. Use separate instances for device info collection in different threads
4. Call cleanup only when all operations are complete

## Performance Considerations

**Function Timing** (approximate, ARM64 @ 2.0GHz):
- `get_android_id()`: 2-5ms
- `get_oaid()`: 50-200ms (depends on provider)
- `detect_debugger()`: 5-15ms
- `detect_root()`: 10-20ms
- `detect_emulator()`: 5-10ms
- `detect_frida()`: 10-30ms
- `perform_full_security_check()`: 30-80ms

**Recommendations**:
1. Cache DeviceInfo results (use `cache_device_info()`)
2. Perform security checks in background thread
3. Avoid calling OAID functions repeatedly
4. Use selective security checks instead of full checks when possible

---

**Version**: 1.0.0  
**Last Updated**: 2024-01-31  
**API Level**: 1
