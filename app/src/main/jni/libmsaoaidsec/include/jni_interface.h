#ifndef LIBMSAOAIDSEC_JNI_INTERFACE_H
#define LIBMSAOAIDSEC_JNI_INTERFACE_H

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

#define JAVA_PKG "com/mobile/security/oaid"

#define JNI_METHOD(returnType, methodName) \
    JNIEXPORT returnType JNICALL Java_com_mobile_security_oaid_##methodName

jint JNI_OnLoad(JavaVM* vm, void* reserved);
void JNI_OnUnload(JavaVM* vm, void* reserved);

JNI_METHOD(jstring, DeviceInfo_getDeviceId)(JNIEnv* env, jobject thiz, jobject context);
JNI_METHOD(jstring, DeviceInfo_getAndroidId)(JNIEnv* env, jobject thiz, jobject context);
JNI_METHOD(jstring, DeviceInfo_getOAID)(JNIEnv* env, jobject thiz, jobject context);
JNI_METHOD(jstring, DeviceInfo_getIMEI)(JNIEnv* env, jobject thiz, jobject context);
JNI_METHOD(jstring, DeviceInfo_getSerial)(JNIEnv* env, jobject thiz, jobject context);
JNI_METHOD(jstring, DeviceInfo_getManufacturer)(JNIEnv* env, jobject thiz);
JNI_METHOD(jstring, DeviceInfo_getModel)(JNIEnv* env, jobject thiz);
JNI_METHOD(jstring, DeviceInfo_getBrand)(JNIEnv* env, jobject thiz);
JNI_METHOD(jstring, DeviceInfo_getHardware)(JNIEnv* env, jobject thiz);
JNI_METHOD(jstring, DeviceInfo_getDevice)(JNIEnv* env, jobject thiz);
JNI_METHOD(jstring, DeviceInfo_getFingerprint)(JNIEnv* env, jobject thiz);
JNI_METHOD(jint, DeviceInfo_getSDKVersion)(JNIEnv* env, jobject thiz);

JNI_METHOD(jboolean, SecurityCheck_checkDebugger)(JNIEnv* env, jobject thiz);
JNI_METHOD(jboolean, SecurityCheck_checkRoot)(JNIEnv* env, jobject thiz);
JNI_METHOD(jboolean, SecurityCheck_checkEmulator)(JNIEnv* env, jobject thiz);
JNI_METHOD(jboolean, SecurityCheck_checkXposed)(JNIEnv* env, jobject thiz);
JNI_METHOD(jboolean, SecurityCheck_checkFrida)(JNIEnv* env, jobject thiz);
JNI_METHOD(jboolean, SecurityCheck_verifySignature)(JNIEnv* env, jobject thiz, jobject context);
JNI_METHOD(jint, SecurityCheck_performAllChecks)(JNIEnv* env, jobject thiz, jobject context);

JNI_METHOD(jstring, OAIDHelper_getOAID)(JNIEnv* env, jobject thiz, jobject context);
JNI_METHOD(jint, OAIDHelper_getOAIDProvider)(JNIEnv* env, jobject thiz);
JNI_METHOD(jboolean, OAIDHelper_isOAIDSupported)(JNIEnv* env, jobject thiz);
JNI_METHOD(jboolean, OAIDHelper_isOAIDAvailable)(JNIEnv* env, jobject thiz, jobject context);
JNI_METHOD(jstring, OAIDHelper_getProviderName)(JNIEnv* env, jobject thiz, jint provider);

JNI_METHOD(jint, Library_init)(JNIEnv* env, jobject thiz, jobject context);
JNI_METHOD(void, Library_cleanup)(JNIEnv* env, jobject thiz);
JNI_METHOD(jstring, Library_getVersion)(JNIEnv* env, jobject thiz);
JNI_METHOD(jboolean, Library_isInitialized)(JNIEnv* env, jobject thiz);

jobject get_application_context(JNIEnv* env);
jobject get_current_activity_thread(JNIEnv* env);

#ifdef __cplusplus
}
#endif

#endif // LIBMSAOAIDSEC_JNI_INTERFACE_H
