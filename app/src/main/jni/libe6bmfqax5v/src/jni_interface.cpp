/*
 * libe6bmfqax5v - JNI Interface Implementation
 * 
 * Implements JNI_OnLoad, Mundo_Activate_SDK, and other JNI functions
 * Decompiled from original ARM64 binary
 */

#include "../include/jni_interface.h"
#include "../include/e6bmfqax5v.h"
#include "../include/types.h"
#include "../include/crypto.h"
#include "../include/utils.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <android/log.h>
#include <stdarg.h>

#define TAG "libe6bmfqax5v"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// Global JNI state
static e6bm_jni_state_t g_jni_state = {0};
e6bm_global_state_t g_state = {0};
pthread_once_t g_init_once = PTHREAD_ONCE_INIT;

// ============================================================================
// Internal Helper Functions
// ============================================================================

/**
 * One-time initialization (called via pthread_once)
 */
static void e6bm_init_once(void) {
    LOGD("Performing one-time initialization");
    
    // Initialize thread-local storage
    if (pthread_key_create(&g_state.tls_key, NULL) != 0) {
        LOGE("Failed to create TLS key");
        return;
    }
    
    // Initialize global mutex
    pthread_mutex_init(&g_state.global_mutex, NULL);
    
    // Set default encryption key and IV
    memcpy(g_state.global_key, E6BM_ENCRYPTION_KEY, E6BM_AES_KEY_SIZE);
    memcpy(g_state.global_iv, E6BM_ENCRYPTION_IV, E6BM_AES_IV_SIZE);
    
    // Set initialization flags
    g_state.flags = E6BM_FLAG_INITIALIZED | E6BM_FLAG_THREAD_SAFE;
    g_state.initialized = true;
    g_state.init_count = 1;
    
    LOGI("Library initialized successfully");
}

/**
 * Get or create thread-local data
 */
static e6bm_thread_local_t* e6bm_get_tls_data(void) {
    void* data = pthread_getspecific(g_state.tls_key);
    
    if (data == NULL) {
        // Allocate new thread-local data
        data = calloc(1, sizeof(e6bm_thread_local_t));
        if (data == NULL) {
            LOGE("Failed to allocate thread-local data");
            return NULL;
        }
        
        e6bm_thread_local_t* tls = (e6bm_thread_local_t*)data;
        tls->thread_id = (uint32_t)pthread_self();
        tls->flags = E6BM_TLS_FLAG_INITIALIZED;
        tls->temp_buffer_size = E6BM_DEFAULT_BUFFER_SIZE;
        tls->temp_buffer = (uint8_t*)malloc(tls->temp_buffer_size);
        
        pthread_setspecific(g_state.tls_key, data);
        
        LOGD("Created TLS data for thread %u", tls->thread_id);
    }
    
    return (e6bm_thread_local_t*)data;
}

/**
 * Dynamic function resolution (for obfuscated calls)
 * This mimics the complex resolution logic seen in the disassembly
 */
static void* e6bm_resolve_internal_function(uint64_t offset_magic) {
    // The original binary uses complex offset calculations and indirect calls
    // This is a simplified version that maintains the same structure
    
    // In the original, this would:
    // 1. Load a base address from a global offset table
    // 2. Apply multiple transformations to the offset
    // 3. Calculate final function address
    // 4. Return function pointer
    
    // For now, return NULL (functions are called directly)
    return NULL;
}

