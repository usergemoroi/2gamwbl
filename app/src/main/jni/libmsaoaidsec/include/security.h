#ifndef LIBMSAOAIDSEC_SECURITY_H
#define LIBMSAOAIDSEC_SECURITY_H

#include <jni.h>
#include <stdbool.h>
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

bool detect_debugger();
bool detect_debugger_via_tracerpid();
bool detect_debugger_via_ptrace();
bool detect_debugger_via_timing();

bool detect_root();
bool detect_root_via_su_binary();
bool detect_root_via_superuser_apk();
bool detect_root_via_build_tags();
bool detect_root_via_system_properties();

bool detect_emulator();
bool detect_emulator_via_files();
bool detect_emulator_via_properties();
bool detect_emulator_via_build_info();
bool detect_emulator_via_cpu_info();
bool detect_emulator_via_hardware();

bool detect_xposed();
bool detect_xposed_via_class_loading();
bool detect_xposed_via_stack_trace();
bool detect_xposed_via_method_hooking();

bool detect_frida();
bool detect_frida_via_library();
bool detect_frida_via_thread();
bool detect_frida_via_port();
bool detect_frida_via_maps();

bool verify_apk_signature(JNIEnv* env, jobject context);
bool verify_apk_signature_via_package_manager(JNIEnv* env, jobject context);
bool check_installer_package(JNIEnv* env, jobject context);

bool check_memory_integrity();
bool check_code_integrity();
bool check_library_integrity();

SecurityCheckResult* perform_security_check(JNIEnv* env, jobject context, int32_t checks);
SecurityCheckResult* perform_full_security_check(JNIEnv* env, jobject context);

bool is_running_in_secure_environment(JNIEnv* env, jobject context);
int32_t get_security_level(SecurityCheckResult* result);

void init_anti_debugging();
void init_anti_tampering();

bool check_process_name();
bool check_package_signature();

#ifdef __cplusplus
}
#endif

#endif // LIBMSAOAIDSEC_SECURITY_H
