#include "msaoaidsec.h"

static JavaVM* g_jvm = nullptr;

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    g_jvm = vm;
    log_info(LOG_TAG, "libmsaoaidsec v%s loaded", MSAOAIDSEC_VERSION);
    
    JNIEnv* env;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        log_error(LOG_TAG, "Failed to get JNIEnv");
        return JNI_ERR;
    }
    
    init_anti_debugging();
    init_anti_tampering();
    
    log_info(LOG_TAG, "Security checks initialized");
    
    return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM* vm, void* reserved) {
    log_info(LOG_TAG, "libmsaoaidsec unloading");
    msaoaidsec_cleanup();
    g_jvm = nullptr;
}

jobject get_application_context(JNIEnv* env) {
    if (!env) return nullptr;
    
    jclass activity_thread_class = find_class_safe(env, JAVA_CLASS_ACTIVITY_THREAD);
    if (!activity_thread_class) {
        log_error(LOG_TAG, "Failed to find ActivityThread class");
        return nullptr;
    }
    
    jmethodID current_activity_thread = get_static_method_id_safe(env, activity_thread_class,
                                                                   JAVA_METHOD_CURRENT_APPLICATION_THREAD,
                                                                   JAVA_SIGNATURE_CURRENT_ACTIVITY_THREAD);
    if (!current_activity_thread) {
        log_error(LOG_TAG, "Failed to find currentActivityThread method");
        env->DeleteLocalRef(activity_thread_class);
        return nullptr;
    }
    
    jobject activity_thread = env->CallStaticObjectMethod(activity_thread_class, current_activity_thread);
    if (!activity_thread) {
        log_error(LOG_TAG, "Failed to get ActivityThread instance");
        env->DeleteLocalRef(activity_thread_class);
        return nullptr;
    }
    
    jmethodID get_application = get_method_id_safe(env, activity_thread_class,
                                                    JAVA_METHOD_GET_APPLICATION,
                                                    JAVA_SIGNATURE_GET_APPLICATION);
    if (!get_application) {
        log_error(LOG_TAG, "Failed to find getApplication method");
        env->DeleteLocalRef(activity_thread);
        env->DeleteLocalRef(activity_thread_class);
        return nullptr;
    }
    
    jobject application = env->CallObjectMethod(activity_thread, get_application);
    
    env->DeleteLocalRef(activity_thread);
    env->DeleteLocalRef(activity_thread_class);
    
    return application;
}

jobject get_current_activity_thread(JNIEnv* env) {
    if (!env) return nullptr;
    
    jclass activity_thread_class = find_class_safe(env, JAVA_CLASS_ACTIVITY_THREAD);
    if (!activity_thread_class) return nullptr;
    
    jmethodID current_activity_thread = get_static_method_id_safe(env, activity_thread_class,
                                                                   JAVA_METHOD_CURRENT_APPLICATION_THREAD,
                                                                   JAVA_SIGNATURE_CURRENT_ACTIVITY_THREAD);
    if (!current_activity_thread) {
        env->DeleteLocalRef(activity_thread_class);
        return nullptr;
    }
    
    jobject activity_thread = env->CallStaticObjectMethod(activity_thread_class, current_activity_thread);
    env->DeleteLocalRef(activity_thread_class);
    
    return activity_thread;
}

JNI_METHOD(jstring, DeviceInfo_getDeviceId)(JNIEnv* env, jobject thiz, jobject context) {
    if (!context) {
        context = get_application_context(env);
    }
    
    char* device_id = get_device_id(env, context);
    if (!device_id) return nullptr;
    
    jstring result = string_to_jstring(env, device_id);
    free(device_id);
    
    return result;
}

JNI_METHOD(jstring, DeviceInfo_getAndroidId)(JNIEnv* env, jobject thiz, jobject context) {
    if (!context) {
        context = get_application_context(env);
    }
    
    char* android_id = get_android_id(env, context);
    if (!android_id) return nullptr;
    
    jstring result = string_to_jstring(env, android_id);
    free(android_id);
    
    return result;
}

JNI_METHOD(jstring, DeviceInfo_getOAID)(JNIEnv* env, jobject thiz, jobject context) {
    if (!context) {
        context = get_application_context(env);
    }
    
    char* oaid = get_oaid(env, context);
    if (!oaid) return nullptr;
    
    jstring result = string_to_jstring(env, oaid);
    free(oaid);
    
    return result;
}

JNI_METHOD(jstring, DeviceInfo_getIMEI)(JNIEnv* env, jobject thiz, jobject context) {
    if (!context) {
        context = get_application_context(env);
    }
    
    char* imei = get_imei(env, context);
    if (!imei) return nullptr;
    
    jstring result = string_to_jstring(env, imei);
    free(imei);
    
    return result;
}

JNI_METHOD(jstring, DeviceInfo_getSerial)(JNIEnv* env, jobject thiz, jobject context) {
    if (!context) {
        context = get_application_context(env);
    }
    
    char* serial = get_serial_number(env, context);
    if (!serial) return nullptr;
    
    jstring result = string_to_jstring(env, serial);
    free(serial);
    
    return result;
}

JNI_METHOD(jstring, DeviceInfo_getManufacturer)(JNIEnv* env, jobject thiz) {
    char* manufacturer = get_manufacturer();
    if (!manufacturer) return nullptr;
    
    jstring result = string_to_jstring(env, manufacturer);
    free(manufacturer);
    
    return result;
}

