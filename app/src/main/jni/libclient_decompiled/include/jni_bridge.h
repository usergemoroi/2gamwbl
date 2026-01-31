#ifndef LIBCLIENT_JNI_BRIDGE_H
#define LIBCLIENT_JNI_BRIDGE_H

#include <jni.h>
#include <string>
#include <vector>
#include <android/log.h>

// JNI Helper macros
#define JNI_METHOD(return_type, class_name, method_name) \
    JNIEXPORT return_type JNICALL Java_com_eternal_xdsdk_##class_name##_##method_name

#define JNI_COMPANION_METHOD(return_type, method_name) \
    JNIEXPORT return_type JNICALL Java_com_eternal_xdsdk_SuperJNI_00024Companion_##method_name

// Logging macros
#define LOG_TAG "libclient"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

// JNI utility functions
namespace client {
namespace jni {

class JNIHelper {
public:
    static bool AttachCurrentThread(JavaVM* vm, JNIEnv** env);
    static void DetachCurrentThread(JavaVM* vm);
    
    static std::string JavaStringToStdString(JNIEnv* env, jstring jstr);
    static jstring StdStringToJavaString(JNIEnv* env, const std::string& str);
    
    static std::vector<std::string> JavaStringArrayToVector(JNIEnv* env, jobjectArray array);
    static jobjectArray VectorToJavaStringArray(JNIEnv* env, const std::vector<std::string>& vec);
    
    static bool CheckException(JNIEnv* env);
    static void ClearException(JNIEnv* env);
    
    static jclass FindClass(JNIEnv* env, const char* class_name);
    static jmethodID GetMethodID(JNIEnv* env, jclass clazz, const char* method_name, const char* signature);
    static jfieldID GetFieldID(JNIEnv* env, jclass clazz, const char* field_name, const char* signature);
    
    static jobject CallObjectMethod(JNIEnv* env, jobject obj, jmethodID method_id, ...);
    static jboolean CallBooleanMethod(JNIEnv* env, jobject obj, jmethodID method_id, ...);
    static jint CallIntMethod(JNIEnv* env, jobject obj, jmethodID method_id, ...);
    static jlong CallLongMethod(JNIEnv* env, jobject obj, jmethodID method_id, ...);
    
    static jobject NewGlobalRef(JNIEnv* env, jobject obj);
    static void DeleteGlobalRef(JNIEnv* env, jobject obj);
    static jobject NewLocalRef(JNIEnv* env, jobject obj);
    static void DeleteLocalRef(JNIEnv* env, jobject obj);
};

class JavaVMHolder {
public:
    static void SetJavaVM(JavaVM* vm);
    static JavaVM* GetJavaVM();
    static JNIEnv* GetEnv();
    
private:
    static JavaVM* s_java_vm;
};

class ScopedJNIEnv {
public:
    explicit ScopedJNIEnv(JavaVM* vm);
    ~ScopedJNIEnv();
    
    JNIEnv* get() const { return env_; }
    operator JNIEnv*() const { return env_; }
    
private:
    JavaVM* vm_;
    JNIEnv* env_;
    bool should_detach_;
};

template<typename T>
class ScopedLocalRef {
public:
    ScopedLocalRef(JNIEnv* env, T ref) : env_(env), ref_(ref) {}
    ~ScopedLocalRef() {
        if (ref_ != nullptr) {
            env_->DeleteLocalRef(ref_);
        }
    }
    
    T get() const { return ref_; }
    T release() {
        T ret = ref_;
        ref_ = nullptr;
        return ret;
    }
    
    operator T() const { return ref_; }
    
private:
    JNIEnv* env_;
    T ref_;
};

} // namespace jni
} // namespace client

// JNI method declarations for FloaterService
extern "C" {

JNI_METHOD(void, FloaterService, connect)(JNIEnv* env, jobject thiz, jstring connection_string);
JNI_METHOD(void, FloaterService, disconnect)(JNIEnv* env, jobject thiz);
JNI_METHOD(void, FloaterService, initSurface)(JNIEnv* env, jobject thiz, jobject surface, jint width, jint height);
JNI_METHOD(void, FloaterService, removeSurface)(JNIEnv* env, jobject thiz);
JNI_METHOD(void, FloaterService, drawOn)(JNIEnv* env, jobject thiz, jbyteArray data, jint width, jint height);
JNI_METHOD(void, FloaterService, drawTick)(JNIEnv* env, jobject thiz);
JNI_METHOD(jboolean, FloaterService, findGame)(JNIEnv* env, jobject thiz, jstring package_name);
JNI_METHOD(void, FloaterService, setScreen)(JNIEnv* env, jobject thiz, jint width, jint height);
JNI_METHOD(void, FloaterService, switch_)(JNIEnv* env, jobject thiz, jboolean enabled);

} // extern "C"

// JNI method declarations for SuperJNI.Companion
extern "C" {

JNI_COMPANION_METHOD(jboolean, check)(JNIEnv* env, jobject thiz, jstring key);
JNI_COMPANION_METHOD(jboolean, licence)(JNIEnv* env, jobject thiz, jstring license_key);
JNI_COMPANION_METHOD(jboolean, update)(JNIEnv* env, jobject thiz, jstring version);
JNI_COMPANION_METHOD(jstring, currGameVer)(JNIEnv* env, jobject thiz);
JNI_COMPANION_METHOD(jstring, currPlugVer)(JNIEnv* env, jobject thiz);
JNI_COMPANION_METHOD(jstring, currPlugUrl)(JNIEnv* env, jobject thiz);
JNI_COMPANION_METHOD(jlong, getTime)(JNIEnv* env, jobject thiz);
JNI_COMPANION_METHOD(jstring, urlTg)(JNIEnv* env, jobject thiz);

} // extern "C"

// JNI initialization
extern "C" {
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved);
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved);
}

#endif // LIBCLIENT_JNI_BRIDGE_H
