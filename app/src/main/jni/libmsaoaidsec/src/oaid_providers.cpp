#include "msaoaidsec.h"
#include <string.h>
#include <stdlib.h>

static OAIDInfo* g_cached_oaid_info = nullptr;

OAIDProvider detect_oaid_provider_from_manufacturer(const char* manufacturer) {
    if (!manufacturer) return OAID_PROVIDER_UNKNOWN;
    
    if (strcasecmp(manufacturer, OAID_MANUFACTURER_XIAOMI) == 0) {
        return OAID_PROVIDER_XIAOMI;
    } else if (strcasecmp(manufacturer, OAID_MANUFACTURER_HUAWEI) == 0 ||
               strcasecmp(manufacturer, OAID_MANUFACTURER_HONOR) == 0) {
        return OAID_PROVIDER_HUAWEI;
    } else if (strcasecmp(manufacturer, OAID_MANUFACTURER_SAMSUNG) == 0) {
        return OAID_PROVIDER_SAMSUNG;
    } else if (strcasecmp(manufacturer, OAID_MANUFACTURER_OPPO) == 0) {
        return OAID_PROVIDER_OPPO;
    } else if (strcasecmp(manufacturer, OAID_MANUFACTURER_VIVO) == 0) {
        return OAID_PROVIDER_VIVO;
    } else if (strcasecmp(manufacturer, OAID_MANUFACTURER_ONEPLUS) == 0) {
        return OAID_PROVIDER_ONEPLUS;
    } else if (strcasecmp(manufacturer, OAID_MANUFACTURER_MEIZU) == 0) {
        return OAID_PROVIDER_MEIZU;
    } else if (strcasecmp(manufacturer, OAID_MANUFACTURER_ASUS) == 0) {
        return OAID_PROVIDER_ASUS;
    } else if (strcasecmp(manufacturer, OAID_MANUFACTURER_LENOVO) == 0) {
        return OAID_PROVIDER_LENOVO;
    }
    
    return OAID_PROVIDER_GOOGLE;
}

OAIDProvider get_oaid_provider() {
    char* manufacturer = get_manufacturer();
    if (!manufacturer) return OAID_PROVIDER_UNKNOWN;
    
    OAIDProvider provider = detect_oaid_provider_from_manufacturer(manufacturer);
    free(manufacturer);
    
    return provider;
}

const char* get_oaid_provider_name(OAIDProvider provider) {
    switch (provider) {
        case OAID_PROVIDER_XIAOMI: return "Xiaomi";
        case OAID_PROVIDER_HUAWEI: return "Huawei";
        case OAID_PROVIDER_SAMSUNG: return "Samsung";
        case OAID_PROVIDER_OPPO: return "Oppo";
        case OAID_PROVIDER_VIVO: return "Vivo";
        case OAID_PROVIDER_ONEPLUS: return "OnePlus";
        case OAID_PROVIDER_MEIZU: return "Meizu";
        case OAID_PROVIDER_ASUS: return "Asus";
        case OAID_PROVIDER_LENOVO: return "Lenovo";
        case OAID_PROVIDER_GOOGLE: return "Google";
        default: return "Unknown";
    }
}

char* get_xiaomi_oaid(JNIEnv* env, jobject context) {
    if (!env || !context) return nullptr;
    
    jclass id_provider_class = find_class_safe(env, "com/android/id/impl/IdProviderImpl");
    if (!id_provider_class) {
        log_debug(LOG_TAG, "Xiaomi OAID service not available");
        return nullptr;
    }
    
    jmethodID get_oaid_method = get_static_method_id_safe(env, id_provider_class,
                                                           "getOAID",
                                                           "(Landroid/content/Context;)Ljava/lang/String;");
    if (!get_oaid_method) {
        env->DeleteLocalRef(id_provider_class);
        return nullptr;
    }
    
    jstring oaid_jstr = (jstring)env->CallStaticObjectMethod(id_provider_class, get_oaid_method, context);
    env->DeleteLocalRef(id_provider_class);
    
    if (!oaid_jstr) return nullptr;
    
    char* oaid = jstring_to_string(env, oaid_jstr);
    env->DeleteLocalRef(oaid_jstr);
    
    log_info(LOG_TAG, "Retrieved Xiaomi OAID: %s", oaid ? oaid : "null");
    return oaid;
}

