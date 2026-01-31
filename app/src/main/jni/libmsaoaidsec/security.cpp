/*
 * Decompiled from libmsaoaidsec.so (arm64-v8a)
 * GameBlaster Pro v3.1 - Security and Device ID Library
 * 
 * This library handles device identification, anti-tamper checks,
 * and security-related functions for the application.
 * 
 * Binary size: 670KB
 * Architecture: ARM64-v8a (AArch64)
 * 
 * Note: This appears to be based on Alibaba Mobile Security SDK (OAID)
 */

#include <jni.h>
#include <android/log.h>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <cstring>
#include <sys/system_properties.h>
#include <unistd.h>
#include <sys/stat.h>

#define LOG_TAG "SecurityLib"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace security {

// Device Information Collector
class DeviceInfo {
private:
    std::string android_id_;
    std::string oaid_;
    std::string imei_;
    std::string serial_;
    std::string manufacturer_;
    std::string model_;
    std::string brand_;
    std::string device_;
    std::string hardware_;
    std::string fingerprint_;
    std::mutex info_mutex_;
    
public:
    DeviceInfo() {
        collectDeviceInfo();
    }
    
    void collectDeviceInfo() {
        std::lock_guard<std::mutex> lock(info_mutex_);
        
        char prop_value[PROP_VALUE_MAX];
        
        // Get system properties
        if (__system_property_get("ro.product.manufacturer", prop_value) > 0) {
            manufacturer_ = prop_value;
        }
        
        if (__system_property_get("ro.product.model", prop_value) > 0) {
            model_ = prop_value;
        }
        
        if (__system_property_get("ro.product.brand", prop_value) > 0) {
            brand_ = prop_value;
        }
        
        if (__system_property_get("ro.product.device", prop_value) > 0) {
            device_ = prop_value;
        }
        
        if (__system_property_get("ro.hardware", prop_value) > 0) {
            hardware_ = prop_value;
        }
        
        if (__system_property_get("ro.build.fingerprint", prop_value) > 0) {
            fingerprint_ = prop_value;
        }
        
        if (__system_property_get("ro.serialno", prop_value) > 0) {
            serial_ = prop_value;
        }
        
        LOGI("Device: %s %s %s", manufacturer_.c_str(), brand_.c_str(), model_.c_str());
    }
    
    std::string getAndroidId(JNIEnv* env, jobject context) {
        if (!android_id_.empty()) {
            return android_id_;
        }
        
        // Get ContentResolver
        jclass context_class = env->GetObjectClass(context);
        jmethodID get_resolver = env->GetMethodID(context_class, "getContentResolver",
                                                  "()Landroid/content/ContentResolver;");
        jobject resolver = env->CallObjectMethod(context, get_resolver);
        
        // Get Settings.Secure class
        jclass secure_class = env->FindClass("android/provider/Settings$Secure");
        jmethodID get_string = env->GetStaticMethodID(secure_class, "getString",
            "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;");
        
        // Get ANDROID_ID
        jstring name = env->NewStringUTF("android_id");
        jstring value = (jstring)env->CallStaticObjectMethod(secure_class, get_string, resolver, name);
        
        if (value != nullptr) {
            const char* str = env->GetStringUTFChars(value, nullptr);
            android_id_ = str;
            env->ReleaseStringUTFChars(value, str);
        }
        
        return android_id_;
    }
    
    std::string getOAID(JNIEnv* env, jobject context) {
        if (!oaid_.empty()) {
            return oaid_;
        }
        
        // Try to get OAID from various sources
        // This is a simplified version - real implementation would check multiple providers
        
        // Try Xiaomi OAID
        oaid_ = getMiuiOAID(env, context);
        if (!oaid_.empty()) {
            return oaid_;
        }
        
        // Try Huawei OAID
        oaid_ = getHuaweiOAID(env, context);
        if (!oaid_.empty()) {
            return oaid_;
        }
        
        // Try Samsung OAID
        oaid_ = getSamsungOAID(env, context);
        if (!oaid_.empty()) {
            return oaid_;
        }
        
        // Fallback to Android ID
        return getAndroidId(env, context);
    }
    
