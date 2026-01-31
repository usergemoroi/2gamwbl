#ifndef LIBCLIENT_ANTI_TAMPER_H
#define LIBCLIENT_ANTI_TAMPER_H

#include "types.h"
#include <string>
#include <functional>
#include <atomic>
#include <thread>
#include <memory>

namespace client {

class AntiTamper {
public:
    AntiTamper();
    ~AntiTamper();
    
    // Initialization
    bool initialize();
    void shutdown();
    
    // Security checks
    SecurityCheckResult performAllChecks();
    
    // Individual checks
    bool checkDebugger();
    bool checkRoot();
    bool checkEmulator();
    bool checkXposed();
    bool checkFrida();
    bool checkSignature();
    bool checkMemoryTampering();
    
    // Continuous monitoring
    void startMonitoring();
    void stopMonitoring();
    bool isMonitoring() const { return monitoring_; }
    
    // Configuration
    void setCheckInterval(int interval_ms);
    void enableCheck(const std::string& check_name, bool enable);
    
    // Callbacks
    using TamperDetectedCallback = std::function<void(const std::string& threat)>;
    void setTamperDetectedCallback(TamperDetectedCallback callback);
    
    // Result access
    SecurityCheckResult getLastCheckResult() const { return last_result_; }
    bool isSafe() const;
    
private:
    // Debugger detection
    bool checkTracerPid();
    bool checkDebugPort();
    bool checkDebugFlags();
    
    // Root detection
    bool checkSuBinary();
    bool checkSuperuserApp();
    bool checkRootPackages();
    bool checkRWSystem();
    bool checkTestKeys();
    
    // Emulator detection
    bool checkQemuProperties();
    bool checkGoldfishHardware();
    bool checkEmulatorFiles();
    bool checkSensorData();
    
    // Framework detection
    bool checkXposedBridge();
    bool checkXposedFiles();
    bool checkFridaAgent();
    bool checkFridaPorts();
    bool checkSubstrateFramework();
    
    // Signature verification
    bool verifyAPKSignature(const std::string& expected_signature);
    std::string getAPKSignature();
    std::string calculateSignatureHash();
    
    // Memory integrity
    bool checkCodeIntegrity();
    bool checkLibraryIntegrity();
    bool scanMemoryForPatterns();
    
    // File system checks
    bool fileExists(const std::string& path);
    std::string readFile(const std::string& path);
    std::vector<std::string> listDirectory(const std::string& path);
    
    // Process checks
    bool isProcessRunning(const std::string& process_name);
    std::vector<std::string> getRunningProcesses();
    
    // Port checks
    bool isPortOpen(int port);
    std::vector<int> getOpenPorts();
    
    // Property checks
    std::string getSystemProperty(const std::string& key);
    bool hasSystemProperty(const std::string& key);
    
    // Monitoring thread
    void monitoringThreadFunc();
    void performPeriodicChecks();
    void handleThreatDetected(const std::string& threat);
    
    // State
    std::atomic<bool> initialized_;
    std::atomic<bool> monitoring_;
    std::atomic<bool> should_stop_;
    
    // Check configuration
    struct CheckConfig {
        bool enabled;
        int weight;
    };
    std::map<std::string, CheckConfig> check_configs_;
    std::mutex config_mutex_;
    
    // Results
    SecurityCheckResult last_result_;
    mutable std::mutex result_mutex_;
    
    // Monitoring
    std::unique_ptr<std::thread> monitor_thread_;
    int check_interval_ms_;
    
    // Callback
    TamperDetectedCallback tamper_detected_callback_;
    std::mutex callback_mutex_;
    
    // Known threat signatures
    std::vector<std::string> root_packages_;
    std::vector<std::string> xposed_files_;
    std::vector<std::string> frida_signatures_;
    std::vector<std::string> emulator_properties_;
};

} // namespace client

#endif // LIBCLIENT_ANTI_TAMPER_H
