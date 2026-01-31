#ifndef LIBCLIENT_CLIENT_H
#define LIBCLIENT_CLIENT_H

#include "types.h"
#include "render_engine.h"
#include "game_detector.h"
#include "network_client.h"
#include "plugin_manager.h"
#include "device_info.h"
#include "anti_tamper.h"

#include <memory>
#include <string>
#include <atomic>
#include <android/native_window.h>

namespace client {

class Client {
public:
    static Client& getInstance();
    
    // Lifecycle
    bool initialize();
    void shutdown();
    bool isInitialized() const { return initialized_; }
    
    // Component access
    RenderEngine* getRenderEngine() { return render_engine_.get(); }
    GameDetector* getGameDetector() { return game_detector_.get(); }
    NetworkClient* getNetworkClient() { return network_client_.get(); }
    PluginManager* getPluginManager() { return plugin_manager_.get(); }
    AntiTamper* getAntiTamper() { return anti_tamper_.get(); }
    
    // Device information
    const DeviceInfo& getDeviceInfo() const { return device_info_; }
    
    // Application state
    const AppState& getAppState() const { return app_state_; }
    void setConfigPath(const std::string& path);
    void setCachePath(const std::string& path);
    
    // Configuration
    bool loadConfig(const std::string& config_file);
    bool saveConfig(const std::string& config_file);
    
    // High-level operations
    bool connectToServer(const std::string& server_url);
    void disconnectFromServer();
    bool validateLicense(const std::string& license_key);
    
    // Rendering operations
    bool initializeRendering(ANativeWindow* window, int width, int height);
    void shutdownRendering();
    void startRendering();
    void stopRendering();
    
    // Game operations
    bool findAndAttachToGame(const std::string& package_name);
    void detachFromGame();
    bool isAttachedToGame() const;
    
    // Update operations
    bool checkForUpdates();
    bool performUpdate();
    
    // Error handling
    ErrorCode getLastError() const { return last_error_; }
    std::string getLastErrorMessage() const { return last_error_message_; }
    
private:
    Client();
    ~Client();
    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;
    
    // Initialization helpers
    bool initializeComponents();
    void shutdownComponents();
    bool verifyIntegrity();
    
    // Error handling
    void setError(ErrorCode code, const std::string& message);
    void clearError();
    
    // State
    std::atomic<bool> initialized_;
    AppState app_state_;
    DeviceInfo device_info_;
    
    // Components
    std::unique_ptr<RenderEngine> render_engine_;
    std::unique_ptr<GameDetector> game_detector_;
    std::unique_ptr<NetworkClient> network_client_;
    std::unique_ptr<PluginManager> plugin_manager_;
    std::unique_ptr<AntiTamper> anti_tamper_;
    
    // License
    LicenseInfo license_info_;
    std::mutex license_mutex_;
    
    // Error tracking
    ErrorCode last_error_;
    std::string last_error_message_;
    mutable std::mutex error_mutex_;
    
    // Configuration
    NetworkConfig network_config_;
    OverlayConfig overlay_config_;
    mutable std::mutex config_mutex_;
};

} // namespace client

#endif // LIBCLIENT_CLIENT_H