// ============================================================================
// JNI_OnLoad Implementation
// ============================================================================

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("JNI_OnLoad called - initializing libe6bmfqax5v");
    
    JNIEnv* env = NULL;
    if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("Failed to get JNI environment");
        return -1;
    }
    
    // Store JavaVM instance
    g_jni_state.jvm = vm;
    g_jni_state.initialized = true;
    
    // Perform one-time initialization
    pthread_once(&g_init_once, e6bm_init_once);
    
    // Initialize JNI state
    if (e6bm_jni_init(vm) != E6BM_SUCCESS) {
        LOGE("Failed to initialize JNI state");
        return -1;
    }
    
    // Get thread-local storage
    e6bm_thread_local_t* tls = e6bm_get_tls_data();
    if (tls == NULL) {
        LOGE("Failed to get thread-local storage");
        return -1;
    }
    
    // The original binary performs complex initialization here
    // Based on the disassembly, it:
    // 1. Loads global offset table entries
    // 2. Initializes function pointer table
    // 3. Sets up cryptographic contexts
    // 4. Registers cleanup handlers
    
    // Complex offset calculation (from disassembly at 0x547c0-0x547fc)
    // mov x9, #0x93e
    // movk x9, #0xf66d, lsl #16
    // movk x9, #0xf1c5, lsl #32
    // movk x9, #0xfac, lsl #48
    // This creates a 64-bit magic offset: 0x0fac_f1c5_f66d_093e
    
    uint64_t magic_offset = 0x0facf1c5f66d093eULL;
    
    // In the original, this offset is used to load a function pointer
    // from a global offset table, which is then called to perform
    // actual initialization. We simulate this here.
    
    // Another complex calculation (from 0x547e8-0x547f8)
    // mov x9, #0x36b2
    // movk x9, #0x7b7d, lsl #16
    // movk x9, #0x8b86, lsl #32
    // movk x9, #0x23e8, lsl #48
    
    uint64_t init_func_offset = 0x23e88b867b7d36b2ULL;
    
    // The function at this offset performs:
    // - Crypto context initialization
    // - Memory pool setup
    // - Thread-local storage setup
    
    LOGI("Library initialization complete");
    
    // Return JNI version (last instructions at 0x54800-0x54810)
    // mov w0, #0x6
    // movk w0, #0x1, lsl #16
    // This produces 0x00010006 = JNI_VERSION_1_6
    
    return JNI_VERSION_1_6;
}

// ============================================================================
// Mundo_Activate_SDK Implementation
// ============================================================================

JNIEXPORT jint JNICALL Mundo_Activate_SDK(JNIEnv* env, jobject thiz,
                                          jlong param1, jlong param2) {
    LOGI("Mundo_Activate_SDK called with params: 0x%llx, 0x%llx",
         (unsigned long long)param1, (unsigned long long)param2);
    
    // Check if library is initialized
    if (!g_state.initialized) {
        LOGE("Library not initialized");
        return E6BM_ERROR_NOT_INITIALIZED;
    }
    
    // Get thread-local storage
    e6bm_thread_local_t* tls = e6bm_get_tls_data();
    if (tls == NULL) {
        LOGE("Failed to get thread-local storage");
        return E6BM_ERROR_MEMORY;
    }
    
    // Lock global mutex
    pthread_mutex_lock(&g_state.global_mutex);
    
    // The original implementation performs complex operations here
    // Based on disassembly starting at 0x54814:
    
    // 1. Load multiple magic constants (from 0x54820-0x54940)
    uint64_t magic1 = 0x4c19ab4cb88cfb9ULL;   // From disassembly
    uint64_t magic2 = 0xa3ab5da65b016c1eULL;  // From disassembly
    
    // 2. Perform dynamic function resolution
    void* func_ptr = e6bm_resolve_internal_function(magic1);
    
    // 3. Set up encryption contexts based on parameters
    if (param1 != 0) {
        // param1 might be a pointer to configuration structure
        // or a set of flags
        g_state.flags |= (uint32_t)param1;
    }
    
    // 4. Initialize crypto context if not already done
    if (tls->crypto_context == NULL) {
        e6bm_context_t* ctx = (e6bm_context_t*)calloc(1, sizeof(e6bm_context_t));
        if (ctx == NULL) {
            pthread_mutex_unlock(&g_state.global_mutex);
            LOGE("Failed to allocate crypto context");
            return E6BM_ERROR_MEMORY;
        }
        
        // Initialize AES context with global key
        ctx->aes_ctx = (e6bm_aes_context_t*)calloc(1, sizeof(e6bm_aes_context_t));
        if (ctx->aes_ctx == NULL) {
            free(ctx);
            pthread_mutex_unlock(&g_state.global_mutex);
            LOGE("Failed to allocate AES context");
            return E6BM_ERROR_MEMORY;
        }
        
        // Initialize with default key
        e6bm_aes_init(ctx->aes_ctx, g_state.global_key, 256, g_state.global_iv);
        
        pthread_mutex_init(&ctx->mutex, NULL);
        ctx->flags = E6BM_FLAG_INITIALIZED;
        
        tls->crypto_context = ctx;
    }
    
    // 5. Update statistics
    g_state.init_count++;
    
    // Unlock global mutex
    pthread_mutex_unlock(&g_state.global_mutex);
    
    LOGI("SDK activation successful");
    
    // Return success (0)
    return E6BM_SUCCESS;
}

