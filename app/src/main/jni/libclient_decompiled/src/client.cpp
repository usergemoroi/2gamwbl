#include "../include/client.h"
#include "../include/jni_bridge.h"
#include <android/log.h>

#define LOG_TAG "libclient"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace client {

Client& Client::getInstance() {
    static Client instance;
    return instance;
}

Client::Client() 
    : initialized_(false), last_error_(ErrorCode::SUCCESS) {
}

Client::~Client() {
    shutdown();
}

bool Client::initialize() {
    if (initialized_) {
        return true;
    }
    
    LOGI("Initializing client v%s", constants::LIBRARY_VERSION);
    
    // Collect device information
    device_info_.populate();
    
    // Initialize components
    if (!initializeComponents()) {
        LOGE("Failed to initialize components");
        return false;
    }
    
    // Verify integrity
    if (!verifyIntegrity()) {
        LOGW("Integrity check failed");
    }
    
    app_state_.is_initialized = true;
    app_state_.is_running = true;
    app_state_.start_timestamp = platform::System::getCurrentTimeMillis();
    
    initialized_ = true;
    LOGI("Client initialized successfully");
    return true;
}

void Client::shutdown() {
    if (!initialized_) {
        return;
    }
    
    LOGI("Shutting down client");
    
    shutdownComponents();
    
    app_state_.is_running = false;
    initialized_ = false;
    
    LOGI("Client shut down");
}

bool Client::initializeComponents() {
    // Initialize render engine
    render_engine_ = std::make_unique<RenderEngine>();
    if (!render_engine_->initialize()) {
        LOGE("Failed to initialize render engine");
        return false;
    }
    
    // Initialize game detector
    game_detector_ = std::make_unique<GameDetector>();
    if (!game_detector_->initialize()) {
        LOGE("Failed to initialize game detector");
        return false;
    }
    
    // Initialize network client
    network_config_.setDefaults();
    network_client_ = std::make_unique<NetworkClient>();
    if (!network_client_->initialize(network_config_)) {
        LOGE("Failed to initialize network client");
        return false;
    }
    
    // Initialize plugin manager
    plugin_manager_ = std::make_unique<PluginManager>();
    if (!plugin_manager_->initialize(network_client_.get())) {
        LOGE("Failed to initialize plugin manager");
        return false;
    }
    
    // Initialize anti-tamper
    anti_tamper_ = std::make_unique<AntiTamper>();
    if (!anti_tamper_->initialize()) {
        LOGE("Failed to initialize anti-tamper");
        return false;
    }
    
    return true;
}

void Client::shutdownComponents() {
    if (anti_tamper_) {
        anti_tamper_->shutdown();
        anti_tamper_.reset();
    }
    
    if (plugin_manager_) {
        plugin_manager_->shutdown();
        plugin_manager_.reset();
    }
    
    if (network_client_) {
        network_client_->shutdown();
        network_client_.reset();
    }
    
    if (game_detector_) {
        game_detector_->shutdown();
        game_detector_.reset();
    }
    
    if (render_engine_) {
        render_engine_->shutdown();
        render_engine_.reset();
    }
}

bool Client::verifyIntegrity() {
    if (!anti_tamper_) {
        return false;
    }
    
    SecurityCheckResult result = anti_tamper_->performAllChecks();
    if (!result.isSafe()) {
        LOGW("Security check warnings: %s", result.warning_message.c_str());
    }
    
    return result.isSafe();
}

bool Client::connectToServer(const std::string& server_url) {
    if (!network_client_) {
        setError(ErrorCode::ERROR_INIT_FAILED, "Network client not initialized");
        return false;
    }
    
    return network_client_->connect(server_url);
}

void Client::disconnectFromServer() {
    if (network_client_) {
        network_client_->disconnect();
    }
}

bool Client::validateLicense(const std::string& license_key) {
    if (!network_client_) {
        setError(ErrorCode::ERROR_INIT_FAILED, "Network client not initialized");
        return false;
    }
    
    std::lock_guard<std::mutex> lock(license_mutex_);
    
    license_info_.license_key = license_key;
    bool valid = network_client_->checkLicense(license_key);
    license_info_.is_valid = valid;
    
    return valid;
}

bool Client::initializeRendering(ANativeWindow* window, int width, int height) {
    if (!render_engine_) {
        setError(ErrorCode::ERROR_INIT_FAILED, "Render engine not initialized");
        return false;
    }
    
    return render_engine_->createSurface(window, width, height);
}

void Client::shutdownRendering() {
    if (render_engine_) {
        render_engine_->stopRendering();
        render_engine_->destroySurface();
    }
}

void Client::startRendering() {
    if (render_engine_) {
        render_engine_->startRendering();
    }
}

void Client::stopRendering() {
    if (render_engine_) {
        render_engine_->stopRendering();
    }
}

bool Client::findAndAttachToGame(const std::string& package_name) {
    if (!game_detector_) {
        setError(ErrorCode::ERROR_INIT_FAILED, "Game detector not initialized");
        return false;
    }
    
    return game_detector_->findGame(package_name);
}

void Client::detachFromGame() {
    // Implementation would detach from currently attached game
}

bool Client::isAttachedToGame() const {
    if (!game_detector_) {
        return false;
    }
    
    return game_detector_->hasGameDetected();
}

bool Client::checkForUpdates() {
    if (!network_client_) {
        setError(ErrorCode::ERROR_INIT_FAILED, "Network client not initialized");
        return false;
    }
    
    UpdateInfo info = network_client_->checkForUpdates();
    return info.is_available;
}

bool Client::performUpdate() {
    // Implementation would download and apply update
    return false;
}

bool Client::loadConfig(const std::string& config_file) {
    // Implementation would load configuration from file
    return true;
}

bool Client::saveConfig(const std::string& config_file) {
    // Implementation would save configuration to file
    return true;
}

void Client::setConfigPath(const std::string& path) {
    app_state_.config_path = path;
}

void Client::setCachePath(const std::string& path) {
    app_state_.cache_path = path;
}

void Client::setError(ErrorCode code, const std::string& message) {
    std::lock_guard<std::mutex> lock(error_mutex_);
    last_error_ = code;
    last_error_message_ = message;
    LOGE("Error: %s", message.c_str());
}

void Client::clearError() {
    std::lock_guard<std::mutex> lock(error_mutex_);
    last_error_ = ErrorCode::SUCCESS;
    last_error_message_.clear();
}

} // namespace client
