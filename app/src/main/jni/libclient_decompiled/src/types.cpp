#include "../include/types.h"
#include "../include/device_info.h"
#include "../include/internal/platform_specific.h"

namespace client {

// DeviceInfo implementation
DeviceInfo::DeviceInfo() : sdk_version(0) {}

void DeviceInfo::populate() {
    *this = DeviceInfoCollector::collect();
}

// GameInfo implementation
GameInfo::GameInfo() 
    : version_code(0), uid(0), pid(0), is_running(false), start_time(0) {}

void GameInfo::reset() {
    package_name.clear();
    version_name.clear();
    version_code = 0;
    uid = 0;
    pid = 0;
    is_running = false;
    start_time = 0;
}

// NetworkConfig implementation
NetworkConfig::NetworkConfig() 
    : port(443), timeout_ms(30000), use_ssl(true) {}

void NetworkConfig::setDefaults() {
    server_url = "https://api.gameblaster.example.com";
    port = 443;
    timeout_ms = 30000;
    use_ssl = true;
    user_agent = "GameBlaster/3.1 (Android)";
}

// PluginMetadata implementation
PluginMetadata::PluginMetadata() 
    : size(0), is_downloaded(false), is_loaded(false) {}

// GLContext implementation
GLContext::GLContext() 
    : display(EGL_NO_DISPLAY), context(EGL_NO_CONTEXT), 
      surface(EGL_NO_SURFACE), config(nullptr),
      width(0), height(0), is_initialized(false) {}

void GLContext::reset() {
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
    config = nullptr;
    width = 0;
    height = 0;
    is_initialized = false;
}

// ShaderData implementation
ShaderData::ShaderData() 
    : program(0), vertex_shader(0), fragment_shader(0), is_compiled(false) {}

void ShaderData::cleanup() {
    program = 0;
    vertex_shader = 0;
    fragment_shader = 0;
    vertex_source.clear();
    fragment_source.clear();
    is_compiled = false;
}

// VertexBuffer implementation
VertexBuffer::VertexBuffer() 
    : vbo(0), vao(0), ebo(0), vertex_count(0), index_count(0), is_created(false) {}

void VertexBuffer::cleanup() {
    vbo = 0;
    vao = 0;
    ebo = 0;
    vertex_count = 0;
    index_count = 0;
    is_created = false;
}

// TextureData implementation
TextureData::TextureData() 
    : texture_id(0), width(0), height(0), format(0), internal_format(0), is_loaded(false) {}

void TextureData::cleanup() {
    texture_id = 0;
    width = 0;
    height = 0;
    is_loaded = false;
}

// LicenseInfo implementation
LicenseInfo::LicenseInfo() 
    : expiration_timestamp(0), is_valid(false), is_expired(false), days_remaining(0) {}

void LicenseInfo::checkExpiration() {
    // Implementation would check current time against expiration_timestamp
    is_expired = false;
    days_remaining = 365;
}

// ServerResponse implementation
ServerResponse::ServerResponse() 
    : status_code(0), success(false), timestamp(0) {}

// RenderState implementation
RenderState::RenderState() 
    : is_rendering(false), is_visible(true), fps(0.0f), 
      frame_count(0), last_frame_time(0) {}

// ConnectionState implementation
ConnectionState::ConnectionState() 
    : is_connected(false), port(0), connect_time(0), 
      last_ping_time(0), reconnect_attempts(0) {}

void ConnectionState::reset() {
    is_connected = false;
    server_address.clear();
    port = 0;
    connect_time = 0;
    last_ping_time = 0;
    reconnect_attempts = 0;
}

// SecurityCheckResult implementation
SecurityCheckResult::SecurityCheckResult() 
    : is_rooted(false), is_emulator(false), is_debugged(false), 
      has_xposed(false), has_frida(false), signature_valid(true) {}

bool SecurityCheckResult::isSafe() const {
    return !is_rooted && !is_emulator && !is_debugged && 
           !has_xposed && !has_frida && signature_valid;
}

// AppState implementation
AppState::AppState() 
    : is_initialized(false), is_running(false), start_timestamp(0) {
    app_version = "3.1.0";
    build_number = "1";
}

// OverlayConfig implementation
OverlayConfig::OverlayConfig() 
    : x_position(0), y_position(0), width(400), height(300), 
      opacity(0.8f), draggable(true), resizable(true), always_on_top(true) {}

void OverlayConfig::setDefaults() {
    x_position = 0;
    y_position = 0;
    width = 400;
    height = 300;
    opacity = 0.8f;
    draggable = true;
    resizable = true;
    always_on_top = true;
}

// TouchEvent implementation
TouchEvent::TouchEvent() 
    : action(0), x(0.0f), y(0.0f), pressure(0.0f), timestamp(0), pointer_id(0) {}

// TargetGame implementation
TargetGame::TargetGame() 
    : min_version(0), max_version(INT32_MAX), is_supported(true) {}

// UpdateInfo implementation
UpdateInfo::UpdateInfo() 
    : file_size(0), is_mandatory(false), is_available(false) {}

} // namespace client
