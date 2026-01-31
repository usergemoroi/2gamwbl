#ifndef LIBMSAOAIDSEC_TYPES_H
#define LIBMSAOAIDSEC_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DeviceInfo {
    char* device_id;
    char* android_id;
    char* oaid;
    char* imei;
    char* serial_number;
    char* manufacturer;
    char* model;
    char* brand;
    char* hardware;
    char* build_id;
    char* fingerprint;
    int32_t sdk_version;
    int32_t api_level;
    bool is_rooted;
    bool is_emulator;
    bool is_debuggable;
} DeviceInfo;

typedef struct SecurityCheckResult {
    bool debugger_detected;
    bool root_detected;
    bool emulator_detected;
    bool xposed_detected;
    bool frida_detected;
    bool signature_valid;
    bool memory_tampered;
    int32_t check_count;
    uint64_t timestamp;
} SecurityCheckResult;

typedef struct OAIDInfo {
    char* oaid;
    char* provider_name;
    bool is_available;
    bool is_limit_tracking;
    int32_t error_code;
    char* error_message;
} OAIDInfo;

typedef enum SecurityCheck {
    SECURITY_CHECK_DEBUGGER = 0x01,
    SECURITY_CHECK_ROOT = 0x02,
    SECURITY_CHECK_EMULATOR = 0x04,
    SECURITY_CHECK_XPOSED = 0x08,
    SECURITY_CHECK_FRIDA = 0x10,
    SECURITY_CHECK_SIGNATURE = 0x20,
    SECURITY_CHECK_MEMORY = 0x40,
    SECURITY_CHECK_ALL = 0xFF
} SecurityCheck;

typedef enum OAIDProvider {
    OAID_PROVIDER_XIAOMI = 1,
    OAID_PROVIDER_HUAWEI = 2,
    OAID_PROVIDER_SAMSUNG = 3,
    OAID_PROVIDER_OPPO = 4,
    OAID_PROVIDER_VIVO = 5,
    OAID_PROVIDER_ONEPLUS = 6,
    OAID_PROVIDER_MEIZU = 7,
    OAID_PROVIDER_ASUS = 8,
    OAID_PROVIDER_LENOVO = 9,
    OAID_PROVIDER_GOOGLE = 10,
    OAID_PROVIDER_UNKNOWN = 0
} OAIDProvider;

typedef enum ErrorCode {
    ERROR_NONE = 0,
    ERROR_INIT_FAILED = -1,
    ERROR_NOT_SUPPORTED = -2,
    ERROR_PERMISSION_DENIED = -3,
    ERROR_INVALID_ARGUMENT = -4,
    ERROR_OUT_OF_MEMORY = -5,
    ERROR_OAID_NOT_AVAILABLE = -6,
    ERROR_JNI_EXCEPTION = -7,
    ERROR_UNKNOWN = -99
} ErrorCode;

typedef struct LibraryState {
    bool initialized;
    void* jvm;
    void* context;
    int32_t init_count;
    SecurityCheckResult last_check;
    DeviceInfo* cached_device_info;
    OAIDInfo* cached_oaid_info;
} LibraryState;

void free_device_info(DeviceInfo* info);
void free_security_check_result(SecurityCheckResult* result);
void free_oaid_info(OAIDInfo* info);

DeviceInfo* create_device_info();
SecurityCheckResult* create_security_check_result();
OAIDInfo* create_oaid_info();

#ifdef __cplusplus
}
#endif

#endif // LIBMSAOAIDSEC_TYPES_H
