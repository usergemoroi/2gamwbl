/*
 * libe6bmfqax5v - Initialization and Cleanup
 */

#include "../include/e6bmfqax5v.h"
#include "../include/types.h"
#include "../include/crypto.h"
#include "../include/utils.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// Global initialization state
extern e6bm_global_state_t g_state;
extern pthread_once_t g_init_once;

// TLS cleanup callback
static void e6bm_tls_destructor(void* ptr) {
    if (ptr) {
        e6bm_thread_local_t* tls = (e6bm_thread_local_t*)ptr;
        
        if (tls->temp_buffer) {
            free(tls->temp_buffer);
        }
        
        if (tls->crypto_context) {
            e6bm_crypto_context_free(tls->crypto_context);
        }
        
        free(tls);
    }
}

int e6bm_tls_init(void) {
    return pthread_key_create(&g_state.tls_key, e6bm_tls_destructor);
}

void e6bm_tls_cleanup(void) {
    pthread_key_delete(g_state.tls_key);
}

void* e6bm_tls_get(void) {
    return pthread_getspecific(g_state.tls_key);
}

int e6bm_tls_set(void* data) {
    return pthread_setspecific(g_state.tls_key, data);
}

int e6bm_crypto_context_init(void** ctx) {
    if (ctx == NULL) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    e6bm_context_t* context = (e6bm_context_t*)calloc(1, sizeof(e6bm_context_t));
    if (context == NULL) {
        return E6BM_ERROR_MEMORY;
    }
    
    pthread_mutex_init(&context->mutex, NULL);
    context->flags = E6BM_FLAG_INITIALIZED;
    
    *ctx = context;
    return E6BM_SUCCESS;
}

void e6bm_crypto_context_free(void* ctx) {
    if (ctx == NULL) {
        return;
    }
    
    e6bm_context_t* context = (e6bm_context_t*)ctx;
    
    if (context->aes_ctx) {
        e6bm_secure_memzero(context->aes_ctx, sizeof(e6bm_aes_context_t));
        free(context->aes_ctx);
    }
    
    if (context->md5_ctx) {
        free(context->md5_ctx);
    }
    
    if (context->sha256_ctx) {
        free(context->sha256_ctx);
    }
    
    if (context->zlib_ctx) {
        free(context->zlib_ctx);
    }
    
    pthread_mutex_destroy(&context->mutex);
    free(context);
}