    std::string getMiuiOAID(JNIEnv* env, jobject context) {
        // Try to get OAID from MIUI
        try {
            jclass miui_class = env->FindClass("com/bun/miitmdid/core/JLibrary");
            if (miui_class == nullptr) {
                env->ExceptionClear();
                return "";
            }
            
            // Implementation would call into MIUI OAID SDK
            // This is simplified
        } catch (...) {
            env->ExceptionClear();
        }
        
        return "";
    }
    
    std::string getHuaweiOAID(JNIEnv* env, jobject context) {
        // Try to get OAID from Huawei HMS
        try {
            jclass huawei_class = env->FindClass("com/huawei/hms/ads/identifier/AdvertisingIdClient");
            if (huawei_class == nullptr) {
                env->ExceptionClear();
                return "";
            }
            
            // Implementation would call into Huawei HMS
            // This is simplified
        } catch (...) {
            env->ExceptionClear();
        }
        
        return "";
    }
    
    std::string getSamsungOAID(JNIEnv* env, jobject context) {
        // Try to get OAID from Samsung
        try {
            jclass samsung_class = env->FindClass("com/sec/android/app/samsungapps/SamsungAppsManager");
            if (samsung_class == nullptr) {
                env->ExceptionClear();
                return "";
            }
            
            // Implementation would call into Samsung SDK
            // This is simplified
        } catch (...) {
            env->ExceptionClear();
        }
        
        return "";
    }
    
    std::string getManufacturer() const { return manufacturer_; }
    std::string getModel() const { return model_; }
    std::string getBrand() const { return brand_; }
    std::string getDevice() const { return device_; }
    std::string getHardware() const { return hardware_; }
    std::string getFingerprint() const { return fingerprint_; }
    std::string getSerial() const { return serial_; }
};

// Anti-Tamper Detection
class AntiTamper {
public:
    static bool checkDebugger() {
        // Check if debugger is attached
        char status[256];
        FILE* fp = fopen("/proc/self/status", "r");
        if (fp == nullptr) {
            return false;
        }
        
        bool debugger_attached = false;
        while (fgets(status, sizeof(status), fp)) {
            if (strncmp(status, "TracerPid:", 10) == 0) {
                int tracer_pid = atoi(status + 10);
                if (tracer_pid != 0) {
                    debugger_attached = true;
                    LOGW("Debugger detected! TracerPid: %d", tracer_pid);
                }
                break;
            }
        }
        
        fclose(fp);
        return debugger_attached;
    }
    
    static bool checkRoot() {
        // Check for common root indicators
        const char* root_files[] = {
            "/system/app/Superuser.apk",
            "/system/xbin/su",
            "/system/bin/su",
            "/sbin/su",
            "/system/su",
            "/system/bin/.ext/.su",
            "/data/local/xbin/su",
            "/data/local/bin/su",
            "/data/local/su"
        };
        
        for (const char* file : root_files) {
            struct stat st;
            if (stat(file, &st) == 0) {
                LOGW("Root indicator found: %s", file);
                return true;
            }
        }
        
        // Check for common root apps
        // In real implementation, this would check package manager
        
        return false;
    }
    
    static bool checkEmulator() {
        char prop_value[PROP_VALUE_MAX];
        
        // Check for emulator properties
        if (__system_property_get("ro.kernel.qemu", prop_value) > 0) {
            if (strcmp(prop_value, "1") == 0) {
                LOGW("Emulator detected (qemu)");
                return true;
            }
        }
        
        if (__system_property_get("ro.hardware", prop_value) > 0) {
            if (strstr(prop_value, "goldfish") != nullptr ||
                strstr(prop_value, "ranchu") != nullptr) {
                LOGW("Emulator detected (hardware)");
                return true;
            }
        }
        
        if (__system_property_get("ro.product.model", prop_value) > 0) {
            if (strstr(prop_value, "Emulator") != nullptr ||
                strstr(prop_value, "Android SDK") != nullptr) {
                LOGW("Emulator detected (model)");
                return true;
            }
        }
        
        return false;
    }
    
