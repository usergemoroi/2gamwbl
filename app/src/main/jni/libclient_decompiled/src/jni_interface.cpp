#include "../include/jni_bridge.h"
#include "../include/client.h"
#include "../include/constants.h"
#include <android/native_window_jni.h>

using namespace client;
using namespace client::jni;

static JavaVM* g_java_vm = nullptr;

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("JNI_OnLoad called");
    
    g_java_vm = vm;
    JavaVMHolder::SetJavaVM(vm);
    
    JNIEnv* env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        LOGE("Failed to get JNIEnv");
        return JNI_ERR;
    }
    
    // Initialize client
    if (!Client::getInstance().initialize()) {
        LOGE("Failed to initialize client");
        return JNI_ERR;
    }
    
    LOGI("libclient %s loaded successfully", constants::LIBRARY_VERSION);
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {
    LOGI("JNI_OnUnload called");
    
    Client::getInstance().shutdown();
    
    g_java_vm = nullptr;
}

// FloaterService JNI methods

JNI_METHOD(void, FloaterService, connect)(JNIEnv* env, jobject thiz, jstring connection_string) {
    LOGI("FloaterService.connect called");
    
    if (connection_string == nullptr) {
        LOGW("Connection string is null");
        return;
    }
    
    std::string conn_str = JNIHelper::JavaStringToStdString(env, connection_string);
    LOGD("Connecting to: %s", conn_str.c_str());
    
    Client& client = Client::getInstance();
    if (!client.connectToServer(conn_str)) {
        LOGE("Failed to connect to server");
    }
}

JNI_METHOD(void, FloaterService, disconnect)(JNIEnv* env, jobject thiz) {
    LOGI("FloaterService.disconnect called");
    
    Client& client = Client::getInstance();
    client.disconnectFromServer();
}

JNI_METHOD(void, FloaterService, initSurface)(JNIEnv* env, jobject thiz, 
                                              jobject surface, jint width, jint height) {
    LOGI("FloaterService.initSurface called: %dx%d", width, height);
    
    if (surface == nullptr) {
        LOGE("Surface is null");
        return;
    }
    
    ANativeWindow* window = ANativeWindow_fromSurface(env, surface);
    if (window == nullptr) {
        LOGE("Failed to get native window from surface");
        return;
    }
    
    Client& client = Client::getInstance();
    RenderEngine* render_engine = client.getRenderEngine();
    
    if (!render_engine->createSurface(window, width, height)) {
        LOGE("Failed to create render surface");
        ANativeWindow_release(window);
        return;
    }
    
    LOGI("Surface initialized successfully");
}

JNI_METHOD(void, FloaterService, removeSurface)(JNIEnv* env, jobject thiz) {
    LOGI("FloaterService.removeSurface called");
    
    Client& client = Client::getInstance();
    RenderEngine* render_engine = client.getRenderEngine();
    
    render_engine->stopRendering();
    render_engine->destroySurface();
}

JNI_METHOD(void, FloaterService, drawOn)(JNIEnv* env, jobject thiz, 
                                        jbyteArray data, jint width, jint height) {
    if (data == nullptr) {
        LOGW("drawOn: data is null");
        return;
    }
    
    jsize data_length = env->GetArrayLength(data);
    jbyte* data_ptr = env->GetByteArrayElements(data, nullptr);
    
    if (data_ptr == nullptr) {
        LOGE("drawOn: failed to get array elements");
        return;
    }
    
    Client& client = Client::getInstance();
    RenderEngine* render_engine = client.getRenderEngine();
    
    render_engine->drawData(reinterpret_cast<uint8_t*>(data_ptr), width, height);
    
    env->ReleaseByteArrayElements(data, data_ptr, JNI_ABORT);
}

JNI_METHOD(void, FloaterService, drawTick)(JNIEnv* env, jobject thiz) {
    Client& client = Client::getInstance();
    RenderEngine* render_engine = client.getRenderEngine();
    
    render_engine->drawTick();
}

JNI_METHOD(jboolean, FloaterService, findGame)(JNIEnv* env, jobject thiz, jstring package_name) {
    if (package_name == nullptr) {
        LOGW("findGame: package_name is null");
        return JNI_FALSE;
    }
    
    std::string pkg_name = JNIHelper::JavaStringToStdString(env, package_name);
    LOGD("Finding game: %s", pkg_name.c_str());
    
    Client& client = Client::getInstance();
    GameDetector* detector = client.getGameDetector();
    
    bool found = detector->findGame(pkg_name);
    LOGI("Game %s: %s", pkg_name.c_str(), found ? "found" : "not found");
    
    return found ? JNI_TRUE : JNI_FALSE;
}

