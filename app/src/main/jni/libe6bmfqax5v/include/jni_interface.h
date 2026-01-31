/*
 * libe6bmfqax5v - JNI Interface
 * 
 * JNI method definitions and helper macros
 */

#ifndef E6BMFQAX5V_JNI_INTERFACE_H
#define E6BMFQAX5V_JNI_INTERFACE_H

#include <jni.h>
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// JNI Helper Macros
// ============================================================================

// JNI method name construction macro
#define JAVA_METHOD(return_type, class_name, method_name) \
    JNIEXPORT return_type JNICALL \
    Java_##class_name##_##method_name

// JNI class path conversion (dots to underscores)
#define JNI_CLASS_PATH(path) path

// Common JNI error checking
#define JNI_CHECK_NULL(env, obj, ret) \
    if (!(obj)) { \
        jclass exc = (*env)->FindClass(env, "java/lang/NullPointerException"); \
        if (exc) (*env)->ThrowNew(env, exc, "Null pointer argument"); \
        return (ret); \
    }

#define JNI_CHECK_EXCEPTION(env, ret) \
    if ((*env)->ExceptionCheck(env)) { \
        (*env)->ExceptionDescribe(env); \
        (*env)->ExceptionClear(env); \
        return (ret); \
    }

// ============================================================================
// JNI State Management
// ============================================================================

// JNI environment cache
typedef struct {
    JavaVM* jvm;
    jobject global_context;
    jmethodID callback_method;
    bool initialized;
} e6bm_jni_state_t;

/**
 * Initialize JNI state
 */
int e6bm_jni_init(JavaVM* vm);

/**
 * Cleanup JNI state
 */
void e6bm_jni_cleanup(void);

/**
 * Get JNI environment for current thread
 */
JNIEnv* e6bm_jni_get_env(void);

/**
 * Attach current thread to JVM
 */
JNIEnv* e6bm_jni_attach_thread(void);

/**
 * Detach current thread from JVM
 */
void e6bm_jni_detach_thread(void);

// ============================================================================
// JNI Helper Functions
// ============================================================================

/**
 * Convert jbyteArray to native buffer
 */
int e6bm_jni_byte_array_to_buffer(JNIEnv* env, jbyteArray array,
                                  uint8_t** buffer, size_t* size);

/**
 * Convert native buffer to jbyteArray
 */
jbyteArray e6bm_jni_buffer_to_byte_array(JNIEnv* env,
                                         const uint8_t* buffer, size_t size);

/**
 * Convert jstring to native string
 */
char* e6bm_jni_string_to_cstr(JNIEnv* env, jstring jstr);

/**
 * Convert native string to jstring
 */
jstring e6bm_jni_cstr_to_string(JNIEnv* env, const char* cstr);

/**
 * Free native string
 */
void e6bm_jni_free_cstr(JNIEnv* env, jstring jstr, const char* cstr);

/**
 * Throw Java exception
 */
void e6bm_jni_throw_exception(JNIEnv* env, const char* class_name,
                              const char* message);

/**
 * Get field value (int)
 */
jint e6bm_jni_get_int_field(JNIEnv* env, jobject obj, const char* field_name);

/**
 * Set field value (int)
 */
void e6bm_jni_set_int_field(JNIEnv* env, jobject obj,
                            const char* field_name, jint value);

/**
 * Get field value (long)
 */
jlong e6bm_jni_get_long_field(JNIEnv* env, jobject obj, const char* field_name);

/**
 * Set field value (long)
 */
void e6bm_jni_set_long_field(JNIEnv* env, jobject obj,
                             const char* field_name, jlong value);

/**
 * Call Java method (void return)
 */
void e6bm_jni_call_void_method(JNIEnv* env, jobject obj,
                               const char* method_name, const char* signature, ...);

/**
 * Call Java method (int return)
 */
jint e6bm_jni_call_int_method(JNIEnv* env, jobject obj,
                              const char* method_name, const char* signature, ...);

/**
 * Find class
 */
jclass e6bm_jni_find_class(JNIEnv* env, const char* class_name);

/**
 * Create global reference
 */
jobject e6bm_jni_new_global_ref(JNIEnv* env, jobject obj);

/**
 * Delete global reference
 */
void e6bm_jni_delete_global_ref(JNIEnv* env, jobject obj);

// ============================================================================
// Exported JNI Functions
// ============================================================================