JNI_METHOD(jstring, DeviceInfo_getModel)(JNIEnv* env, jobject thiz) {
    char* model = get_model();
    if (!model) return nullptr;
    
    jstring result = string_to_jstring(env, model);
    free(model);
    
    return result;
}

JNI_METHOD(jstring, DeviceInfo_getBrand)(JNIEnv* env, jobject thiz) {
    char* brand = get_brand();
    if (!brand) return nullptr;
    
    jstring result = string_to_jstring(env, brand);
    free(brand);
    
    return result;
}

JNI_METHOD(jstring, DeviceInfo_getHardware)(JNIEnv* env, jobject thiz) {
    char* hardware = get_hardware();
    if (!hardware) return nullptr;
    
    jstring result = string_to_jstring(env, hardware);
    free(hardware);
    
    return result;
}

JNI_METHOD(jstring, DeviceInfo_getDevice)(JNIEnv* env, jobject thiz) {
    char* device = get_device();
    if (!device) return nullptr;
    
    jstring result = string_to_jstring(env, device);
    free(device);
    
    return result;
}

JNI_METHOD(jstring, DeviceInfo_getFingerprint)(JNIEnv* env, jobject thiz) {
    char* fingerprint = get_fingerprint();
    if (!fingerprint) return nullptr;
    
    jstring result = string_to_jstring(env, fingerprint);
    free(fingerprint);
    
    return result;
}

JNI_METHOD(jint, DeviceInfo_getSDKVersion)(JNIEnv* env, jobject thiz) {
    return get_sdk_version();
}

JNI_METHOD(jboolean, SecurityCheck_checkDebugger)(JNIEnv* env, jobject thiz) {
    return detect_debugger() ? JNI_TRUE : JNI_FALSE;
}

JNI_METHOD(jboolean, SecurityCheck_checkRoot)(JNIEnv* env, jobject thiz) {
    return detect_root() ? JNI_TRUE : JNI_FALSE;
}

JNI_METHOD(jboolean, SecurityCheck_checkEmulator)(JNIEnv* env, jobject thiz) {
    return detect_emulator() ? JNI_TRUE : JNI_FALSE;
}

JNI_METHOD(jboolean, SecurityCheck_checkXposed)(JNIEnv* env, jobject thiz) {
    return detect_xposed() ? JNI_TRUE : JNI_FALSE;
}

JNI_METHOD(jboolean, SecurityCheck_checkFrida)(JNIEnv* env, jobject thiz) {
    return detect_frida() ? JNI_TRUE : JNI_FALSE;
}

JNI_METHOD(jboolean, SecurityCheck_verifySignature)(JNIEnv* env, jobject thiz, jobject context) {
    if (!context) {
        context = get_application_context(env);
    }
    
    return verify_apk_signature(env, context) ? JNI_TRUE : JNI_FALSE;
}

JNI_METHOD(jint, SecurityCheck_performAllChecks)(JNIEnv* env, jobject thiz, jobject context) {
    if (!context) {
        context = get_application_context(env);
    }
    
    SecurityCheckResult* result = perform_full_security_check(env, context);
    if (!result) return 0;
    
    int32_t level = get_security_level(result);
    free_security_check_result(result);
    
    return level;
}

JNI_METHOD(jstring, OAIDHelper_getOAID)(JNIEnv* env, jobject thiz, jobject context) {
    if (!context) {
        context = get_application_context(env);
    }
    
    char* oaid = get_oaid(env, context);
    if (!oaid) return nullptr;
    
    jstring result = string_to_jstring(env, oaid);
    free(oaid);
    
    return result;
}

JNI_METHOD(jint, OAIDHelper_getOAIDProvider)(JNIEnv* env, jobject thiz) {
    return (jint)get_oaid_provider();
}

JNI_METHOD(jboolean, OAIDHelper_isOAIDSupported)(JNIEnv* env, jobject thiz) {
    return is_oaid_supported() ? JNI_TRUE : JNI_FALSE;
}

JNI_METHOD(jboolean, OAIDHelper_isOAIDAvailable)(JNIEnv* env, jobject thiz, jobject context) {
    if (!context) {
        context = get_application_context(env);
    }
    
    return is_oaid_available(env, context) ? JNI_TRUE : JNI_FALSE;
}

JNI_METHOD(jstring, OAIDHelper_getProviderName)(JNIEnv* env, jobject thiz, jint provider) {
    const char* name = get_oaid_provider_name((OAIDProvider)provider);
    return string_to_jstring(env, name);
}

JNI_METHOD(jint, Library_init)(JNIEnv* env, jobject thiz, jobject context) {
    return msaoaidsec_init(env, context);
}

JNI_METHOD(void, Library_cleanup)(JNIEnv* env, jobject thiz) {
    msaoaidsec_cleanup();
}

JNI_METHOD(jstring, Library_getVersion)(JNIEnv* env, jobject thiz) {
    return string_to_jstring(env, msaoaidsec_get_version());
}

JNI_METHOD(jboolean, Library_isInitialized)(JNIEnv* env, jobject thiz) {
    return msaoaidsec_is_initialized() ? JNI_TRUE : JNI_FALSE;
}
