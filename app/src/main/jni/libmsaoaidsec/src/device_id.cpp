#include "msaoaidsec.h"
#include <string.h>
#include <stdlib.h>

static DeviceInfo* g_cached_device_info = nullptr;

char* get_android_id(JNIEnv* env, jobject context) {
    if (!env || !context) return nullptr;
    
    jclass settings_secure_class = find_class_safe(env, SETTINGS_SECURE_CLASS);
    if (!settings_secure_class) {
        log_error(LOG_TAG, "Failed to find Settings.Secure class");
        return nullptr;
    }
    
    jmethodID get_string_method = get_static_method_id_safe(env, settings_secure_class,
                                                             JAVA_METHOD_GET_STRING,
                                                             "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;");
    if (!get_string_method) {
        log_error(LOG_TAG, "Failed to find getString method");
        env->DeleteLocalRef(settings_secure_class);
        return nullptr;
    }
    
    jclass context_class = env->GetObjectClass(context);
    jmethodID get_content_resolver = get_method_id_safe(env, context_class,
                                                         "getContentResolver",
                                                         "()Landroid/content/ContentResolver;");
    
    if (!get_content_resolver) {
        log_error(LOG_TAG, "Failed to find getContentResolver method");
        env->DeleteLocalRef(settings_secure_class);
        env->DeleteLocalRef(context_class);
        return nullptr;
    }
    
    jobject content_resolver = env->CallObjectMethod(context, get_content_resolver);
    if (!content_resolver) {
        log_error(LOG_TAG, "Failed to get ContentResolver");
        env->DeleteLocalRef(settings_secure_class);
        env->DeleteLocalRef(context_class);
        return nullptr;
    }
    
    jstring android_id_key = env->NewStringUTF(ANDROID_ID_SECURE_SETTINGS);
    jstring android_id_jstr = (jstring)env->CallStaticObjectMethod(settings_secure_class,
                                                                     get_string_method,
                                                                     content_resolver,
                                                                     android_id_key);
    
    env->DeleteLocalRef(android_id_key);
    env->DeleteLocalRef(content_resolver);
    env->DeleteLocalRef(context_class);
    env->DeleteLocalRef(settings_secure_class);
    
    if (!android_id_jstr) {
        log_error(LOG_TAG, "Failed to get Android ID");
        return nullptr;
    }
    
    char* android_id = jstring_to_string(env, android_id_jstr);
    env->DeleteLocalRef(android_id_jstr);
    
    log_info(LOG_TAG, "Retrieved Android ID: %s", android_id ? android_id : "null");
    return android_id;
}

char* get_device_id(JNIEnv* env, jobject context) {
    char* android_id = get_android_id(env, context);
    if (android_id) return android_id;
    
    char* oaid = get_oaid(env, context);
    if (oaid) return oaid;
    
    return safe_strdup("unknown");
}

char* get_oaid(JNIEnv* env, jobject context) {
    OAIDInfo* info = get_oaid_info(env, context);
    if (!info) return nullptr;
    
    char* oaid = safe_strdup(info->oaid);
    free_oaid_info(info);
    
    return oaid;
}

char* get_imei(JNIEnv* env, jobject context) {
    return nullptr;
}

char* get_serial_number(JNIEnv* env, jobject context) {
    jclass build_class = find_class_safe(env, JAVA_CLASS_BUILD);
    if (!build_class) return nullptr;
    
    jfieldID serial_field = get_static_field_id_safe(env, build_class, "SERIAL", "Ljava/lang/String;");
    if (!serial_field) {
        env->DeleteLocalRef(build_class);
        return nullptr;
    }
    
    jstring serial_jstr = (jstring)env->GetStaticObjectField(build_class, serial_field);
    env->DeleteLocalRef(build_class);
    
    if (!serial_jstr) return nullptr;
    
    char* serial = jstring_to_string(env, serial_jstr);
    env->DeleteLocalRef(serial_jstr);
    
    return serial;
}

char* get_manufacturer() {
    return get_system_property("ro.product.manufacturer");
}

char* get_model() {
    return get_system_property("ro.product.model");
}

char* get_brand() {
    return get_system_property("ro.product.brand");
}

char* get_hardware() {
    return get_system_property("ro.hardware");
}

char* get_device() {
    return get_system_property("ro.product.device");
}

char* get_product() {
    return get_system_property("ro.product.name");
}

