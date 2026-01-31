/*
 * Decompiled from lib2f8c0b3257fcc345.so
 * GameBlaster Pro v3.1 - Burri Encrypted Loader Library
 * 
 * This library is extracted from encrypted assets at runtime and provides
 * the bridge between the proxy loader and the actual application code.
 * 
 * File location in APK: assets/burriiiii/<abi>/lib2f8c0b3257fcc345.so
 * Encryption: Custom Burri encryption scheme with CRC verification
 */

#include <jni.h>
#include <android/log.h>
#include <dlfcn.h>
#include <string>
#include <map>
#include <mutex>
#include <cstring>

#define LOG_TAG "BurriLoader"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace burri {

// Global state
static JavaVM* g_jvm = nullptr;
static jobject g_application_obj = nullptr;
static jobject g_app_component_factory = nullptr;
static std::string g_real_app_name;
static std::string g_real_package_name;
static std::map<std::string, void*> g_loaded_libraries;
static std::mutex g_state_mutex;

// Application Bridge - bridges to the real application hidden in native code
class ApplicationBridge {
private:
    jobject real_application_;
    jclass real_app_class_;
    std::string class_name_;
    
public:
    ApplicationBridge() : real_application_(nullptr), real_app_class_(nullptr) {}
    
    bool initialize(JNIEnv* env, const std::string& className) {
        LOGI("Initializing ApplicationBridge for: %s", className.c_str());
        
        class_name_ = className;
        
        // Load the real application class
        jclass app_class = env->FindClass(className.c_str());
        if (app_class == nullptr) {
            LOGE("Failed to find application class: %s", className.c_str());
            env->ExceptionClear();
            return false;
        }
        
        real_app_class_ = reinterpret_cast<jclass>(env->NewGlobalRef(app_class));
        
        // Create instance of real application
        jmethodID constructor = env->GetMethodID(real_app_class_, "<init>", "()V");
        if (constructor == nullptr) {
            LOGE("Failed to find constructor for: %s", className.c_str());
            env->ExceptionClear();
            return false;
        }
        
        jobject app_obj = env->NewObject(real_app_class_, constructor);
        if (app_obj == nullptr) {
            LOGE("Failed to create instance of: %s", className.c_str());
            env->ExceptionClear();
            return false;
        }
        
        real_application_ = env->NewGlobalRef(app_obj);
        
        LOGI("ApplicationBridge initialized successfully");
        return true;
    }
    
    jobject getApplication() const {
        return real_application_;
    }
    
    jclass getApplicationClass() const {
        return real_app_class_;
    }
    
    void attachContext(JNIEnv* env, jobject context) {
        if (real_application_ == nullptr) {
            LOGW("Real application not initialized");
            return;
        }
        
        // Call attachBaseContext on real application
        jmethodID attach_method = env->GetMethodID(real_app_class_, 
            "attachBaseContext", "(Landroid/content/Context;)V");
        
        if (attach_method != nullptr) {
            env->CallVoidMethod(real_application_, attach_method, context);
            LOGI("attachBaseContext called on real application");
        }
    }
    
    void onCreate(JNIEnv* env) {
        if (real_application_ == nullptr) {
            LOGW("Real application not initialized");
            return;
        }
        
        // Call onCreate on real application
        jmethodID onCreate_method = env->GetMethodID(real_app_class_, 
            "onCreate", "()V");
        
        if (onCreate_method != nullptr) {
            env->CallVoidMethod(real_application_, onCreate_method);
            LOGI("onCreate called on real application");
        }
    }
};

static ApplicationBridge* g_app_bridge = nullptr;

// ComponentFactory Bridge - intercepts component creation
class ComponentFactoryBridge {
private:
    std::map<std::string, std::string> component_mapping_;
    
public:
    ComponentFactoryBridge() {
        // Initialize component mapping from manifest stub names to real names
        LOGI("ComponentFactoryBridge initialized");
    }
    