char* get_huawei_oaid(JNIEnv* env, jobject context) {
    if (!env || !context) return nullptr;
    
    jclass oaid_client_class = find_class_safe(env, "com/huawei/hms/ads/identifier/AdvertisingIdClient");
    if (!oaid_client_class) {
        log_debug(LOG_TAG, "Huawei OAID service not available");
        return nullptr;
    }
    
    jmethodID get_advertising_id_info = get_static_method_id_safe(env, oaid_client_class,
                                                                   "getAdvertisingIdInfo",
                                                                   "(Landroid/content/Context;)Lcom/huawei/hms/ads/identifier/AdvertisingIdClient$Info;");
    if (!get_advertising_id_info) {
        env->DeleteLocalRef(oaid_client_class);
        return nullptr;
    }
    
    jobject info = env->CallStaticObjectMethod(oaid_client_class, get_advertising_id_info, context);
    if (!info || check_jni_exception(env)) {
        clear_jni_exception(env);
        env->DeleteLocalRef(oaid_client_class);
        return nullptr;
    }
    
    jclass info_class = env->GetObjectClass(info);
    jmethodID get_id = get_method_id_safe(env, info_class, "getId", "()Ljava/lang/String;");
    
    if (!get_id) {
        env->DeleteLocalRef(info);
        env->DeleteLocalRef(info_class);
        env->DeleteLocalRef(oaid_client_class);
        return nullptr;
    }
    
    jstring oaid_jstr = (jstring)env->CallObjectMethod(info, get_id);
    env->DeleteLocalRef(info);
    env->DeleteLocalRef(info_class);
    env->DeleteLocalRef(oaid_client_class);
    
    if (!oaid_jstr) return nullptr;
    
    char* oaid = jstring_to_string(env, oaid_jstr);
    env->DeleteLocalRef(oaid_jstr);
    
    log_info(LOG_TAG, "Retrieved Huawei OAID: %s", oaid ? oaid : "null");
    return oaid;
}

char* get_samsung_oaid(JNIEnv* env, jobject context) {
    if (!env || !context) return nullptr;
    log_debug(LOG_TAG, "Samsung OAID not implemented yet");
    return nullptr;
}

char* get_oppo_oaid(JNIEnv* env, jobject context) {
    if (!env || !context) return nullptr;
    
    jclass oaid_manager_class = find_class_safe(env, "com/heytap/openid/sdk/IdentifierManager");
    if (!oaid_manager_class) {
        log_debug(LOG_TAG, "Oppo OAID service not available");
        return nullptr;
    }
    
    jmethodID get_oaid_method = get_static_method_id_safe(env, oaid_manager_class,
                                                           "getOAID",
                                                           "(Landroid/content/Context;)Ljava/lang/String;");
    if (!get_oaid_method) {
        env->DeleteLocalRef(oaid_manager_class);
        return nullptr;
    }
    
    jstring oaid_jstr = (jstring)env->CallStaticObjectMethod(oaid_manager_class, get_oaid_method, context);
    env->DeleteLocalRef(oaid_manager_class);
    
    if (!oaid_jstr) return nullptr;
    
    char* oaid = jstring_to_string(env, oaid_jstr);
    env->DeleteLocalRef(oaid_jstr);
    
    log_info(LOG_TAG, "Retrieved Oppo OAID: %s", oaid ? oaid : "null");
    return oaid;
}

