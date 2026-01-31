#ifndef LIBMSAOAIDSEC_DEVICE_ID_H
#define LIBMSAOAIDSEC_DEVICE_ID_H

#include <jni.h>
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

char* get_device_id(JNIEnv* env, jobject context);
char* get_android_id(JNIEnv* env, jobject context);
char* get_oaid(JNIEnv* env, jobject context);
char* get_imei(JNIEnv* env, jobject context);
char* get_serial_number(JNIEnv* env, jobject context);

char* get_manufacturer();
char* get_model();
char* get_brand();
char* get_hardware();
char* get_device();
char* get_product();
char* get_fingerprint();
char* get_build_id();
int32_t get_sdk_version();
int32_t get_api_level();

DeviceInfo* get_full_device_info(JNIEnv* env, jobject context);
DeviceInfo* get_cached_device_info();
void cache_device_info(DeviceInfo* info);
void clear_device_info_cache();

char* generate_device_fingerprint(DeviceInfo* info);
char* generate_device_hash(DeviceInfo* info);

bool is_device_id_valid(const char* device_id);
bool is_android_id_valid(const char* android_id);
bool is_oaid_valid(const char* oaid);
bool is_imei_valid(const char* imei);

#ifdef __cplusplus
}
#endif

#endif // LIBMSAOAIDSEC_DEVICE_ID_H