    void registerComponent(const std::string& stub_name, const std::string& real_name) {
        std::lock_guard<std::mutex> lock(g_state_mutex);
        component_mapping_[stub_name] = real_name;
        LOGI("Registered component: %s -> %s", stub_name.c_str(), real_name.c_str());
    }
    
    std::string getRealComponentName(const std::string& stub_name) {
        std::lock_guard<std::mutex> lock(g_state_mutex);
        auto it = component_mapping_.find(stub_name);
        if (it != component_mapping_.end()) {
            return it->second;
        }
        return stub_name;
    }
    
    jobject instantiateActivity(JNIEnv* env, jclass cl, const std::string& className) {
        std::string real_name = getRealComponentName(className);
        
        LOGI("Instantiating activity: %s (real: %s)", className.c_str(), real_name.c_str());
        
        jclass activity_class = env->FindClass(real_name.c_str());
        if (activity_class == nullptr) {
            LOGE("Failed to find activity class: %s", real_name.c_str());
            env->ExceptionClear();
            return nullptr;
        }
        
        jmethodID constructor = env->GetMethodID(activity_class, "<init>", "()V");
        if (constructor == nullptr) {
            LOGE("Failed to find constructor for: %s", real_name.c_str());
            env->ExceptionClear();
            return nullptr;
        }
        
        return env->NewObject(activity_class, constructor);
    }
};

static ComponentFactoryBridge* g_component_factory_bridge = nullptr;

// ClassLoader Bridge - provides custom class loading
class ClassLoaderBridge {
private:
    jobject system_classloader_;
    jobject dex_classloader_;
    
public:
    ClassLoaderBridge() : system_classloader_(nullptr), dex_classloader_(nullptr) {}
    
    void initialize(JNIEnv* env, jobject classloader) {
        LOGI("Initializing ClassLoaderBridge");
        system_classloader_ = env->NewGlobalRef(classloader);
    }
    
    jclass loadClass(JNIEnv* env, const std::string& className) {
        if (system_classloader_ == nullptr) {
            LOGW("ClassLoader not initialized");
            return nullptr;
        }
        
        jclass classloader_class = env->GetObjectClass(system_classloader_);
        jmethodID load_class_method = env->GetMethodID(classloader_class, 
            "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
        
        if (load_class_method == nullptr) {
            LOGE("Failed to find loadClass method");
            env->ExceptionClear();
            return nullptr;
        }
        
        jstring class_name = env->NewStringUTF(className.c_str());
        jclass loaded_class = reinterpret_cast<jclass>(
            env->CallObjectMethod(system_classloader_, load_class_method, class_name));
        env->DeleteLocalRef(class_name);
        
        return loaded_class;
    }
};

static ClassLoaderBridge* g_classloader_bridge = nullptr;

} // namespace burri

// JNI Implementation
extern "C" {

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("lib2f8c0b3257fcc345.so loaded - Burri Loader");
    
    burri::g_jvm = vm;
    
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        LOGE("Failed to get JNI environment");
        return JNI_ERR;
    }
    
    // Initialize bridges
    burri::g_app_bridge = new burri::ApplicationBridge();
    burri::g_component_factory_bridge = new burri::ComponentFactoryBridge();
    burri::g_classloader_bridge = new burri::ClassLoaderBridge();
    
    LOGI("Burri Loader initialized");
    
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved) {
    LOGI("lib2f8c0b3257fcc345.so unloading");
    
    delete burri::g_app_bridge;
    delete burri::g_component_factory_bridge;
    delete burri::g_classloader_bridge;
}

// Native bridge methods for ProxyApplication and ProxyComponentFactory

// Initialize Application - called from ProxyApplication.attachBaseContext
JNIEXPORT void JNICALL
Java_p00093e85d7aface4dfb_JniBridge_craa(JNIEnv* env, jclass clazz, 
                                          jobject context, jstring appName) {
    const char* app_name_str = env->GetStringUTFChars(appName, nullptr);
    
    LOGI("craa: Initializing real application: %s", app_name_str);
    
    burri::g_real_app_name = app_name_str;
    
    // Initialize application bridge
    if (burri::g_app_bridge) {
        burri::g_app_bridge->initialize(env, app_name_str);
        burri::g_app_bridge->attachContext(env, context);
    }
    
    env->ReleaseStringUTFChars(appName, app_name_str);
}

