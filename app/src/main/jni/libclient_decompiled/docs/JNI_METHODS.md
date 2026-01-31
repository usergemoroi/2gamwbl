# JNI Methods Reference

Complete reference for all JNI methods exported by libclient.so

---

## FloaterService Methods

### connect
**Signature:** `void connect(String connection_string)`  
**Native:** `Java_com_eternal_xdsdk_FloaterService_connect`

**Purpose:** Establish connection to remote server

**Parameters:**
- `connection_string`: Server URL (e.g., "https://api.example.com")

**Example:**
```java
FloaterService service = new FloaterService();
service.connect("https://gameblaster-api.com");
```

---

### disconnect
**Signature:** `void disconnect()`  
**Native:** `Java_com_eternal_xdsdk_FloaterService_disconnect`

**Purpose:** Terminate server connection and cleanup resources

---

### initSurface
**Signature:** `void initSurface(Surface surface, int width, int height)`  
**Native:** `Java_com_eternal_xdsdk_FloaterService_initSurface`

**Purpose:** Initialize OpenGL ES rendering surface

**Parameters:**
- `surface`: Android Surface object
- `width`: Surface width in pixels
- `height`: Surface height in pixels

**Example:**
```java
SurfaceTexture texture = new SurfaceTexture(0);
Surface surface = new Surface(texture);
service.initSurface(surface, 1920, 1080);
```

---

### removeSurface
**Signature:** `void removeSurface()`  
**Native:** `Java_com_eternal_xdsdk_FloaterService_removeSurface`

**Purpose:** Destroy rendering surface and release EGL context

---

### drawOn
**Signature:** `void drawOn(byte[] data, int width, int height)`  
**Native:** `Java_com_eternal_xdsdk_FloaterService_drawOn`

**Purpose:** Update texture and render frame with provided data

**Parameters:**
- `data`: RGBA8888 pixel data
- `width`: Texture width
- `height`: Texture height

---

### drawTick
**Signature:** `void drawTick()`  
**Native:** `Java_com_eternal_xdsdk_FloaterService_drawTick`

**Purpose:** Render single frame (main rendering method)

---

### findGame
**Signature:** `boolean findGame(String packageName)`  
**Native:** `Java_com_eternal_xdsdk_FloaterService_findGame`

**Purpose:** Detect if specified game is running

**Parameters:**
- `packageName`: Android package name (e.g., "com.tencent.ig")

**Returns:** `true` if game is running, `false` otherwise

**Example:**
```java
boolean found = service.findGame("com.pubg.imobile");
if (found) {
    Log.i("TAG", "PUBG Mobile is running");
}
```

---

### setScreen
**Signature:** `void setScreen(int width, int height)`  
**Native:** `Java_com_eternal_xdsdk_FloaterService_setScreen`

**Purpose:** Update screen dimensions (for orientation change)

**Parameters:**
- `width`: New screen width
- `height`: New screen height

---

### switch
**Signature:** `void switch(boolean enabled)`  
**Native:** `Java_com_eternal_xdsdk_FloaterService_switch`

**Purpose:** Enable or disable rendering

**Parameters:**
- `enabled`: `true` to start rendering, `false` to stop

---

## SuperJNI.Companion Methods

### check
**Signature:** `boolean check(String key)`  
**Native:** `Java_com_eternal_xdsdk_SuperJNI_00024Companion_check`

**Purpose:** Validate activation key with server

**Parameters:**
- `key`: Activation key string

**Returns:** `true` if key is valid, `false` otherwise

---

### licence
**Signature:** `boolean licence(String license_key)`  
**Native:** `Java_com_eternal_xdsdk_SuperJNI_00024Companion_licence`

**Purpose:** Validate license and activate application

**Parameters:**
- `license_key`: License key (32 characters)

**Returns:** `true` if license is valid and not expired

**Example:**
```java
String key = "XXXX-XXXX-XXXX-XXXX-XXXX-XXXX-XXXX";
boolean valid = SuperJNI.Companion.licence(key);
```

---

### update
**Signature:** `boolean update(String version)`  
**Native:** `Java_com_eternal_xdsdk_SuperJNI_00024Companion_update`

**Purpose:** Check if update is available for specified version

**Parameters:**
- `version`: Current version string (e.g., "3.1.0")

**Returns:** `true` if update available, `false` otherwise

---

### currGameVer
**Signature:** `String currGameVer()`  
**Native:** `Java_com_eternal_xdsdk_SuperJNI_00024Companion_currGameVer`

