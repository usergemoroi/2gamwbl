#ifndef LIBCLIENT_PLUGIN_MANAGER_H
#define LIBCLIENT_PLUGIN_MANAGER_H

#include "types.h"
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <atomic>
#include <thread>

namespace client {

class NetworkClient;

class PluginManager {
public:
    PluginManager();
    ~PluginManager();
    
    // Initialization
    bool initialize(NetworkClient* network_client);
    void shutdown();
    bool isInitialized() const { return initialized_; }
    
    // Plugin lifecycle
    bool loadPlugin(const std::string& plugin_path);
    bool unloadPlugin(const std::string& plugin_name);
    bool reloadPlugin(const std::string& plugin_name);
    bool isPluginLoaded(const std::string& plugin_name) const;
    
    // Plugin discovery and updates
    void checkForUpdates();
    bool downloadPlugin(const PluginMetadata& metadata);
    bool updatePlugin(const std::string& plugin_name);
    std::vector<PluginMetadata> getAvailablePlugins();
    
    // Automatic updates
    void enableAutoUpdate(bool enable);
    bool isAutoUpdateEnabled() const { return auto_update_enabled_; }
    void setUpdateInterval(int interval_ms);
    
    // Plugin information
    std::vector<std::string> getLoadedPlugins() const;
    PluginMetadata getPluginMetadata(const std::string& plugin_name) const;
    PluginState getPluginState(const std::string& plugin_name) const;
    
    // Plugin directory management
    void setPluginDirectory(const std::string& directory);
    std::string getPluginDirectory() const { return plugin_directory_; }
    void clearPluginCache();
    
    // Callbacks
    using PluginLoadedCallback = std::function<void(const std::string&)>;
    using PluginUnloadedCallback = std::function<void(const std::string&)>;
    using PluginErrorCallback = std::function<void(const std::string&, const std::string&)>;
    using UpdateAvailableCallback = std::function<void(const PluginMetadata&)>;
    
    void setPluginLoadedCallback(PluginLoadedCallback callback);
    void setPluginUnloadedCallback(PluginUnloadedCallback callback);
    void setPluginErrorCallback(PluginErrorCallback callback);
    void setUpdateAvailableCallback(UpdateAvailableCallback callback);
    
    // Error handling
    std::string getLastError() const { return last_error_; }
    
private:
    // Plugin loading internals
    void* loadLibrary(const std::string& path);
    void unloadLibrary(void* handle);
    void* getSymbol(void* handle, const std::string& symbol_name);
    
    // Plugin validation
    bool validatePlugin(const std::string& plugin_path);
    bool verifySignature(const std::string& plugin_path);
    bool checkCompatibility(const PluginMetadata& metadata);
    
    // Plugin metadata
    bool readMetadata(const std::string& plugin_path, PluginMetadata& metadata);
    bool writeMetadata(const std::string& plugin_path, const PluginMetadata& metadata);
    
    // Update checking
    void updateCheckThreadFunc();
    void performUpdateCheck();
    std::vector<PluginMetadata> fetchPluginList();
    bool isUpdateAvailable(const std::string& plugin_name, const std::string& current_version);
    
    // Plugin installation
    bool installPlugin(const std::string& source_path, const std::string& plugin_name);
    bool removePlugin(const std::string& plugin_name);
    bool backupPlugin(const std::string& plugin_name);
    bool restorePlugin(const std::string& plugin_name);
    
    // File operations
    bool extractPlugin(const std::string& archive_path, const std::string& output_dir);
    bool verifyChecksum(const std::string& file_path, const std::string& expected_checksum);
    std::string calculateChecksum(const std::string& file_path);
    
    // Path management
    std::string getPluginPath(const std::string& plugin_name) const;
    std::string getBackupPath(const std::string& plugin_name) const;
    std::string getTempPath(const std::string& plugin_name) const;
    
    // Callbacks execution
    void notifyPluginLoaded(const std::string& plugin_name);
    void notifyPluginUnloaded(const std::string& plugin_name);
    void notifyPluginError(const std::string& plugin_name, const std::string& error);
    void notifyUpdateAvailable(const PluginMetadata& metadata);
    
    // Error handling
    void setError(const std::string& error);
    void clearError();
    
    // State
    std::atomic<bool> initialized_;
    std::atomic<bool> auto_update_enabled_;
    std::atomic<bool> should_stop_;
    
    // Plugin tracking
    struct PluginInfo {
        std::string name;
        std::string path;
        void* handle;
        PluginMetadata metadata;
        PluginState state;
        uint64_t load_time;
    };
    
    std::map<std::string, PluginInfo> loaded_plugins_;
    mutable std::mutex plugins_mutex_;
    
    // Network client
    NetworkClient* network_client_;
    
    // Configuration
    std::string plugin_directory_;
    int update_check_interval_ms_;
    bool verify_signatures_;
    bool allow_downgrades_;
    
    // Update checking thread
    std::unique_ptr<std::thread> update_thread_;
    std::mutex update_mutex_;
    
    // Callbacks
    PluginLoadedCallback plugin_loaded_callback_;
    PluginUnloadedCallback plugin_unloaded_callback_;
    PluginErrorCallback plugin_error_callback_;
    UpdateAvailableCallback update_available_callback_;
    std::mutex callback_mutex_;
    
    // Error tracking
    std::string last_error_;
    mutable std::mutex error_mutex_;
};

} // namespace client

#endif // LIBCLIENT_PLUGIN_MANAGER_H