/**
 * JNI_OnLoad - Called when library is loaded
 * This function is automatically called by the JVM when System.loadLibrary()
 * is invoked from Java code.
 * 
 * Responsibilities:
 * - Initialize global state
 * - Register native methods
 * - Set up thread-local storage
 * - Initialize cryptographic contexts
 * - Return supported JNI version
 * 
 * @param vm JavaVM instance
 * @param reserved Reserved parameter (unused)
 * @return JNI version (JNI_VERSION_1_6)
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved);

/**
 * JNI_OnUnload - Called when library is unloaded
 * 
 * @param vm JavaVM instance
 * @param reserved Reserved parameter (unused)
 */
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved);

/**
 * Mundo_Activate_SDK - SDK activation and initialization
 * This function is called from Java to activate the SDK and set up
 * encryption/decryption contexts.
 * 
 * @param env JNI environment
 * @param thiz Java object reference (this)
 * @param param1 Configuration parameter 1 (flags or config pointer)
 * @param param2 Configuration parameter 2 (reserved)
 * @return Status code (0 = success, negative = error)
 */
JNIEXPORT jint JNICALL Mundo_Activate_SDK(JNIEnv* env, jobject thiz,
                                          jlong param1, jlong param2);

// ============================================================================
// Additional Native Methods (if needed for Java binding)
// ============================================================================

/**
 * Native encrypt method
 */
JAVA_METHOD(jbyteArray, com_eternal_xdsdk_Crypto, nativeEncrypt)
    (JNIEnv* env, jobject thiz, jbyteArray input, jbyteArray key, jbyteArray iv);

/**
 * Native decrypt method
 */
JAVA_METHOD(jbyteArray, com_eternal_xdsdk_Crypto, nativeDecrypt)
    (JNIEnv* env, jobject thiz, jbyteArray input, jbyteArray key, jbyteArray iv);

/**
 * Native hash (MD5) method
 */
JAVA_METHOD(jbyteArray, com_eternal_xdsdk_Crypto, nativeMD5)
    (JNIEnv* env, jobject thiz, jbyteArray input);

/**
 * Native hash (SHA-256) method
 */
JAVA_METHOD(jbyteArray, com_eternal_xdsdk_Crypto, nativeSHA256)
    (JNIEnv* env, jobject thiz, jbyteArray input);

/**
 * Native compress method
 */
JAVA_METHOD(jbyteArray, com_eternal_xdsdk_Crypto, nativeCompress)
    (JNIEnv* env, jobject thiz, jbyteArray input, jint level);

/**
 * Native decompress method
 */
JAVA_METHOD(jbyteArray, com_eternal_xdsdk_Crypto, nativeDecompress)
    (JNIEnv* env, jobject thiz, jbyteArray input);

// ============================================================================
// Method Registration
// ============================================================================

// JNI native method structure
typedef struct {
    const char* name;
    const char* signature;
    void* fn_ptr;
} e6bm_jni_method_t;

/**
 * Register native methods
 */
int e6bm_jni_register_natives(JNIEnv* env, const char* class_name,
                              const e6bm_jni_method_t* methods, int num_methods);

/**
 * Unregister native methods
 */
void e6bm_jni_unregister_natives(JNIEnv* env, const char* class_name);

// ============================================================================
// JNI Callback Interface
// ============================================================================

/**
 * Register callback for progress updates
 */
void e6bm_jni_register_progress_callback(JNIEnv* env, jobject callback);

/**
 * Invoke progress callback
 */
void e6bm_jni_invoke_progress_callback(int progress, int total);

/**
 * Register error callback
 */
void e6bm_jni_register_error_callback(JNIEnv* env, jobject callback);

/**
 * Invoke error callback
 */
void e6bm_jni_invoke_error_callback(int error_code, const char* message);

// ============================================================================
// Constants
// ============================================================================

// JNI signature strings
#define JNI_SIG_VOID "()V"
#define JNI_SIG_INT "()I"
#define JNI_SIG_LONG "()J"
#define JNI_SIG_STRING "()Ljava/lang/String;"
#define JNI_SIG_BYTE_ARRAY "()[B"
#define JNI_SIG_INT_INT "(I)I"
#define JNI_SIG_LONG_LONG "(J)J"
#define JNI_SIG_BYTE_ARRAY_BYTE_ARRAY "([B)[B"

// Common Java exception classes
#define JNI_EXC_NULL_POINTER "java/lang/NullPointerException"
#define JNI_EXC_ILLEGAL_ARGUMENT "java/lang/IllegalArgumentException"
#define JNI_EXC_ILLEGAL_STATE "java/lang/IllegalStateException"
#define JNI_EXC_OUT_OF_MEMORY "java/lang/OutOfMemoryError"
#define JNI_EXC_RUNTIME "java/lang/RuntimeException"
#define JNI_EXC_IO "java/io/IOException"

#ifdef __cplusplus
}
#endif

#endif // E6BMFQAX5V_JNI_INTERFACE_H
