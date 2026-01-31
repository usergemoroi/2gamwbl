#include "../include/plugin_manager.h"
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "PluginManager", __VA_ARGS__)

namespace client {

PluginManager::PluginManager() 
    : initialized_(false), auto_update_enabled_(false), should_stop_(false),
      network_client_(nullptr), update_check_interval_ms_(300000), 
      verify_signatures_(true), allow_downgrades_(false) {
}

PluginManager::~PluginManager() {
    shutdown();
}

bool PluginManager::initialize(NetworkClient* network_client) {
    if (initialized_) return true;
    network_client_ = network_client;
    initialized_ = true;
    LOGI("Plugin manager initialized");
    return true;
}

void PluginManager::shutdown() {
    if (!initialized_) return;
    stopAutoUpdate();
    
    std::lock_guard<std::mutex> lock(plugins_mutex_);
    for (auto& pair : loaded_plugins_) {
        if (pair.second.handle) {
            unloadLibrary(pair.second.handle);
        }
    }
    loaded_plugins_.clear();
    initialized_ = false;
}

bool PluginManager::loadPlugin(const std::string& plugin_path) {
    if (!validatePlugin(plugin_path)) {
        return false;
    }
    
    void* handle = loadLibrary(plugin_path);
    if (!handle) {
        return false;
    }
    
    PluginInfo info;
    info.name = plugin_path;
    info.path = plugin_path;
    info.handle = handle;
    info.state = PluginState::LOADED;
    
    std::lock_guard<std::mutex> lock(plugins_mutex_);
    loaded_plugins_[info.name] = info;
    
    notifyPluginLoaded(info.name);
    return true;
}

bool PluginManager::unloadPlugin(const std::string& plugin_name) {
    std::lock_guard<std::mutex> lock(plugins_mutex_);
    auto it = loaded_plugins_.find(plugin_name);
    if (it == loaded_plugins_.end()) {
        return false;
    }
    
    if (it->second.handle) {
        unloadLibrary(it->second.handle);
    }
    
    loaded_plugins_.erase(it);
    notifyPluginUnloaded(plugin_name);
    return true;
}

bool PluginManager::isPluginLoaded(const std::string& plugin_name) const {
    std::lock_guard<std::mutex> lock(plugins_mutex_);
    return loaded_plugins_.find(plugin_name) != loaded_plugins_.end();
}

void PluginManager::enableAutoUpdate(bool enable) {
    auto_update_enabled_ = enable;
    if (enable) {
        should_stop_ = false;
        update_thread_ = std::make_unique<std::thread>(&PluginManager::updateCheckThreadFunc, this);
    } else {
        stopAutoUpdate();
    }
}

void PluginManager::stopAutoUpdate() {
    should_stop_ = true;
    if (update_thread_ && update_thread_->joinable()) {
        update_thread_->join();
    }
    update_thread_.reset();
}

void PluginManager::checkForUpdates() {
    if (!network_client_) {
        return;
    }
    performUpdateCheck();
}

std::vector<std::string> PluginManager::getLoadedPlugins() const {
    std::vector<std::string> names;
    std::lock_guard<std::mutex> lock(plugins_mutex_);
    for (const auto& pair : loaded_plugins_) {
        names.push_back(pair.first);
    }
    return names;
}

PluginMetadata PluginManager::getPluginMetadata(const std::string& plugin_name) const {
    std::lock_guard<std::mutex> lock(plugins_mutex_);
    auto it = loaded_plugins_.find(plugin_name);
    if (it != loaded_plugins_.end()) {
        return it->second.metadata;
    }
    return PluginMetadata();
}

PluginState PluginManager::getPluginState(const std::string& plugin_name) const {
    std::lock_guard<std::mutex> lock(plugins_mutex_);
    auto it = loaded_plugins_.find(plugin_name);
    if (it != loaded_plugins_.end()) {
        return it->second.state;
    }
    return PluginState::NOT_LOADED;
}

void PluginManager::setPluginDirectory(const std::string& directory) {
    plugin_directory_ = directory;
}

void PluginManager::clearPluginCache() {
    // Stub implementation
}

void* PluginManager::loadLibrary(const std::string& path) {
    return dlopen(path.c_str(), RTLD_NOW);
}

void PluginManager::unloadLibrary(void* handle) {
    if (handle) {
        dlclose(handle);
    }
}

bool PluginManager::validatePlugin(const std::string& plugin_path) {
    return true; // Stub
}

void PluginManager::updateCheckThreadFunc() {
    while (!should_stop_) {
        performUpdateCheck();
        std::this_thread::sleep_for(std::chrono::milliseconds(update_check_interval_ms_));
    }
}

void PluginManager::performUpdateCheck() {
    // Stub implementation
}

void PluginManager::notifyPluginLoaded(const std::string& plugin_name) {
    std::lock_guard<std::mutex> lock(callback_mutex_);
    if (plugin_loaded_callback_) {
        plugin_loaded_callback_(plugin_name);
    }
}

void PluginManager::notifyPluginUnloaded(const std::string& plugin_name) {
    std::lock_guard<std::mutex> lock(callback_mutex_);
    if (plugin_unloaded_callback_) {
        plugin_unloaded_callback_(plugin_name);
    }
}

void PluginManager::notifyPluginError(const std::string& plugin_name, const std::string& error) {
    std::lock_guard<std::mutex> lock(callback_mutex_);
    if (plugin_error_callback_) {
        plugin_error_callback_(plugin_name, error);
    }
}

} // namespace client
