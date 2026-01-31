#include "../include/client.h"
#include "../include/jni_bridge.h"
#include "../include/constants.h"

namespace client {
namespace jni {

JavaVM* JavaVMHolder::s_java_vm = nullptr;

void JavaVMHolder::SetJavaVM(JavaVM* vm) {
    s_java_vm = vm;
}

JavaVM* JavaVMHolder::GetJavaVM() {
    return s_java_vm;
}

JNIEnv* JavaVMHolder::GetEnv() {
    if (s_java_vm == nullptr) {
        return nullptr;
    }
    
    JNIEnv* env = nullptr;
    if (s_java_vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return nullptr;
    }
    
    return env;
}

bool JNIHelper::AttachCurrentThread(JavaVM* vm, JNIEnv** env) {
    if (vm == nullptr || env == nullptr) {
        return false;
    }
    
    jint result = vm->AttachCurrentThread(env, nullptr);
    return result == JNI_OK;
}

void JNIHelper::DetachCurrentThread(JavaVM* vm) {
    if (vm != nullptr) {
        vm->DetachCurrentThread();
    }
}

std::string JNIHelper::JavaStringToStdString(JNIEnv* env, jstring jstr) {
    if (env == nullptr || jstr == nullptr) {
        return "";
    }
    
    const char* chars = env->GetStringUTFChars(jstr, nullptr);
    if (chars == nullptr) {
        return "";
    }
    
    std::string result(chars);
    env->ReleaseStringUTFChars(jstr, chars);
    
    return result;
}

jstring JNIHelper::StdStringToJavaString(JNIEnv* env, const std::string& str) {
    if (env == nullptr) {
        return nullptr;
    }
    
    return env->NewStringUTF(str.c_str());
}

bool JNIHelper::CheckException(JNIEnv* env) {
    if (env == nullptr) {
        return false;
    }
    
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        return true;
    }
    
    return false;
}

void JNIHelper::ClearException(JNIEnv* env) {
    if (env != nullptr && env->ExceptionCheck()) {
        env->ExceptionClear();
    }
}

ScopedJNIEnv::ScopedJNIEnv(JavaVM* vm) : vm_(vm), env_(nullptr), should_detach_(false) {
    if (vm_ != nullptr) {
        jint result = vm_->GetEnv(reinterpret_cast<void**>(&env_), JNI_VERSION_1_6);
        if (result == JNI_EDETACHED) {
            result = vm_->AttachCurrentThread(&env_, nullptr);
            if (result == JNI_OK) {
                should_detach_ = true;
            }
        }
    }
}

ScopedJNIEnv::~ScopedJNIEnv() {
    if (should_detach_ && vm_ != nullptr) {
        vm_->DetachCurrentThread();
    }
}

} // namespace jni
} // namespace client