char* get_vivo_oaid(JNIEnv* env, jobject context) {
    if (!env || !context) return nullptr;
    
    jclass oaid_manager_class = find_class_safe(env, "com/vivo/identifier/IdentifierManager");
    if (!oaid_manager_class) {
        log_debug(LOG_TAG, "Vivo OAID service not available");
        return nullptr;
    }
    
    jmethodID get_oaid_method = get_static_method_id_safe(env, oaid_manager_class,
                                                           "getOAID",
                                                           "(Landroid/content/Context;)Ljava/lang/String;");
    if (!get_oaid_method) {
        env->DeleteLocalRef(oaid_manager_class);
        return nullptr;
    }
    
    jstring oaid_jstr = (jstring)env->CallStaticObjectMethod(oaid_manager_class, get_oaid_method, context);
    env->DeleteLocalRef(oaid_manager_class);
    
    if (!oaid_jstr) return nullptr;
    
    char* oaid = jstring_to_string(env, oaid_jstr);
    env->DeleteLocalRef(oaid_jstr);
    
    log_info(LOG_TAG, "Retrieved Vivo OAID: %s", oaid ? oaid : "null");
    return oaid;
}

char* get_oneplus_oaid(JNIEnv* env, jobject context) {
    return get_oppo_oaid(env, context);
}

char* get_meizu_oaid(JNIEnv* env, jobject context) {
    if (!env || !context) return nullptr;
    log_debug(LOG_TAG, "Meizu OAID not implemented yet");
    return nullptr;
}

char* get_asus_oaid(JNIEnv* env, jobject context) {
    if (!env || !context) return nullptr;
    log_debug(LOG_TAG, "Asus OAID not implemented yet");
    return nullptr;
}

char* get_lenovo_oaid(JNIEnv* env, jobject context) {
    if (!env || !context) return nullptr;
    log_debug(LOG_TAG, "Lenovo OAID not implemented yet");
    return nullptr;
}

char* get_google_advertising_id(JNIEnv* env, jobject context) {
    if (!env || !context) return nullptr;
    
    jclass advertising_id_client_class = find_class_safe(env, "com/google/android/gms/ads/identifier/AdvertisingIdClient");
    if (!advertising_id_client_class) {
        log_debug(LOG_TAG, "Google Advertising ID service not available");
        return nullptr;
    }
    
    jmethodID get_advertising_id_info = get_static_method_id_safe(env, advertising_id_client_class,
                                                                   "getAdvertisingIdInfo",
                                                                   "(Landroid/content/Context;)Lcom/google/android/gms/ads/identifier/AdvertisingIdClient$Info;");
    if (!get_advertising_id_info) {
        env->DeleteLocalRef(advertising_id_client_class);
        return nullptr;
    }
    
    jobject info = env->CallStaticObjectMethod(advertising_id_client_class, get_advertising_id_info, context);
    if (!info || check_jni_exception(env)) {
        clear_jni_exception(env);
        env->DeleteLocalRef(advertising_id_client_class);
        return nullptr;
    }
    
    jclass info_class = env->GetObjectClass(info);
    jmethodID get_id = get_method_id_safe(env, info_class, "getId", "()Ljava/lang/String;");
    
    if (!get_id) {
        env->DeleteLocalRef(info);
        env->DeleteLocalRef(info_class);
        env->DeleteLocalRef(advertising_id_client_class);
        return nullptr;
    }
    
    jstring ad_id_jstr = (jstring)env->CallObjectMethod(info, get_id);
    env->DeleteLocalRef(info);
    env->DeleteLocalRef(info_class);
    env->DeleteLocalRef(advertising_id_client_class);
    
    if (!ad_id_jstr) return nullptr;
    
    char* ad_id = jstring_to_string(env, ad_id_jstr);
    env->DeleteLocalRef(ad_id_jstr);
    
    log_info(LOG_TAG, "Retrieved Google Advertising ID: %s", ad_id ? ad_id : "null");
    return ad_id;
}

char* get_oaid_by_provider(JNIEnv* env, jobject context, OAIDProvider provider) {
    switch (provider) {
        case OAID_PROVIDER_XIAOMI:
            return get_xiaomi_oaid(env, context);
        case OAID_PROVIDER_HUAWEI:
            return get_huawei_oaid(env, context);
        case OAID_PROVIDER_SAMSUNG:
            return get_samsung_oaid(env, context);
        case OAID_PROVIDER_OPPO:
            return get_oppo_oaid(env, context);
        case OAID_PROVIDER_VIVO:
            return get_vivo_oaid(env, context);
        case OAID_PROVIDER_ONEPLUS:
            return get_oneplus_oaid(env, context);
        case OAID_PROVIDER_MEIZU:
            return get_meizu_oaid(env, context);
        case OAID_PROVIDER_ASUS:
            return get_asus_oaid(env, context);
        case OAID_PROVIDER_LENOVO:
            return get_lenovo_oaid(env, context);
        case OAID_PROVIDER_GOOGLE:
            return get_google_advertising_id(env, context);
        default:
            return nullptr;
    }
}

