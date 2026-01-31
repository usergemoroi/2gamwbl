#include "msaoaidsec.h"
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>

bool detect_debugger_via_tracerpid() {
    char* tracer_pid_str = read_file_line(TRACERPID_PATH, DEBUG_MARKER);
    if (!tracer_pid_str) return false;
    
    int tracer_pid = atoi(tracer_pid_str);
    free(tracer_pid_str);
    
    return tracer_pid != 0;
}

bool detect_debugger_via_ptrace() {
    pid_t pid = fork();
    if (pid == -1) return false;
    
    if (pid == 0) {
        pid_t parent = getppid();
        if (ptrace(PTRACE_ATTACH, parent, nullptr, nullptr) == -1) {
            _exit(1);
        }
        waitpid(parent, nullptr, 0);
        ptrace(PTRACE_DETACH, parent, nullptr, nullptr);
        _exit(0);
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status) != 0;
    }
}

bool detect_debugger_via_timing() {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    volatile int x = 0;
    for (int i = 0; i < 1000; i++) {
        x += i;
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
    
    return elapsed_ns > 10000000;
}

bool detect_debugger() {
    if (detect_debugger_via_tracerpid()) {
        log_warn(LOG_TAG, "Debugger detected via TracerPid");
        return true;
    }
    
    if (detect_debugger_via_ptrace()) {
        log_warn(LOG_TAG, "Debugger detected via ptrace");
        return true;
    }
    
    return false;
}

bool detect_root_via_su_binary() {
    const char* su_paths[] = { SU_BINARY_PATHS };
    int path_count = sizeof(su_paths) / sizeof(su_paths[0]);
    
    for (int i = 0; i < path_count; i++) {
        if (file_exists(su_paths[i])) {
            log_warn(LOG_TAG, "Root detected: found su binary at %s", su_paths[i]);
            return true;
        }
    }
    
    return false;
}

bool detect_root_via_superuser_apk() {
    const char* superuser_apks[] = { SUPERUSER_APKS };
    int apk_count = sizeof(superuser_apks) / sizeof(superuser_apks[0]);
    
    for (int i = 0; i < apk_count; i++) {
        char path[MAX_PATH_LENGTH];
        snprintf(path, sizeof(path), "/data/data/%s", superuser_apks[i]);
        
        if (directory_exists(path)) {
            log_warn(LOG_TAG, "Root detected: found superuser APK %s", superuser_apks[i]);
            return true;
        }
    }
    
    return false;
}

bool detect_root_via_build_tags() {
    char* build_tags = get_system_property("ro.build.tags");
    if (!build_tags) return false;
    
    bool is_test_keys = contains(build_tags, "test-keys");
    free(build_tags);
    
    if (is_test_keys) {
        log_warn(LOG_TAG, "Root detected: ro.build.tags contains test-keys");
        return true;
    }
    
    return false;
}

bool detect_root_via_system_properties() {
    char* ro_secure = get_system_property("ro.secure");
    if (ro_secure) {
        int secure = atoi(ro_secure);
        free(ro_secure);
        
        if (secure == 0) {
            log_warn(LOG_TAG, "Root detected: ro.secure is 0");
            return true;
        }
    }
    
    char* ro_debuggable = get_system_property("ro.debuggable");
    if (ro_debuggable) {
        int debuggable = atoi(ro_debuggable);
        free(ro_debuggable);
        
        if (debuggable == 1) {
            log_warn(LOG_TAG, "Root detected: ro.debuggable is 1");
            return true;
        }
    }
    
    return false;
}

bool detect_root() {
    if (detect_root_via_su_binary()) return true;
    if (detect_root_via_superuser_apk()) return true;
    if (detect_root_via_build_tags()) return true;
    if (detect_root_via_system_properties()) return true;
    
    return false;
}

bool detect_emulator_via_files() {
    const char* emulator_files[] = { EMULATOR_FILES };
    int file_count = sizeof(emulator_files) / sizeof(emulator_files[0]);
    
    for (int i = 0; i < file_count; i++) {
        if (file_exists(emulator_files[i])) {
            log_warn(LOG_TAG, "Emulator detected: found file %s", emulator_files[i]);
            return true;
        }
    }
    
    return false;
}

bool detect_emulator_via_properties() {
    const char* emulator_properties[][2] = {
        {"ro.kernel.qemu", "1"},
        {"ro.hardware", "goldfish"},
        {"ro.hardware", "ranchu"},
        {"ro.product.device", "generic"},
        {"ro.build.product", "sdk"}
    };
    
    int prop_count = sizeof(emulator_properties) / sizeof(emulator_properties[0]);
    
    for (int i = 0; i < prop_count; i++) {
        char* value = get_system_property(emulator_properties[i][0]);
        if (value) {
            bool matches = contains(value, emulator_properties[i][1]);
            free(value);
            
            if (matches) {
                log_warn(LOG_TAG, "Emulator detected: property %s contains %s", 
                        emulator_properties[i][0], emulator_properties[i][1]);
                return true;
            }
        }
    }
    
    return false;
}

bool detect_emulator_via_build_info() {
    char* manufacturer = get_manufacturer();
    char* model = get_model();
    char* brand = get_brand();
    
    const char* emulator_brands[] = { EMULATOR_BRANDS };
    int brand_count = sizeof(emulator_brands) / sizeof(emulator_brands[0]);
    
    bool detected = false;
    
    for (int i = 0; i < brand_count; i++) {
        if ((manufacturer && strcasecmp(manufacturer, emulator_brands[i]) == 0) ||
            (model && strcasecmp(model, emulator_brands[i]) == 0) ||
            (brand && strcasecmp(brand, emulator_brands[i]) == 0)) {
            log_warn(LOG_TAG, "Emulator detected: build info matches %s", emulator_brands[i]);
            detected = true;
            break;
        }
    }
    
    free(manufacturer);
    free(model);
    free(brand);
    
    return detected;
}

bool detect_emulator_via_cpu_info() {
    char* cpu_info = read_file("/proc/cpuinfo");
    if (!cpu_info) return false;
    
    bool is_emulator = contains(cpu_info, "goldfish") || 
                       contains(cpu_info, "ranchu") ||
                       contains(cpu_info, "vbox");
    
    free(cpu_info);
    
    if (is_emulator) {
        log_warn(LOG_TAG, "Emulator detected via CPU info");
    }
    
    return is_emulator;
}

bool detect_emulator_via_hardware() {
    char* hardware = get_hardware();
    if (!hardware) return false;
    
    bool is_emulator = contains(hardware, "goldfish") || 
                       contains(hardware, "ranchu") ||
                       contains(hardware, "vbox") ||
                       strcasecmp(hardware, "android_x86") == 0;
    
    free(hardware);
    
    if (is_emulator) {
        log_warn(LOG_TAG, "Emulator detected via hardware");
    }
    
    return is_emulator;
}

bool detect_emulator() {
    if (detect_emulator_via_files()) return true;
    if (detect_emulator_via_properties()) return true;
    if (detect_emulator_via_build_info()) return true;
    if (detect_emulator_via_cpu_info()) return true;
    if (detect_emulator_via_hardware()) return true;
    
    return false;
}

bool detect_xposed_via_class_loading() {
    return false;
}

bool detect_xposed_via_stack_trace() {
    return false;
}

bool detect_xposed_via_method_hooking() {
    return false;
}

bool detect_xposed() {
    char* maps = read_file(MAPS_PATH);
    if (!maps) return false;
    
    bool detected = contains(maps, "XposedBridge.jar") || 
                    contains(maps, "XposedInstaller");
    
    free(maps);
    
    if (detected) {
        log_warn(LOG_TAG, "Xposed detected via memory maps");
    }
    
    return detected;
}

bool detect_frida_via_library() {
    const char* frida_libs[] = { FRIDA_LIBRARIES };
    int lib_count = sizeof(frida_libs) / sizeof(frida_libs[0]);
    
    for (int i = 0; i < lib_count; i++) {
        if (is_library_loaded(frida_libs[i])) {
            log_warn(LOG_TAG, "Frida detected: loaded library %s", frida_libs[i]);
            return true;
        }
    }
    
    return false;
}

bool detect_frida_via_thread() {
    const char* frida_threads[] = { FRIDA_THREADS };
    int thread_count = sizeof(frida_threads) / sizeof(frida_threads[0]);
    
    for (int i = 0; i < thread_count; i++) {
        if (is_thread_running(frida_threads[i])) {
            log_warn(LOG_TAG, "Frida detected: found thread %s", frida_threads[i]);
            return true;
        }
    }
    
    return false;
}

bool detect_frida_via_port() {
    if (file_exists("/tmp/frida-server.sock")) {
        log_warn(LOG_TAG, "Frida detected: found frida-server socket");
        return true;
    }
    
    char* tcp = read_file("/proc/net/tcp");
    if (tcp) {
        bool detected = contains(tcp, ":69A7");
        free(tcp);
        
        if (detected) {
            log_warn(LOG_TAG, "Frida detected: default port 27047 in use");
            return true;
        }
    }
    
    return false;
}

bool detect_frida_via_maps() {
    char* maps = read_file(MAPS_PATH);
    if (!maps) return false;
    
    bool detected = contains(maps, "frida");
    free(maps);
    
    if (detected) {
        log_warn(LOG_TAG, "Frida detected via memory maps");
    }
    
    return detected;
}

bool detect_frida() {
    if (detect_frida_via_library()) return true;
    if (detect_frida_via_thread()) return true;
    if (detect_frida_via_port()) return true;
    if (detect_frida_via_maps()) return true;
    
    return false;
}

bool verify_apk_signature(JNIEnv* env, jobject context) {
    return true;
}

bool verify_apk_signature_via_package_manager(JNIEnv* env, jobject context) {
    return true;
}

bool check_installer_package(JNIEnv* env, jobject context) {
    return true;
}

bool check_memory_integrity() {
    return true;
}

bool check_code_integrity() {
    return true;
}

bool check_library_integrity() {
    return true;
}

SecurityCheckResult* perform_security_check(JNIEnv* env, jobject context, int32_t checks) {
    SecurityCheckResult* result = create_security_check_result();
    if (!result) return nullptr;
    
    if (checks & SECURITY_CHECK_DEBUGGER) {
        result->debugger_detected = detect_debugger();
    }
    
    if (checks & SECURITY_CHECK_ROOT) {
        result->root_detected = detect_root();
    }
    
    if (checks & SECURITY_CHECK_EMULATOR) {
        result->emulator_detected = detect_emulator();
    }
    
    if (checks & SECURITY_CHECK_XPOSED) {
        result->xposed_detected = detect_xposed();
    }
    
    if (checks & SECURITY_CHECK_FRIDA) {
        result->frida_detected = detect_frida();
    }
    
    if (checks & SECURITY_CHECK_SIGNATURE) {
        result->signature_valid = verify_apk_signature(env, context);
    }
    
    if (checks & SECURITY_CHECK_MEMORY) {
        result->memory_tampered = !check_memory_integrity();
    }
    
    result->check_count++;
    result->timestamp = get_current_timestamp_ms();
    
    return result;
}

SecurityCheckResult* perform_full_security_check(JNIEnv* env, jobject context) {
    return perform_security_check(env, context, SECURITY_CHECK_ALL);
}

bool is_running_in_secure_environment(JNIEnv* env, jobject context) {
    SecurityCheckResult* result = perform_full_security_check(env, context);
    if (!result) return false;
    
    bool is_secure = !result->debugger_detected &&
                     !result->root_detected &&
                     !result->emulator_detected &&
                     !result->xposed_detected &&
                     !result->frida_detected &&
                     result->signature_valid &&
                     !result->memory_tampered;
    
    free_security_check_result(result);
    return is_secure;
}

int32_t get_security_level(SecurityCheckResult* result) {
    if (!result) return 0;
    
    int32_t level = 100;
    
    if (result->debugger_detected) level -= 20;
    if (result->root_detected) level -= 25;
    if (result->emulator_detected) level -= 15;
    if (result->xposed_detected) level -= 15;
    if (result->frida_detected) level -= 20;
    if (!result->signature_valid) level -= 30;
    if (result->memory_tampered) level -= 25;
    
    return level < 0 ? 0 : level;
}

void init_anti_debugging() {
    prctl(PR_SET_DUMPABLE, 0);
}

void init_anti_tampering() {
}

bool check_process_name() {
    return true;
}

bool check_package_signature() {
    return true;
}
