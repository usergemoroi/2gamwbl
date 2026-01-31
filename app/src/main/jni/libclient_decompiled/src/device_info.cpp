#include "../include/device_info.h"
#include "../include/internal/platform_specific.h"

namespace client {

DeviceInfo DeviceInfoCollector::collect() {
    DeviceInfo info;
    info.android_id = getAndroidId();
    info.oaid = getOAID();
    info.serial_number = getSerialNumber();
    info.manufacturer = getManufacturer();
    info.model = getModel();
    info.android_version = getAndroidVersion();
    info.sdk_version = getSDKVersion();
    return info;
}

std::string DeviceInfoCollector::getAndroidId() {
    return "0000000000000000"; // Stub - would read from Settings.Secure
}

std::string DeviceInfoCollector::getOAID() {
    return ""; // Stub - would query OAID providers
}

std::string DeviceInfoCollector::getIMEI() {
    return ""; // Deprecated in modern Android
}

std::string DeviceInfoCollector::getSerialNumber() {
    return platform::System::getProperty("ro.serialno");
}

std::string DeviceInfoCollector::getMACAddress() {
    return ""; // Stub - would read from network interfaces
}

std::string DeviceInfoCollector::getManufacturer() {
    return platform::System::getProperty("ro.product.manufacturer");
}

std::string DeviceInfoCollector::getModel() {
    return platform::System::getProperty("ro.product.model");
}

std::string DeviceInfoCollector::getAndroidVersion() {
    return platform::System::getProperty("ro.build.version.release");
}

int DeviceInfoCollector::getSDKVersion() {
    std::string sdk = platform::System::getProperty("ro.build.version.sdk");
    try {
        return std::stoi(sdk);
    } catch (...) {
        return 30; // Default to Android 11
    }
}

std::string DeviceInfoCollector::getBrand() {
    return platform::System::getProperty("ro.product.brand");
}

std::string DeviceInfoCollector::getDevice() {
    return platform::System::getProperty("ro.product.device");
}

std::string DeviceInfoCollector::getBoard() {
    return platform::System::getProperty("ro.product.board");
}

std::string DeviceInfoCollector::getHardware() {
    return platform::System::getProperty("ro.hardware");
}

std::string DeviceInfoCollector::getCPUABI() {
    return platform::System::getArchitecture();
}

int DeviceInfoCollector::getCPUCores() {
    return platform::System::getCPUCount();
}

uint64_t DeviceInfoCollector::getTotalMemory() {
    return platform::System::getTotalMemory();
}

uint64_t DeviceInfoCollector::getAvailableMemory() {
    return platform::System::getAvailableMemory();
}

std::string DeviceInfoCollector::getDeviceFingerprint() {
    return platform::System::getProperty("ro.build.fingerprint");
}

std::string DeviceInfoCollector::generateHardwareID() {
    // Stub - would generate unique hardware ID from device properties
    return "";
}

std::map<std::string, std::string> DeviceInfoCollector::getAllProperties() {
    std::map<std::string, std::string> props;
    props["manufacturer"] = getManufacturer();
    props["model"] = getModel();
    props["brand"] = getBrand();
    props["device"] = getDevice();
    props["android_version"] = getAndroidVersion();
    return props;
}

} // namespace client
