#ifndef LIBCLIENT_TYPES_H
#define LIBCLIENT_TYPES_H

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <EGL/egl.h>
#include <GLES3/gl3.h>

namespace client {

// Forward declarations
class RenderEngine;
class GameDetector;
class NetworkClient;
class PluginManager;

// Device information structure
struct DeviceInfo {
    std::string android_id;
    std::string oaid;
    std::string imei;
    std::string serial_number;
    std::string mac_address;
    std::string manufacturer;
    std::string model;
    std::string android_version;
    int sdk_version;
    
    DeviceInfo();
    void populate();
};

// Game information structure
struct GameInfo {
    std::string package_name;
    std::string version_name;
    int version_code;
    int uid;
    int pid;
    bool is_running;
    uint64_t start_time;
    
    GameInfo();
    void reset();
};

// Network configuration
struct NetworkConfig {
    std::string server_url;
    int port;
    int timeout_ms;
    bool use_ssl;
    std::string user_agent;
    std::string auth_token;
    
    NetworkConfig();
    void setDefaults();
};

// Plugin metadata
struct PluginMetadata {
    std::string name;
    std::string version;
    std::string url;
    uint64_t size;
    std::string checksum;
    std::string description;
    bool is_downloaded;
    bool is_loaded;
    
    PluginMetadata();
};

// OpenGL context wrapper
struct GLContext {
    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
    EGLConfig config;
    int width;
    int height;
    bool is_initialized;
    
    GLContext();
    void reset();
};

// Shader data
struct ShaderData {
    GLuint program;
    GLuint vertex_shader;
    GLuint fragment_shader;
    std::string vertex_source;
    std::string fragment_source;
    bool is_compiled;
    
    ShaderData();
    void cleanup();
};

// Vertex buffer data
struct VertexBuffer {
    GLuint vbo;
    GLuint vao;
    GLuint ebo;
    size_t vertex_count;
    size_t index_count;
    bool is_created;
    
    VertexBuffer();
    void cleanup();
};

// Texture data
struct TextureData {
    GLuint texture_id;
    int width;
    int height;
    GLenum format;
    GLenum internal_format;
    bool is_loaded;
    
    TextureData();
    void cleanup();
};

// License information
struct LicenseInfo {
    std::string license_key;
    std::string user_id;
    uint64_t expiration_timestamp;
    bool is_valid;
    bool is_expired;
    int days_remaining;
    
    LicenseInfo();
    void checkExpiration();
};

// Server response structure
struct ServerResponse {
    int status_code;
    std::string body;
    std::string error_message;
    bool success;
    uint64_t timestamp;
    
    ServerResponse();
};

// Rendering state
struct RenderState {
    bool is_rendering;
    bool is_visible;
    float fps;
    uint64_t frame_count;
    uint64_t last_frame_time;
    std::mutex state_mutex;
    
    RenderState();
};

// Connection state
struct ConnectionState {
    bool is_connected;
    std::string server_address;
    int port;
    uint64_t connect_time;
    uint64_t last_ping_time;
    int reconnect_attempts;
    std::mutex conn_mutex;
    
    ConnectionState();
    void reset();
};

// Security check results
struct SecurityCheckResult {
    bool is_rooted;
    bool is_emulator;
    bool is_debugged;
    bool has_xposed;
    bool has_frida;
    bool signature_valid;
    std::string warning_message;
    
    SecurityCheckResult();
    bool isSafe() const;
};

// Application state
struct AppState {
    bool is_initialized;
    bool is_running;
    std::string app_version;
    std::string build_number;
    uint64_t start_timestamp;
    std::string config_path;
    std::string cache_path;
    
    AppState();
};

// Overlay configuration
struct OverlayConfig {
    int x_position;
    int y_position;
    int width;
    int height;
    float opacity;
    bool draggable;
    bool resizable;
    bool always_on_top;
    
    OverlayConfig();
    void setDefaults();
};

// Touch event data
struct TouchEvent {
    int action;
    float x;
    float y;
    float pressure;
    uint64_t timestamp;
    int pointer_id;
    
    TouchEvent();
};

// Target game descriptor
struct TargetGame {
    std::string package_name;
    std::string display_name;
    std::string region;
    int min_version;
    int max_version;
    bool is_supported;
    
    TargetGame();
};

// Update information
struct UpdateInfo {
    std::string latest_version;
    std::string download_url;
    std::string release_notes;
    uint64_t file_size;
    bool is_mandatory;
    bool is_available;
    
    UpdateInfo();
};

// Constants
namespace constants {
    constexpr int MAX_RECONNECT_ATTEMPTS = 5;
    constexpr int DEFAULT_TIMEOUT_MS = 30000;
    constexpr int MAX_TEXTURE_SIZE = 4096;
    constexpr int DEFAULT_FPS_TARGET = 60;
    constexpr float DEFAULT_OPACITY = 0.8f;
    
    // API endpoints
    constexpr const char* API_CHECK = "/api/v1/check";
    constexpr const char* API_UPDATE = "/api/v1/update";
    constexpr const char* API_VERSION = "/api/v1/version";
    constexpr const char* API_PLUGIN = "/api/v1/plugin";
    
    // Default values
    constexpr const char* DEFAULT_USER_AGENT = "GameBlaster/3.1";
    constexpr const char* DEFAULT_SERVER = "https://api.gameblaster.example.com";
    constexpr int DEFAULT_PORT = 443;
}

// Error codes
enum class ErrorCode {
    SUCCESS = 0,
    ERROR_INIT_FAILED = 1,
    ERROR_INVALID_PARAMS = 2,
    ERROR_NETWORK_FAILED = 3,
    ERROR_LICENSE_INVALID = 4,
    ERROR_GL_FAILED = 5,
    ERROR_GAME_NOT_FOUND = 6,
    ERROR_PLUGIN_LOAD_FAILED = 7,
    ERROR_SECURITY_CHECK_FAILED = 8,
    ERROR_FILE_NOT_FOUND = 9,
    ERROR_PERMISSION_DENIED = 10,
    ERROR_UNKNOWN = 99
};

// Connection status
enum class ConnectionStatus {
    DISCONNECTED = 0,
    CONNECTING = 1,
    CONNECTED = 2,
    RECONNECTING = 3,
    ERROR_STATE = 4
};

// Game detection status
enum class GameStatus {
    NOT_DETECTED = 0,
    DETECTED = 1,
    RUNNING = 2,
    STOPPED = 3,
    UNSUPPORTED = 4
};

// Render mode
enum class RenderMode {
    IDLE = 0,
    ACTIVE = 1,
    PAUSED = 2,
    OVERLAY = 3
};

// Plugin state
enum class PluginState {
    NOT_LOADED = 0,
    LOADING = 1,
    LOADED = 2,
    ERROR_STATE = 3,
    UNLOADING = 4
};

} // namespace client

#endif // LIBCLIENT_TYPES_H