    static bool checkXposed() {
        // Check for Xposed framework
        try {
            FILE* fp = fopen("/proc/self/maps", "r");
            if (fp == nullptr) {
                return false;
            }
            
            char line[512];
            bool xposed_found = false;
            
            while (fgets(line, sizeof(line), fp)) {
                if (strstr(line, "XposedBridge") != nullptr ||
                    strstr(line, "de.robv.android.xposed") != nullptr ||
                    strstr(line, "EdXposed") != nullptr) {
                    LOGW("Xposed framework detected!");
                    xposed_found = true;
                    break;
                }
            }
            
            fclose(fp);
            return xposed_found;
        } catch (...) {
            return false;
        }
    }
    
    static bool checkFrida() {
        // Check for Frida instrumentation
        try {
            FILE* fp = fopen("/proc/self/maps", "r");
            if (fp == nullptr) {
                return false;
            }
            
            char line[512];
            bool frida_found = false;
            
            while (fgets(line, sizeof(line), fp)) {
                if (strstr(line, "frida") != nullptr ||
                    strstr(line, "frida-agent") != nullptr ||
                    strstr(line, "gum-js-loop") != nullptr) {
                    LOGW("Frida detected!");
                    frida_found = true;
                    break;
                }
            }
            
            fclose(fp);
            return frida_found;
        } catch (...) {
            return false;
        }
    }
};

// Signature Verification
class SignatureVerifier {
public:
    static bool verifyAppSignature(JNIEnv* env, jobject context, const std::string& expected_signature) {
        try {
            // Get PackageManager
            jclass context_class = env->GetObjectClass(context);
            jmethodID get_pm = env->GetMethodID(context_class, "getPackageManager",
                                               "()Landroid/content/pm/PackageManager;");
            jobject pm = env->CallObjectMethod(context, get_pm);
            
            // Get package name
            jmethodID get_package_name = env->GetMethodID(context_class, "getPackageName",
                                                         "()Ljava/lang/String;");
            jstring package_name = (jstring)env->CallObjectMethod(context, get_package_name);
            
            // Get PackageInfo with signatures
            jclass pm_class = env->GetObjectClass(pm);
            jmethodID get_package_info = env->GetMethodID(pm_class, "getPackageInfo",
                "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
            
            const int GET_SIGNATURES = 0x00000040;
            jobject package_info = env->CallObjectMethod(pm, get_package_info, package_name, GET_SIGNATURES);
            
            if (package_info == nullptr) {
                LOGE("Failed to get package info");
                return false;
            }
            
            // Get signatures array
            jclass package_info_class = env->GetObjectClass(package_info);
            jfieldID signatures_field = env->GetFieldID(package_info_class, "signatures",
                                                        "[Landroid/content/pm/Signature;");
            jobjectArray signatures = (jobjectArray)env->GetObjectField(package_info, signatures_field);
            
            if (signatures == nullptr) {
                LOGE("No signatures found");
                return false;
            }
            
            // Verify first signature
            jobject signature = env->GetObjectArrayElement(signatures, 0);
            jclass signature_class = env->GetObjectClass(signature);
            jmethodID to_chars = env->GetMethodID(signature_class, "toCharsString",
                                                  "()Ljava/lang/String;");
            jstring sig_string = (jstring)env->CallObjectMethod(signature, to_chars);
            
            const char* sig = env->GetStringUTFChars(sig_string, nullptr);
            bool valid = (expected_signature == sig);
            
            if (!valid) {
                LOGW("Signature mismatch!");
            }
            
            env->ReleaseStringUTFChars(sig_string, sig);
            
            return valid;
        } catch (...) {
            LOGE("Exception during signature verification");
            env->ExceptionClear();
            return false;
        }
    }
};

static DeviceInfo* g_device_info = nullptr;

} // namespace security

