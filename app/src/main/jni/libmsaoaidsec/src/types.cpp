#include "msaoaidsec.h"
#include <stdlib.h>
#include <string.h>

DeviceInfo* create_device_info() {
    DeviceInfo* info = (DeviceInfo*)calloc(1, sizeof(DeviceInfo));
    return info;
}

void free_device_info(DeviceInfo* info) {
    if (!info) return;
    
    safe_free((void**)&info->device_id);
    safe_free((void**)&info->android_id);
    safe_free((void**)&info->oaid);
    safe_free((void**)&info->imei);
    safe_free((void**)&info->serial_number);
    safe_free((void**)&info->manufacturer);
    safe_free((void**)&info->model);
    safe_free((void**)&info->brand);
    safe_free((void**)&info->hardware);
    safe_free((void**)&info->build_id);
    safe_free((void**)&info->fingerprint);
    
    free(info);
}

SecurityCheckResult* create_security_check_result() {
    SecurityCheckResult* result = (SecurityCheckResult*)calloc(1, sizeof(SecurityCheckResult));
    if (result) {
        result->signature_valid = true;
    }
    return result;
}

void free_security_check_result(SecurityCheckResult* result) {
    if (!result) return;
    free(result);
}

OAIDInfo* create_oaid_info() {
    OAIDInfo* info = (OAIDInfo*)calloc(1, sizeof(OAIDInfo));
    return info;
}

void free_oaid_info(OAIDInfo* info) {
    if (!info) return;
    
    safe_free((void**)&info->oaid);
    safe_free((void**)&info->provider_name);
    safe_free((void**)&info->error_message);
    
    free(info);
}
