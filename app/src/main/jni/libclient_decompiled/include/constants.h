#ifndef LIBCLIENT_CONSTANTS_H
#define LIBCLIENT_CONSTANTS_H

#include <cstdint>

namespace client {
namespace constants {

// Version information
constexpr const char* LIBRARY_VERSION = "3.1.0";
constexpr const char* LIBRARY_NAME = "libclient";
constexpr int VERSION_MAJOR = 3;
constexpr int VERSION_MINOR = 1;
constexpr int VERSION_PATCH = 0;

// Network configuration
constexpr const char* DEFAULT_SERVER_URL = "https://api.gameblaster.example.com";
constexpr int DEFAULT_SERVER_PORT = 443;
constexpr int DEFAULT_TIMEOUT_MS = 30000;
constexpr int DEFAULT_CONNECT_TIMEOUT_MS = 10000;
constexpr int DEFAULT_READ_TIMEOUT_MS = 20000;
constexpr int MAX_RECONNECT_ATTEMPTS = 5;
constexpr int RECONNECT_DELAY_MS = 3000;
constexpr int PING_INTERVAL_MS = 60000;

// API endpoints
constexpr const char* ENDPOINT_CHECK = "/api/v1/check";
constexpr const char* ENDPOINT_LICENSE = "/api/v1/license";
constexpr const char* ENDPOINT_UPDATE = "/api/v1/update";
constexpr const char* ENDPOINT_VERSION = "/api/v1/version";
constexpr const char* ENDPOINT_PLUGIN = "/api/v1/plugin";
constexpr const char* ENDPOINT_TELEGRAM = "/api/v1/telegram";
constexpr const char* ENDPOINT_CONFIG = "/api/v1/config";

// HTTP headers
constexpr const char* HEADER_USER_AGENT = "User-Agent";
constexpr const char* HEADER_CONTENT_TYPE = "Content-Type";
constexpr const char* HEADER_AUTHORIZATION = "Authorization";
constexpr const char* HEADER_DEVICE_ID = "X-Device-ID";
constexpr const char* HEADER_APP_VERSION = "X-App-Version";
constexpr const char* HEADER_GAME_VERSION = "X-Game-Version";

// Default values
constexpr const char* DEFAULT_USER_AGENT = "GameBlaster/3.1 (Android)";
constexpr const char* CONTENT_TYPE_JSON = "application/json";
constexpr const char* CONTENT_TYPE_FORM = "application/x-www-form-urlencoded";

// OpenGL/EGL configuration
constexpr int DEFAULT_SURFACE_WIDTH = 1920;
constexpr int DEFAULT_SURFACE_HEIGHT = 1080;
constexpr int MIN_SURFACE_WIDTH = 320;
constexpr int MIN_SURFACE_HEIGHT = 240;
constexpr int MAX_SURFACE_WIDTH = 4096;
constexpr int MAX_SURFACE_HEIGHT = 4096;
constexpr int DEFAULT_DEPTH_SIZE = 24;
constexpr int DEFAULT_STENCIL_SIZE = 8;
constexpr int EGL_VERSION_MAJOR = 3;
constexpr int EGL_VERSION_MINOR = 0;

// Rendering configuration
constexpr int DEFAULT_FPS_TARGET = 60;
constexpr int MIN_FPS = 15;
constexpr int MAX_FPS = 120;
constexpr float DEFAULT_CLEAR_COLOR_R = 0.0f;
constexpr float DEFAULT_CLEAR_COLOR_G = 0.0f;
constexpr float DEFAULT_CLEAR_COLOR_B = 0.0f;
constexpr float DEFAULT_CLEAR_COLOR_A = 0.0f;
constexpr int MAX_VERTICES = 65536;
constexpr int MAX_INDICES = 196608;

// Texture configuration
constexpr int MAX_TEXTURE_SIZE = 4096;
constexpr int MAX_TEXTURE_UNITS = 16;
constexpr int DEFAULT_TEXTURE_FORMAT = 0x1908; // GL_RGBA
constexpr int DEFAULT_TEXTURE_TYPE = 0x1401;   // GL_UNSIGNED_BYTE

// Buffer configuration
constexpr size_t DEFAULT_BUFFER_SIZE = 4096;
constexpr size_t MAX_BUFFER_SIZE = 1048576; // 1 MB
constexpr size_t NETWORK_BUFFER_SIZE = 8192;
constexpr size_t READ_BUFFER_SIZE = 16384;

// Game detection
constexpr int PROCESS_SCAN_INTERVAL_MS = 2000;
constexpr int GAME_CHECK_TIMEOUT_MS = 5000;
constexpr int MAX_PROCESS_NAME_LEN = 256;

// Target game packages
constexpr const char* GAME_PUBG_MOBILE_CN = "com.tencent.ig";
constexpr const char* GAME_PUBG_MOBILE_GLOBAL = "com.pubg.imobile";
constexpr const char* GAME_PUBG_MOBILE_KR = "com.pubg.krmobile";
constexpr const char* GAME_PUBG_MOBILE_VN = "com.vng.pubgmobile";
constexpr const char* GAME_PUBG_MOBILE_TW = "com.rekoo.pubgm";
constexpr const char* GAME_FREE_FIRE_TW = "com.garena.game.fctw";
constexpr const char* GAME_FREE_FIRE_TH = "com.dts.freefireth";
constexpr const char* GAME_FREE_FIRE_MAX = "com.dts.freefiremax";

// File paths
constexpr const char* CONFIG_FILE_NAME = "config.json";
constexpr const char* CACHE_DIR_NAME = "cache";
constexpr const char* PLUGIN_DIR_NAME = "plugins";
constexpr const char* LOG_FILE_NAME = "client.log";
constexpr const char* LICENSE_FILE_NAME = "license.dat";

// Security
constexpr const char* SIGNATURE_HASH = ""; // Placeholder
constexpr int MAX_DEBUG_CHECK_INTERVAL_MS = 5000;
constexpr int SECURITY_CHECK_INTERVAL_MS = 10000;

// Plugin configuration
constexpr int MAX_PLUGIN_SIZE = 52428800; // 50 MB
constexpr int PLUGIN_CHECK_INTERVAL_MS = 300000; // 5 minutes
constexpr int PLUGIN_DOWNLOAD_TIMEOUT_MS = 120000; // 2 minutes
constexpr const char* PLUGIN_EXTENSION = ".so";

// License configuration
constexpr int LICENSE_KEY_LENGTH = 32;
constexpr int MIN_LICENSE_KEY_LENGTH = 16;
constexpr int LICENSE_CHECK_INTERVAL_MS = 3600000; // 1 hour
constexpr int LICENSE_GRACE_PERIOD_DAYS = 3;

// Overlay configuration
constexpr int DEFAULT_OVERLAY_WIDTH = 400;
constexpr int DEFAULT_OVERLAY_HEIGHT = 300;
constexpr int MIN_OVERLAY_SIZE = 100;
constexpr int MAX_OVERLAY_SIZE = 1920;
constexpr float DEFAULT_OPACITY = 0.8f;
constexpr float MIN_OPACITY = 0.1f;
constexpr float MAX_OPACITY = 1.0f;

// Thread configuration
constexpr int RENDER_THREAD_PRIORITY = -4; // THREAD_PRIORITY_DISPLAY
constexpr int NETWORK_THREAD_PRIORITY = -2; // THREAD_PRIORITY_BACKGROUND
constexpr int WORKER_THREAD_PRIORITY = 0;  // THREAD_PRIORITY_DEFAULT
constexpr int MAX_WORKER_THREADS = 4;

// Timing
constexpr int FRAME_TIME_MS_60FPS = 16;
constexpr int FRAME_TIME_MS_30FPS = 33;
constexpr uint64_t MILLISECONDS_PER_SECOND = 1000;
constexpr uint64_t MICROSECONDS_PER_MILLISECOND = 1000;
constexpr uint64_t NANOSECONDS_PER_MILLISECOND = 1000000;

// Logging
constexpr const char* LOG_TAG = "libclient";
constexpr int MAX_LOG_MESSAGE_SIZE = 1024;
constexpr bool ENABLE_DEBUG_LOGS = true;
constexpr bool ENABLE_VERBOSE_LOGS = false;

// Memory limits
constexpr size_t MAX_STRING_LENGTH = 4096;
constexpr size_t MAX_PATH_LENGTH = 512;
constexpr size_t MAX_FILENAME_LENGTH = 255;
constexpr size_t MAX_JSON_SIZE = 1048576; // 1 MB

// Retry configuration
constexpr int MAX_RETRY_ATTEMPTS = 3;
constexpr int RETRY_DELAY_MS = 1000;
constexpr int RETRY_BACKOFF_MULTIPLIER = 2;

// Status codes
constexpr int HTTP_OK = 200;
constexpr int HTTP_CREATED = 201;
constexpr int HTTP_BAD_REQUEST = 400;
constexpr int HTTP_UNAUTHORIZED = 401;
constexpr int HTTP_FORBIDDEN = 403;
constexpr int HTTP_NOT_FOUND = 404;
constexpr int HTTP_SERVER_ERROR = 500;
constexpr int HTTP_SERVICE_UNAVAILABLE = 503;

// Magic numbers and signatures
constexpr uint32_t MAGIC_NUMBER = 0x434C4E54; // "CLNT"
constexpr uint32_t CONFIG_VERSION = 1;
constexpr uint32_t PLUGIN_SIGNATURE = 0x504C5547; // "PLUG"

// Encryption (Burri scheme)
constexpr const char* BURRI_KEY = "Burri Burri Encryption Key v3.1";
constexpr const char* BURRI_IV = "GameBlasterPro\0\0";
constexpr int AES_KEY_SIZE = 32;
constexpr int AES_IV_SIZE = 16;
constexpr int AES_BLOCK_SIZE = 16;

// String obfuscation
constexpr uint8_t XOR_KEY = 0x42;

// JNI configuration
constexpr int JNI_VERSION = 0x00010006; // JNI_VERSION_1_6
constexpr const char* JNI_CLASS_FLOATER_SERVICE = "com/eternal/xdsdk/FloaterService";
constexpr const char* JNI_CLASS_SUPER_JNI = "com/eternal/xdsdk/SuperJNI$Companion";

// Method signatures
constexpr const char* JNI_SIG_VOID = "()V";
constexpr const char* JNI_SIG_BOOLEAN = "()Z";
constexpr const char* JNI_SIG_INT = "()I";
constexpr const char* JNI_SIG_LONG = "()J";
constexpr const char* JNI_SIG_STRING = "()Ljava/lang/String;";
constexpr const char* JNI_SIG_BOOLEAN_STRING = "(Ljava/lang/String;)Z";
constexpr const char* JNI_SIG_VOID_STRING = "(Ljava/lang/String;)V";

// Performance tuning
constexpr int CACHE_LINE_SIZE = 64;
constexpr int PREFETCH_DISTANCE = 64;
constexpr bool ENABLE_SIMD = true;
constexpr bool ENABLE_THREADING = true;

// Feature flags
constexpr bool FEATURE_AUTO_UPDATE = true;
constexpr bool FEATURE_TELEMETRY = false;
constexpr bool FEATURE_CRASH_REPORTING = false;
constexpr bool FEATURE_ANALYTICS = false;

} // namespace constants
} // namespace client

#endif // LIBCLIENT_CONSTANTS_H
