#include "../include/anti_tamper.h"
#include "../include/internal/platform_specific.h"

namespace client {

AntiTamper::AntiTamper() 
    : initialized_(false), monitoring_(false), should_stop_(false), check_interval_ms_(5000) {
}

AntiTamper::~AntiTamper() {
    shutdown();
}

bool AntiTamper::initialize() {
    if (initialized_) return true;
    
    // Initialize threat signatures
    root_packages_ = {
        "com.noshufou.android.su",
        "com.thirdparty.superuser",
        "eu.chainfire.supersu",
        "com.koushikdutta.superuser",
        "com.zachspong.temprootremovejb",
        "com.ramdroid.appquarantine"
    };
    
    xposed_files_ = {
        "/system/framework/XposedBridge.jar",
        "/system/bin/app_process_xposed",
        "/system/lib/libxposed_art.so"
    };
    
    frida_signatures_ = {
        "frida-agent",
        "gum-js-loop",
        "frida-server"
    };
    
    initialized_ = true;
    return true;
}

void AntiTamper::shutdown() {
    stopMonitoring();
    initialized_ = false;
}

SecurityCheckResult AntiTamper::performAllChecks() {
    SecurityCheckResult result;
    result.is_rooted = checkRoot();
    result.is_emulator = checkEmulator();
    result.is_debugged = checkDebugger();
    result.has_xposed = checkXposed();
    result.has_frida = checkFrida();
    result.signature_valid = checkSignature();
    result.warning_message = "";
    
    if (!result.isSafe()) {
        if (result.is_rooted) result.warning_message += "Root detected; ";
        if (result.is_emulator) result.warning_message += "Emulator detected; ";
        if (result.is_debugged) result.warning_message += "Debugger detected; ";
        if (result.has_xposed) result.warning_message += "Xposed detected; ";
        if (result.has_frida) result.warning_message += "Frida detected; ";
    }
    
    std::lock_guard<std::mutex> lock(result_mutex_);
    last_result_ = result;
    
    return result;
}

bool AntiTamper::checkDebugger() {
    return checkTracerPid();
}

bool AntiTamper::checkTracerPid() {
    std::string status = platform::Process::readProcFile(platform::Process::getCurrentPid(), "status");
    return status.find("TracerPid:\t0") == std::string::npos;
}

bool AntiTamper::checkRoot() {
    return checkSuBinary() || checkRootPackages();
}

bool AntiTamper::checkSuBinary() {
    std::vector<std::string> su_paths = {
        "/system/bin/su",
        "/system/xbin/su",
        "/sbin/su",
        "/su/bin/su"
    };
    
    for (const auto& path : su_paths) {
        if (platform::FileSystem::exists(path)) {
            return true;
        }
    }
    return false;
}

bool AntiTamper::checkRootPackages() {
    for (const auto& pkg : root_packages_) {
        if (platform::Process::isProcessRunning(pkg)) {
            return true;
        }
    }
    return false;
}

bool AntiTamper::checkEmulator() {
    std::string hardware = platform::System::getProperty("ro.hardware");
    std::string product = platform::System::getProperty("ro.product.device");
    
    return hardware.find("goldfish") != std::string::npos ||
           hardware.find("ranchu") != std::string::npos ||
           product.find("sdk") != std::string::npos ||
           product.find("emulator") != std::string::npos;
}

bool AntiTamper::checkXposed() {
    for (const auto& file : xposed_files_) {
        if (platform::FileSystem::exists(file)) {
            return true;
        }
    }
    return false;
}

bool AntiTamper::checkFrida() {
    // Stub - would check for Frida signatures in memory/ports
    return false;
}

bool AntiTamper::checkSignature() {
    // Stub - would verify APK signature
    return true;
}

bool AntiTamper::checkMemoryTampering() {
    // Stub - would check memory integrity
    return false;
}

void AntiTamper::startMonitoring() {
    if (monitoring_) return;
    
    should_stop_ = false;
    monitoring_ = true;
    monitor_thread_ = std::make_unique<std::thread>(&AntiTamper::monitoringThreadFunc, this);
}

void AntiTamper::stopMonitoring() {
    if (!monitoring_) return;
    
    should_stop_ = true;
    if (monitor_thread_ && monitor_thread_->joinable()) {
        monitor_thread_->join();
    }
    monitor_thread_.reset();
    monitoring_ = false;
}

void AntiTamper::setCheckInterval(int interval_ms) {
    check_interval_ms_ = interval_ms;
}

bool AntiTamper::isSafe() const {
    std::lock_guard<std::mutex> lock(result_mutex_);
    return last_result_.isSafe();
}

void AntiTamper::monitoringThreadFunc() {
    while (!should_stop_) {
        performPeriodicChecks();
        std::this_thread::sleep_for(std::chrono::milliseconds(check_interval_ms_));
    }
}

void AntiTamper::performPeriodicChecks() {
    SecurityCheckResult result = performAllChecks();
    if (!result.isSafe()) {
        handleThreatDetected(result.warning_message);
    }
}

void AntiTamper::handleThreatDetected(const std::string& threat) {
    std::lock_guard<std::mutex> lock(callback_mutex_);
    if (tamper_detected_callback_) {
        tamper_detected_callback_(threat);
    }
}

void AntiTamper::setTamperDetectedCallback(TamperDetectedCallback callback) {
    std::lock_guard<std::mutex> lock(callback_mutex_);
    tamper_detected_callback_ = callback;
}

} // namespace client