OAIDInfo* get_oaid_info(JNIEnv* env, jobject context) {
    OAIDInfo* info = create_oaid_info();
    if (!info) return nullptr;
    
    OAIDProvider provider = get_oaid_provider();
    info->provider_name = safe_strdup(get_oaid_provider_name(provider));
    
    info->oaid = get_oaid_by_provider(env, context, provider);
    info->is_available = info->oaid != nullptr;
    info->is_limit_tracking = false;
    info->error_code = info->is_available ? ERROR_NONE : ERROR_OAID_NOT_AVAILABLE;
    
    if (!info->is_available) {
        log_warn(LOG_TAG, "OAID not available for provider: %s", info->provider_name);
        info->error_message = safe_strdup("OAID not available");
    }
    
    return info;
}

bool is_oaid_supported() {
    OAIDProvider provider = get_oaid_provider();
    return provider != OAID_PROVIDER_UNKNOWN;
}

bool is_oaid_available(JNIEnv* env, jobject context) {
    OAIDInfo* info = get_oaid_info(env, context);
    if (!info) return false;
    
    bool available = info->is_available;
    free_oaid_info(info);
    
    return available;
}

bool is_limit_ad_tracking_enabled(JNIEnv* env, jobject context) {
    return false;
}

bool check_oaid_service_available(JNIEnv* env, jobject context, const char* package_name) {
    if (!env || !context || !package_name) return false;
    
    jclass context_class = env->GetObjectClass(context);
    jmethodID get_package_manager = get_method_id_safe(env, context_class,
                                                        "getPackageManager",
                                                        "()Landroid/content/pm/PackageManager;");
    if (!get_package_manager) {
        env->DeleteLocalRef(context_class);
        return false;
    }
    
    jobject package_manager = env->CallObjectMethod(context, get_package_manager);
    if (!package_manager) {
        env->DeleteLocalRef(context_class);
        return false;
    }
    
    jclass package_manager_class = env->GetObjectClass(package_manager);
    jmethodID get_package_info = get_method_id_safe(env, package_manager_class,
                                                     "getPackageInfo",
                                                     "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    
    if (!get_package_info) {
        env->DeleteLocalRef(package_manager);
        env->DeleteLocalRef(package_manager_class);
        env->DeleteLocalRef(context_class);
        return false;
    }
    
    jstring package_name_jstr = env->NewStringUTF(package_name);
    jobject package_info = env->CallObjectMethod(package_manager, get_package_info, package_name_jstr, 0);
    
    env->DeleteLocalRef(package_name_jstr);
    env->DeleteLocalRef(package_manager);
    env->DeleteLocalRef(package_manager_class);
    env->DeleteLocalRef(context_class);
    
    bool available = package_info != nullptr && !check_jni_exception(env);
    
    if (package_info) {
        env->DeleteLocalRef(package_info);
    }
    
    clear_jni_exception(env);
    
    return available;
}

bool bind_oaid_service(JNIEnv* env, jobject context, const char* package_name) {
    return false;
}

void unbind_oaid_service(JNIEnv* env, jobject context) {
}

OAIDInfo* get_cached_oaid_info() {
    return g_cached_oaid_info;
}

void cache_oaid_info(OAIDInfo* info) {
    if (g_cached_oaid_info) {
        free_oaid_info(g_cached_oaid_info);
    }
    g_cached_oaid_info = info;
}

void clear_oaid_info_cache() {
    if (g_cached_oaid_info) {
        free_oaid_info(g_cached_oaid_info);
        g_cached_oaid_info = nullptr;
    }
}