**Purpose:** Get current supported game version from server

**Returns:** Game version string (e.g., "2.5.0")

---

### currPlugVer
**Signature:** `String currPlugVer()`  
**Native:** `Java_com_eternal_xdsdk_SuperJNI_00024Companion_currPlugVer`

**Purpose:** Get current plugin version from server

**Returns:** Plugin version string

---

### currPlugUrl
**Signature:** `String currPlugUrl()`  
**Native:** `Java_com_eternal_xdsdk_SuperJNI_00024Companion_currPlugUrl`

**Purpose:** Get plugin download URL from server

**Returns:** Download URL string

---

### getTime
**Signature:** `long getTime()`  
**Native:** `Java_com_eternal_xdsdk_SuperJNI_00024Companion_getTime`

**Purpose:** Get server timestamp (for time sync)

**Returns:** Unix timestamp in milliseconds

---

### urlTg
**Signature:** `String urlTg()`  
**Native:** `Java_com_eternal_xdsdk_SuperJNI_00024Companion_urlTg`

**Purpose:** Get Telegram support channel URL

**Returns:** Telegram URL (e.g., "https://t.me/support")

---

## JNI Initialization

### JNI_OnLoad
**Signature:** `jint JNI_OnLoad(JavaVM* vm, void* reserved)`

**Purpose:** Called when library is loaded, initializes client

**Returns:** `JNI_VERSION_1_6`

**Implementation:**
```cpp
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    // Save JavaVM reference
    g_java_vm = vm;
    
    // Initialize client
    if (!Client::getInstance().initialize()) {
        return JNI_ERR;
    }
    
    return JNI_VERSION_1_6;
}
```

---

### JNI_OnUnload
**Signature:** `void JNI_OnUnload(JavaVM* vm, void* reserved)`

**Purpose:** Called when library is unloaded, cleanup resources

**Implementation:**
```cpp
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {
    Client::getInstance().shutdown();
}
```

---

## Complete Method List

| Class | Method | Signature | Purpose |
|-------|--------|-----------|---------|
| FloaterService | connect | (Ljava/lang/String;)V | Connect to server |
| FloaterService | disconnect | ()V | Disconnect from server |
| FloaterService | initSurface | (Landroid/view/Surface;II)V | Init OpenGL surface |
| FloaterService | removeSurface | ()V | Remove OpenGL surface |
| FloaterService | drawOn | ([BII)V | Draw texture data |
| FloaterService | drawTick | ()V | Render frame |
| FloaterService | findGame | (Ljava/lang/String;)Z | Find running game |
| FloaterService | setScreen | (II)V | Set screen size |
| FloaterService | switch | (Z)V | Toggle rendering |
| SuperJNI.Companion | check | (Ljava/lang/String;)Z | Check activation key |
| SuperJNI.Companion | licence | (Ljava/lang/String;)Z | Validate license |
| SuperJNI.Companion | update | (Ljava/lang/String;)Z | Check for updates |
| SuperJNI.Companion | currGameVer | ()Ljava/lang/String; | Get game version |
| SuperJNI.Companion | currPlugVer | ()Ljava/lang/String; | Get plugin version |
| SuperJNI.Companion | currPlugUrl | ()Ljava/lang/String; | Get plugin URL |
| SuperJNI.Companion | getTime | ()J | Get server time |
| SuperJNI.Companion | urlTg | ()Ljava/lang/String; | Get Telegram URL |

---

## Error Handling

All JNI methods handle errors gracefully:
- NULL pointer checks for string parameters
- Validation of numeric parameters
- Exception clearing after failures
- Logging of error conditions

**Example Error Handling:**
```cpp
if (connection_string == nullptr) {
    LOGW("Connection string is null");
    return;
}

std::string conn_str = JNIHelper::JavaStringToStdString(env, connection_string);
if (conn_str.empty()) {
    LOGE("Failed to convert connection string");
    return;
}
```

---

## Thread Safety

- All JNI methods are thread-safe
- Internal mutexes protect shared state
- JavaVM attachment handled automatically
- Local references cleaned up properly

---

## Performance Considerations

- JNI calls have ~100ns overhead
- String conversion allocates memory
- Array access requires pinning
- Minimize JNI crossings for performance

**Best Practices:**
- Batch operations when possible
- Cache JNI references
- Use direct ByteBuffers for large data
- Avoid frequent string conversions

---

**Document Version:** 1.0  
**Last Updated:** January 31, 2026