char* get_fingerprint() {
    return get_system_property("ro.build.fingerprint");
}

char* get_build_id() {
    return get_system_property("ro.build.id");
}

int32_t get_sdk_version() {
    char* sdk_str = get_system_property("ro.build.version.sdk");
    if (!sdk_str) return 0;
    
    int32_t sdk = atoi(sdk_str);
    free(sdk_str);
    
    return sdk;
}

int32_t get_api_level() {
    return get_sdk_version();
}

DeviceInfo* get_full_device_info(JNIEnv* env, jobject context) {
    DeviceInfo* info = create_device_info();
    if (!info) return nullptr;
    
    info->device_id = get_device_id(env, context);
    info->android_id = get_android_id(env, context);
    info->oaid = get_oaid(env, context);
    info->imei = get_imei(env, context);
    info->serial_number = get_serial_number(env, context);
    
    info->manufacturer = get_manufacturer();
    info->model = get_model();
    info->brand = get_brand();
    info->hardware = get_hardware();
    info->build_id = get_build_id();
    info->fingerprint = get_fingerprint();
    
    info->sdk_version = get_sdk_version();
    info->api_level = get_api_level();
    
    info->is_rooted = detect_root();
    info->is_emulator = detect_emulator();
    info->is_debuggable = detect_debugger();
    
    log_info(LOG_TAG, "Device info collected:");
    log_info(LOG_TAG, "  Device ID: %s", info->device_id ? info->device_id : "null");
    log_info(LOG_TAG, "  Android ID: %s", info->android_id ? info->android_id : "null");
    log_info(LOG_TAG, "  OAID: %s", info->oaid ? info->oaid : "null");
    log_info(LOG_TAG, "  Manufacturer: %s", info->manufacturer ? info->manufacturer : "null");
    log_info(LOG_TAG, "  Model: %s", info->model ? info->model : "null");
    log_info(LOG_TAG, "  SDK: %d", info->sdk_version);
    
    return info;
}

DeviceInfo* get_cached_device_info() {
    return g_cached_device_info;
}

void cache_device_info(DeviceInfo* info) {
    if (g_cached_device_info) {
        free_device_info(g_cached_device_info);
    }
    g_cached_device_info = info;
}

void clear_device_info_cache() {
    if (g_cached_device_info) {
        free_device_info(g_cached_device_info);
        g_cached_device_info = nullptr;
    }
}

char* generate_device_fingerprint(DeviceInfo* info) {
    if (!info) return nullptr;
    
    char buffer[MAX_STRING_LENGTH * 4];
    snprintf(buffer, sizeof(buffer), "%s|%s|%s|%s|%s|%d",
             info->manufacturer ? info->manufacturer : "",
             info->model ? info->model : "",
             info->brand ? info->brand : "",
             info->hardware ? info->hardware : "",
             info->fingerprint ? info->fingerprint : "",
             info->sdk_version);
    
    return strdup(buffer);
}

char* generate_device_hash(DeviceInfo* info) {
    if (!info) return nullptr;
    
    char* fingerprint = generate_device_fingerprint(info);
    if (!fingerprint) return nullptr;
    
    uint32_t hash = hash_string(fingerprint);
    free(fingerprint);
    
    char hash_str[32];
    snprintf(hash_str, sizeof(hash_str), "%08x", hash);
    
    return strdup(hash_str);
}

bool is_device_id_valid(const char* device_id) {
    if (!device_id || strlen(device_id) == 0) return false;
    if (strcmp(device_id, "unknown") == 0) return false;
    if (strcmp(device_id, "0") == 0) return false;
    return true;
}

bool is_android_id_valid(const char* android_id) {
    if (!android_id || strlen(android_id) == 0) return false;
    if (strcmp(android_id, "9774d56d682e549c") == 0) return false;
    return true;
}

bool is_oaid_valid(const char* oaid) {
    if (!oaid || strlen(oaid) == 0) return false;
    if (strcmp(oaid, "00000000-0000-0000-0000-000000000000") == 0) return false;
    return true;
}

bool is_imei_valid(const char* imei) {
    if (!imei || strlen(imei) == 0) return false;
    if (strlen(imei) < 14 || strlen(imei) > 15) return false;
    
    for (size_t i = 0; i < strlen(imei); i++) {
        if (!isdigit(imei[i])) return false;
    }
    
    return true;
}
