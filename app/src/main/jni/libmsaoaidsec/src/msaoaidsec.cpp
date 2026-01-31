#include "msaoaidsec.h"
#include <stdlib.h>
#include <string.h>

static LibraryState* g_library_state = nullptr;

LibraryState* get_library_state() {
    return g_library_state;
}

void set_library_state(LibraryState* state) {
    g_library_state = state;
}

void* get_java_vm() {
    return g_library_state ? g_library_state->jvm : nullptr;
}

void set_java_vm(void* vm) {
    if (!g_library_state) {
        g_library_state = (LibraryState*)calloc(1, sizeof(LibraryState));
    }
    g_library_state->jvm = vm;
}

void* get_application_context_cached() {
    return g_library_state ? g_library_state->context : nullptr;
}

void set_application_context_cached(void* context) {
    if (!g_library_state) {
        g_library_state = (LibraryState*)calloc(1, sizeof(LibraryState));
    }
    g_library_state->context = context;
}

int msaoaidsec_init(JNIEnv* env, jobject context) {
    if (!env) {
        log_error(LOG_TAG, "Invalid JNIEnv");
        return ERROR_INVALID_ARGUMENT;
    }
    
    if (g_library_state && g_library_state->initialized) {
        log_warn(LOG_TAG, "Library already initialized");
        g_library_state->init_count++;
        return ERROR_NONE;
    }
    
    if (!g_library_state) {
        g_library_state = (LibraryState*)calloc(1, sizeof(LibraryState));
        if (!g_library_state) {
            log_error(LOG_TAG, "Failed to allocate library state");
            return ERROR_OUT_OF_MEMORY;
        }
    }
    
    JavaVM* jvm;
    if (env->GetJavaVM(&jvm) != JNI_OK) {
        log_error(LOG_TAG, "Failed to get JavaVM");
        return ERROR_INIT_FAILED;
    }
    
    g_library_state->jvm = jvm;
    
    if (context) {
        g_library_state->context = env->NewGlobalRef(context);
    } else {
        g_library_state->context = env->NewGlobalRef(get_application_context(env));
    }
    
    if (!g_library_state->context) {
        log_error(LOG_TAG, "Failed to get application context");
        free(g_library_state);
        g_library_state = nullptr;
        return ERROR_INIT_FAILED;
    }
    
    g_library_state->initialized = true;
    g_library_state->init_count = 1;
    
    log_info(LOG_TAG, "libmsaoaidsec v%s initialized successfully", MSAOAIDSEC_VERSION);
    
    return ERROR_NONE;
}

void msaoaidsec_cleanup() {
    if (!g_library_state) {
        return;
    }
    
    if (g_library_state->init_count > 1) {
        g_library_state->init_count--;
        log_info(LOG_TAG, "Cleanup called, init count: %d", g_library_state->init_count);
        return;
    }
    
    log_info(LOG_TAG, "Cleaning up library state");
    
    clear_device_info_cache();
    clear_oaid_info_cache();
    
    if (g_library_state->context && g_library_state->jvm) {
        JNIEnv* env;
        JavaVM* jvm = (JavaVM*)g_library_state->jvm;
        
        if (jvm->GetEnv((void**)&env, JNI_VERSION_1_6) == JNI_OK) {
            env->DeleteGlobalRef((jobject)g_library_state->context);
        }
    }
    
    if (g_library_state->cached_device_info) {
        free_device_info(g_library_state->cached_device_info);
    }
    
    if (g_library_state->cached_oaid_info) {
        free_oaid_info(g_library_state->cached_oaid_info);
    }
    
    free(g_library_state);
    g_library_state = nullptr;
    
    log_info(LOG_TAG, "Library cleaned up");
}

bool msaoaidsec_is_initialized() {
    return g_library_state && g_library_state->initialized;
}

const char* msaoaidsec_get_version() {
    return MSAOAIDSEC_VERSION;
}