// JNI Implementation
extern "C" {

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("libmsaoaidsec.so loaded - Security Library");
    
    security::g_device_info = new security::DeviceInfo();
    
    // Perform initial security checks
    if (security::AntiTamper::checkDebugger()) {
        LOGW("Warning: Debugger detected!");
    }
    
    if (security::AntiTamper::checkRoot()) {
        LOGW("Warning: Root detected!");
    }
    
    if (security::AntiTamper::checkEmulator()) {
        LOGW("Warning: Emulator detected!");
    }
    
    if (security::AntiTamper::checkXposed()) {
        LOGW("Warning: Xposed detected!");
    }
    
    if (security::AntiTamper::checkFrida()) {
        LOGW("Warning: Frida detected!");
    }
    
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved) {
    LOGI("libmsaoaidsec.so unloading");
    delete security::g_device_info;
}

// Get Android ID
JNIEXPORT jstring JNICALL
Java_com_msaoa_security_DeviceId_getAndroidId(JNIEnv* env, jclass clazz, jobject context) {
    std::string android_id = security::g_device_info->getAndroidId(env, context);
    return env->NewStringUTF(android_id.c_str());
}

// Get OAID (Open Anonymous Device Identifier)
JNIEXPORT jstring JNICALL
Java_com_msaoa_security_DeviceId_getOAID(JNIEnv* env, jclass clazz, jobject context) {
    std::string oaid = security::g_device_info->getOAID(env, context);
    return env->NewStringUTF(oaid.c_str());
}

// Get device info
JNIEXPORT jstring JNICALL
Java_com_msaoa_security_DeviceId_getManufacturer(JNIEnv* env, jclass clazz) {
    return env->NewStringUTF(security::g_device_info->getManufacturer().c_str());
}

JNIEXPORT jstring JNICALL
Java_com_msaoa_security_DeviceId_getModel(JNIEnv* env, jclass clazz) {
    return env->NewStringUTF(security::g_device_info->getModel().c_str());
}

JNIEXPORT jstring JNICALL
Java_com_msaoa_security_DeviceId_getBrand(JNIEnv* env, jclass clazz) {
    return env->NewStringUTF(security::g_device_info->getBrand().c_str());
}

// Anti-tamper checks
JNIEXPORT jboolean JNICALL
Java_com_msaoa_security_AntiTamper_checkDebugger(JNIEnv* env, jclass clazz) {
    return security::AntiTamper::checkDebugger() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_msaoa_security_AntiTamper_checkRoot(JNIEnv* env, jclass clazz) {
    return security::AntiTamper::checkRoot() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_msaoa_security_AntiTamper_checkEmulator(JNIEnv* env, jclass clazz) {
    return security::AntiTamper::checkEmulator() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_msaoa_security_AntiTamper_checkXposed(JNIEnv* env, jclass clazz) {
    return security::AntiTamper::checkXposed() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_msaoa_security_AntiTamper_checkFrida(JNIEnv* env, jclass clazz) {
    return security::AntiTamper::checkFrida() ? JNI_TRUE : JNI_FALSE;
}

// Signature verification
JNIEXPORT jboolean JNICALL
Java_com_msaoa_security_SignatureVerifier_verify(JNIEnv* env, jclass clazz, 
                                                 jobject context, jstring expected) {
    const char* expected_sig = env->GetStringUTFChars(expected, nullptr);
    bool result = security::SignatureVerifier::verifyAppSignature(env, context, expected_sig);
    env->ReleaseStringUTFChars(expected, expected_sig);
    return result ? JNI_TRUE : JNI_FALSE;
}

} // extern "C"
