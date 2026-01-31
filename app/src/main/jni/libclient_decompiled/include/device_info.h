#ifndef LIBCLIENT_DEVICE_INFO_H
#define LIBCLIENT_DEVICE_INFO_H

#include "types.h"
#include <string>
#include <map>

namespace client {

class DeviceInfoCollector {
public:
    static DeviceInfo collect();
    
    static std::string getAndroidId();
    static std::string getOAID();
    static std::string getIMEI();
    static std::string getSerialNumber();
    static std::string getMACAddress();
    
    static std::string getManufacturer();
    static std::string getModel();
    static std::string getAndroidVersion();
    static int getSDKVersion();
    
    static std::string getBrand();
    static std::string getDevice();
    static std::string getBoard();
    static std::string getHardware();
    
    static std::string getCPUABI();
    static int getCPUCores();
    static uint64_t getTotalMemory();
    static uint64_t getAvailableMemory();
    
    static std::string getDeviceFingerprint();
    static std::string generateHardwareID();
    
    static std::map<std::string, std::string> getAllProperties();
    
private:
    static std::string readSystemProperty(const char* key);
    static std::string readProcFile(const char* path);
    static bool readMemInfo(uint64_t& total, uint64_t& available);
    
    static std::string getOAIDFromProvider(const std::string& provider);
    static bool checkOAIDService(const std::string& service_name);
    
    static std::string hashString(const std::string& input);
};

} // namespace client

#endif // LIBCLIENT_DEVICE_INFO_H
