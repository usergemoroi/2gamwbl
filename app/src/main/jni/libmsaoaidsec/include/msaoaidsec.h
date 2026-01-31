#ifndef LIBMSAOAIDSEC_H
#define LIBMSAOAIDSEC_H

#include <jni.h>
#include <stdint.h>
#include <stdbool.h>

#include "types.h"
#include "constants.h"
#include "utils.h"
#include "security.h"
#include "device_id.h"
#include "oaid_interface.h"
#include "jni_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

int msaoaidsec_init(JNIEnv* env, jobject context);
void msaoaidsec_cleanup();
bool msaoaidsec_is_initialized();
const char* msaoaidsec_get_version();

LibraryState* get_library_state();
void set_library_state(LibraryState* state);

void* get_java_vm();
void set_java_vm(void* vm);

void* get_application_context_cached();
void set_application_context_cached(void* context);

#ifdef __cplusplus
}
#endif

#endif // LIBMSAOAIDSEC_H
