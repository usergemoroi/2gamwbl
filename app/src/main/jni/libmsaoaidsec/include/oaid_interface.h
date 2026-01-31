#ifndef LIBMSAOAIDSEC_OAID_INTERFACE_H
#define LIBMSAOAIDSEC_OAID_INTERFACE_H

#include <jni.h>
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

OAIDProvider get_oaid_provider();
const char* get_oaid_provider_name(OAIDProvider provider);
OAIDProvider detect_oaid_provider_from_manufacturer(const char* manufacturer);

OAIDInfo* get_oaid_info(JNIEnv* env, jobject context);
char* get_oaid_by_provider(JNIEnv* env, jobject context, OAIDProvider provider);

char* get_xiaomi_oaid(JNIEnv* env, jobject context);
char* get_huawei_oaid(JNIEnv* env, jobject context);
char* get_samsung_oaid(JNIEnv* env, jobject context);
char* get_oppo_oaid(JNIEnv* env, jobject context);
char* get_vivo_oaid(JNIEnv* env, jobject context);
char* get_oneplus_oaid(JNIEnv* env, jobject context);
char* get_meizu_oaid(JNIEnv* env, jobject context);
char* get_asus_oaid(JNIEnv* env, jobject context);
char* get_lenovo_oaid(JNIEnv* env, jobject context);
char* get_google_advertising_id(JNIEnv* env, jobject context);

bool is_oaid_supported();
bool is_oaid_available(JNIEnv* env, jobject context);
bool is_limit_ad_tracking_enabled(JNIEnv* env, jobject context);

bool check_oaid_service_available(JNIEnv* env, jobject context, const char* package_name);
bool bind_oaid_service(JNIEnv* env, jobject context, const char* package_name);
void unbind_oaid_service(JNIEnv* env, jobject context);

OAIDInfo* get_cached_oaid_info();
void cache_oaid_info(OAIDInfo* info);
void clear_oaid_info_cache();

#ifdef __cplusplus
}
#endif

#endif // LIBMSAOAIDSEC_OAID_INTERFACE_H