// Create Application onCreate - called from ProxyApplication.onCreate
JNIEXPORT void JNICALL
Java_p00093e85d7aface4dfb_JniBridge_craoc(JNIEnv* env, jclass clazz, jstring appName) {
    const char* app_name_str = env->GetStringUTFChars(appName, nullptr);
    
    LOGI("craoc: Calling onCreate on real application: %s", app_name_str);
    
    if (burri::g_app_bridge) {
        burri::g_app_bridge->onCreate(env);
    }
    
    env->ReleaseStringUTFChars(appName, app_name_str);
}

// Set ClassLoader - called from ProxyApplication.attachBaseContext
JNIEXPORT void JNICALL
Java_p00093e85d7aface4dfb_JniBridge_cbde(JNIEnv* env, jclass clazz, jobject classLoader) {
    LOGI("cbde: Setting ClassLoader");
    
    if (burri::g_classloader_bridge) {
        burri::g_classloader_bridge->initialize(env, classLoader);
    }
}

// Return Application - called to get the real application instance
JNIEXPORT jobject JNICALL
Java_p00093e85d7aface4dfb_JniBridge_ra(JNIEnv* env, jclass clazz, jstring appName) {
    const char* app_name_str = env->GetStringUTFChars(appName, nullptr);
    
    LOGI("ra: Returning real application: %s", app_name_str);
    
    jobject result = nullptr;
    if (burri::g_app_bridge) {
        result = burri::g_app_bridge->getApplication();
    }
    
    env->ReleaseStringUTFChars(appName, app_name_str);
    return result;
}

// Get Application Package Name
JNIEXPORT jstring JNICALL
Java_p00093e85d7aface4dfb_JniBridge_gap(JNIEnv* env, jclass clazz) {
    LOGI("gap: Getting application package name");
    
    if (burri::g_real_package_name.empty()) {
        burri::g_real_package_name = "com.eternal.xdsdk.App";
    }
    
    return env->NewStringUTF(burri::g_real_package_name.c_str());
}

// Get Data Path
JNIEXPORT jstring JNICALL
Java_p00093e85d7aface4dfb_JniBridge_gdp(JNIEnv* env, jclass clazz) {
    LOGI("gdp: Getting data path");
    return env->NewStringUTF("/data/data/com.eternal.xdsdk");
}

// Initialize Assets
JNIEXPORT void JNICALL
Java_p00093e85d7aface4dfb_JniBridge_ia(JNIEnv* env, jclass clazz) {
    LOGI("ia: Initializing assets");
    
    // Load encrypted assets from burriEnc and burriiiii directories
    // Decrypt and prepare for runtime usage
}

// Get Real Application Package Name
JNIEXPORT jstring JNICALL
Java_p00093e85d7aface4dfb_JniBridge_rapn(JNIEnv* env, jclass clazz) {
    LOGI("rapn: Getting real application package name");
    return env->NewStringUTF(burri::g_real_app_name.c_str());
}

// Get Real Component Factory
JNIEXPORT jstring JNICALL
Java_p00093e85d7aface4dfb_JniBridge_rcf(JNIEnv* env, jclass clazz) {
    LOGI("rcf: Getting real component factory");
    return env->NewStringUTF("com.eternal.xdsdk.AppComponentFactory");
}

// Register DEX - load additional DEX files
JNIEXPORT void JNICALL
Java_p00093e85d7aface4dfb_JniBridge_rde(JNIEnv* env, jclass clazz, 
                                        jobject classLoader, jstring dexPath) {
    const char* dex_path_str = env->GetStringUTFChars(dexPath, nullptr);
    
    LOGI("rde: Registering DEX: %s", dex_path_str);
    
    // Load additional DEX file into ClassLoader
    // This allows loading code from encrypted assets
    
    env->ReleaseStringUTFChars(dexPath, dex_path_str);
}

} // extern "C"