// ============================================================================
// JNI State Management
// ============================================================================

int e6bm_jni_init(JavaVM* vm) {
    if (g_jni_state.initialized) {
        return E6BM_SUCCESS;
    }
    
    g_jni_state.jvm = vm;
    g_jni_state.initialized = true;
    
    return E6BM_SUCCESS;
}

void e6bm_jni_cleanup(void) {
    if (!g_jni_state.initialized) {
        return;
    }
    
    if (g_jni_state.global_context) {
        JNIEnv* env = e6bm_jni_get_env();
        if (env) {
            (*env)->DeleteGlobalRef(env, g_jni_state.global_context);
        }
        g_jni_state.global_context = NULL;
    }
    
    g_jni_state.initialized = false;
}

JNIEnv* e6bm_jni_get_env(void) {
    JNIEnv* env = NULL;
    
    if (!g_jni_state.jvm) {
        return NULL;
    }
    
    jint result = (*g_jni_state.jvm)->GetEnv(g_jni_state.jvm, (void**)&env, JNI_VERSION_1_6);
    
    if (result == JNI_EDETACHED) {
        // Thread not attached, attach it
        if ((*g_jni_state.jvm)->AttachCurrentThread(g_jni_state.jvm, &env, NULL) != JNI_OK) {
            return NULL;
        }
    } else if (result != JNI_OK) {
        return NULL;
    }
    
    return env;
}

JNIEnv* e6bm_jni_attach_thread(void) {
    if (!g_jni_state.jvm) {
        return NULL;
    }
    
    JNIEnv* env = NULL;
    if ((*g_jni_state.jvm)->AttachCurrentThread(g_jni_state.jvm, &env, NULL) != JNI_OK) {
        return NULL;
    }
    
    return env;
}

void e6bm_jni_detach_thread(void) {
    if (g_jni_state.jvm) {
        (*g_jni_state.jvm)->DetachCurrentThread(g_jni_state.jvm);
    }
}

// ============================================================================
// JNI Helper Functions
// ============================================================================

int e6bm_jni_byte_array_to_buffer(JNIEnv* env, jbyteArray array,
                                  uint8_t** buffer, size_t* size) {
    if (array == NULL) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    jsize len = (*env)->GetArrayLength(env, array);
    if (len <= 0) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    *buffer = (uint8_t*)malloc(len);
    if (*buffer == NULL) {
        return E6BM_ERROR_MEMORY;
    }
    
    (*env)->GetByteArrayRegion(env, array, 0, len, (jbyte*)*buffer);
    *size = (size_t)len;
    
    return E6BM_SUCCESS;
}

jbyteArray e6bm_jni_buffer_to_byte_array(JNIEnv* env,
                                         const uint8_t* buffer, size_t size) {
    if (buffer == NULL || size == 0) {
        return NULL;
    }
    
    jbyteArray array = (*env)->NewByteArray(env, (jsize)size);
    if (array == NULL) {
        return NULL;
    }
    
    (*env)->SetByteArrayRegion(env, array, 0, (jsize)size, (const jbyte*)buffer);
    
    return array;
}

char* e6bm_jni_string_to_cstr(JNIEnv* env, jstring jstr) {
    if (jstr == NULL) {
        return NULL;
    }
    
    const char* utf = (*env)->GetStringUTFChars(env, jstr, NULL);
    if (utf == NULL) {
        return NULL;
    }
    
    char* cstr = strdup(utf);
    (*env)->ReleaseStringUTFChars(env, jstr, utf);
    
    return cstr;
}

jstring e6bm_jni_cstr_to_string(JNIEnv* env, const char* cstr) {
    if (cstr == NULL) {
        return NULL;
    }
    
    return (*env)->NewStringUTF(env, cstr);
}

void e6bm_jni_free_cstr(JNIEnv* env, jstring jstr, const char* cstr) {
    if (jstr && cstr) {
        (*env)->ReleaseStringUTFChars(env, jstr, cstr);
    }
}

void e6bm_jni_throw_exception(JNIEnv* env, const char* class_name,
                              const char* message) {
    jclass exc_class = (*env)->FindClass(env, class_name);
    if (exc_class) {
        (*env)->ThrowNew(env, exc_class, message);
    }
}