JNI_METHOD(void, FloaterService, setScreen)(JNIEnv* env, jobject thiz, jint width, jint height) {
    LOGD("FloaterService.setScreen: %dx%d", width, height);
    
    Client& client = Client::getInstance();
    RenderEngine* render_engine = client.getRenderEngine();
    
    render_engine->setScreenSize(width, height);
}

JNI_METHOD(void, FloaterService, switch_)(JNIEnv* env, jobject thiz, jboolean enabled) {
    LOGI("FloaterService.switch: %s", enabled ? "enabled" : "disabled");
    
    Client& client = Client::getInstance();
    RenderEngine* render_engine = client.getRenderEngine();
    
    if (enabled) {
        render_engine->startRendering();
    } else {
        render_engine->stopRendering();
    }
}

// SuperJNI.Companion methods

JNI_COMPANION_METHOD(jboolean, check)(JNIEnv* env, jobject thiz, jstring key) {
    if (key == nullptr) {
        LOGW("check: key is null");
        return JNI_FALSE;
    }
    
    std::string key_str = JNIHelper::JavaStringToStdString(env, key);
    LOGD("Checking key: %s", key_str.substr(0, 8).c_str());
    
    Client& client = Client::getInstance();
    NetworkClient* network_client = client.getNetworkClient();
    
    bool valid = network_client->validateKey(key_str);
    return valid ? JNI_TRUE : JNI_FALSE;
}

JNI_COMPANION_METHOD(jboolean, licence)(JNIEnv* env, jobject thiz, jstring license_key) {
    if (license_key == nullptr) {
        LOGW("licence: license_key is null");
        return JNI_FALSE;
    }
    
    std::string license_str = JNIHelper::JavaStringToStdString(env, license_key);
    LOGD("Validating license");
    
    Client& client = Client::getInstance();
    bool valid = client.validateLicense(license_str);
    
    LOGI("License validation: %s", valid ? "success" : "failed");
    return valid ? JNI_TRUE : JNI_FALSE;
}

JNI_COMPANION_METHOD(jboolean, update)(JNIEnv* env, jobject thiz, jstring version) {
    if (version == nullptr) {
        LOGW("update: version is null");
        return JNI_FALSE;
    }
    
    std::string version_str = JNIHelper::JavaStringToStdString(env, version);
    LOGI("Checking update for version: %s", version_str.c_str());
    
    Client& client = Client::getInstance();
    bool has_update = client.checkForUpdates();
    
    return has_update ? JNI_TRUE : JNI_FALSE;
}

JNI_COMPANION_METHOD(jstring, currGameVer)(JNIEnv* env, jobject thiz) {
    Client& client = Client::getInstance();
    NetworkClient* network_client = client.getNetworkClient();
    
    std::string game_version = network_client->getGameVersion();
    LOGD("Current game version: %s", game_version.c_str());
    
    return JNIHelper::StdStringToJavaString(env, game_version);
}

JNI_COMPANION_METHOD(jstring, currPlugVer)(JNIEnv* env, jobject thiz) {
    Client& client = Client::getInstance();
    NetworkClient* network_client = client.getNetworkClient();
    
    std::string plugin_version = network_client->getPluginVersion();
    LOGD("Current plugin version: %s", plugin_version.c_str());
    
    return JNIHelper::StdStringToJavaString(env, plugin_version);
}

JNI_COMPANION_METHOD(jstring, currPlugUrl)(JNIEnv* env, jobject thiz) {
    Client& client = Client::getInstance();
    NetworkClient* network_client = client.getNetworkClient();
    
    std::string plugin_url = network_client->getPluginUrl();
    LOGD("Current plugin URL: %s", plugin_url.c_str());
    
    return JNIHelper::StdStringToJavaString(env, plugin_url);
}

JNI_COMPANION_METHOD(jlong, getTime)(JNIEnv* env, jobject thiz) {
    Client& client = Client::getInstance();
    NetworkClient* network_client = client.getNetworkClient();
    
    uint64_t server_time = network_client->getServerTime();
    LOGD("Server time: %llu", (unsigned long long)server_time);
    
    return static_cast<jlong>(server_time);
}

JNI_COMPANION_METHOD(jstring, urlTg)(JNIEnv* env, jobject thiz) {
    Client& client = Client::getInstance();
    NetworkClient* network_client = client.getNetworkClient();
    
    std::string telegram_url = network_client->getTelegramUrl();
    LOGD("Telegram URL: %s", telegram_url.c_str());
    
    return JNIHelper::StdStringToJavaString(env, telegram_url);
}

} // extern "C"
