#ifndef LIBCLIENT_NETWORK_CLIENT_H
#define LIBCLIENT_NETWORK_CLIENT_H

#include "types.h"
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <atomic>
#include <thread>

namespace client {

class NetworkClient {
public:
    NetworkClient();
    ~NetworkClient();
    
    // Initialization
    bool initialize(const NetworkConfig& config);
    void shutdown();
    bool isInitialized() const { return initialized_; }
    
    // Connection management
    bool connect(const std::string& server_url);
    void disconnect();
    bool isConnected() const { return connected_; }
    ConnectionStatus getConnectionStatus() const { return connection_status_; }
    
    // HTTP requests
    ServerResponse get(const std::string& endpoint);
    ServerResponse post(const std::string& endpoint, const std::string& data);
    ServerResponse put(const std::string& endpoint, const std::string& data);
    ServerResponse del(const std::string& endpoint);
    
    // Specific API calls
    bool checkLicense(const std::string& license_key);
    bool validateKey(const std::string& key);
    UpdateInfo checkForUpdates();
    std::string getPluginUrl();
    std::string getPluginVersion();
    std::string getGameVersion();
    std::string getTelegramUrl();
    uint64_t getServerTime();
    
    // File operations
    bool downloadFile(const std::string& url, const std::string& output_path);
    bool downloadPlugin(const PluginMetadata& plugin, const std::string& output_path);
    
    // Configuration
    void setServerUrl(const std::string& url);
    void setTimeout(int timeout_ms);
    void setAuthToken(const std::string& token);
    void setUserAgent(const std::string& user_agent);
    void addHeader(const std::string& key, const std::string& value);
    void removeHeader(const std::string& key);
    
    // Callbacks
    using ProgressCallback = std::function<void(uint64_t current, uint64_t total)>;
    void setDownloadProgressCallback(ProgressCallback callback);
    
    // Error handling
    std::string getLastError() const { return last_error_; }
    int getLastErrorCode() const { return last_error_code_; }
    
private:
    // HTTP operations
    ServerResponse sendRequest(const std::string& method, 
                              const std::string& url,
                              const std::string& data = "");
    
    // SSL/TLS operations
    bool initSSL();
    void cleanupSSL();
    bool createSSLConnection(const std::string& host, int port);
    void closeSSLConnection();
    
    // Request building
    std::string buildRequest(const std::string& method,
                            const std::string& path,
                            const std::string& host,
                            const std::string& data);
    
    // Response parsing
    ServerResponse parseResponse(const std::string& raw_response);
    int parseStatusCode(const std::string& status_line);
    std::map<std::string, std::string> parseHeaders(const std::string& headers_text);
    std::string parseBody(const std::string& response);
    
    // URL parsing
    bool parseUrl(const std::string& url, 
                 std::string& protocol,
                 std::string& host,
                 int& port,
                 std::string& path);
    
    // Socket operations
    int createSocket(const std::string& host, int port);
    void closeSocket(int socket_fd);
    bool sendData(int socket_fd, const char* data, size_t length);
    bool receiveData(int socket_fd, std::vector<char>& buffer);
    
    // Connection maintenance
    void connectionMonitorFunc();
    bool ping();
    void attemptReconnect();
    
    // Device information for requests
    void addDeviceHeaders();
    std::string getDeviceId() const;
    
    // JSON utilities
    std::string createJsonRequest(const std::map<std::string, std::string>& data);
    std::map<std::string, std::string> parseJsonResponse(const std::string& json);
    
    // Error handling
    void setError(const std::string& error, int code = 0);
    void clearError();
    
    // State
    std::atomic<bool> initialized_;
    std::atomic<bool> connected_;
    ConnectionStatus connection_status_;
    
    // Configuration
    NetworkConfig config_;
    std::mutex config_mutex_;
    
    // Connection state
    ConnectionState conn_state_;
    int socket_fd_;
    void* ssl_context_;
    void* ssl_connection_;
    
    // Headers
    std::map<std::string, std::string> custom_headers_;
    std::mutex headers_mutex_;
    
    // Device information
    DeviceInfo device_info_;
    
    // Callbacks
    ProgressCallback progress_callback_;
    std::mutex callback_mutex_;
    
    // Connection monitoring
    std::unique_ptr<std::thread> monitor_thread_;
    std::atomic<bool> should_stop_monitor_;
    
    // Error tracking
    std::string last_error_;
    int last_error_code_;
    mutable std::mutex error_mutex_;
    
    // Retry configuration
    int max_retry_attempts_;
    int retry_delay_ms_;
};

} // namespace client

#endif // LIBCLIENT_NETWORK_CLIENT_H
