#include "msaoaidsec.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <android/log.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>

char* safe_strdup(const char* str) {
    if (!str) return nullptr;
    return strdup(str);
}

char* safe_strndup(const char* str, size_t n) {
    if (!str) return nullptr;
    return strndup(str, n);
}

void safe_free(void** ptr) {
    if (ptr && *ptr) {
        free(*ptr);
        *ptr = nullptr;
    }
}

bool file_exists(const char* path) {
    if (!path) return false;
    return access(path, F_OK) == 0;
}

bool is_readable(const char* path) {
    if (!path) return false;
    return access(path, R_OK) == 0;
}

bool directory_exists(const char* path) {
    if (!path) return false;
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

char* read_file(const char* path) {
    if (!path || !file_exists(path)) return nullptr;
    
    FILE* fp = fopen(path, "r");
    if (!fp) return nullptr;
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    if (size <= 0 || size > MAX_PATH_LENGTH * 10) {
        fclose(fp);
        return nullptr;
    }
    
    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        fclose(fp);
        return nullptr;
    }
    
    size_t read_size = fread(buffer, 1, size, fp);
    buffer[read_size] = '\0';
    fclose(fp);
    
    return buffer;
}

char* read_file_line(const char* path, const char* prefix) {
    if (!path || !prefix) return nullptr;
    
    FILE* fp = fopen(path, "r");
    if (!fp) return nullptr;
    
    char line[MAX_STRING_LENGTH];
    char* result = nullptr;
    
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, prefix, strlen(prefix)) == 0) {
            char* value = strchr(line, ':');
            if (value) {
                value++;
                while (*value == ' ' || *value == '\t') value++;
                char* end = strchr(value, '\n');
                if (end) *end = '\0';
                result = strdup(value);
                break;
            }
        }
    }
    
    fclose(fp);
    return result;
}

int read_file_to_buffer(const char* path, char* buffer, size_t size) {
    if (!path || !buffer || size == 0) return -1;
    
    FILE* fp = fopen(path, "r");
    if (!fp) return -1;
    
    size_t bytes_read = fread(buffer, 1, size - 1, fp);
    buffer[bytes_read] = '\0';
    fclose(fp);
    
    return (int)bytes_read;
}

bool starts_with(const char* str, const char* prefix) {
    if (!str || !prefix) return false;
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

bool ends_with(const char* str, const char* suffix) {
    if (!str || !suffix) return false;
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    if (suffix_len > str_len) return false;
    return strcmp(str + str_len - suffix_len, suffix) == 0;
}

bool contains(const char* str, const char* substr) {
    if (!str || !substr) return false;
    return strstr(str, substr) != nullptr;
}

char* get_system_property(const char* key) {
    if (!key) return nullptr;
    char cmd[MAX_STRING_LENGTH];
    snprintf(cmd, sizeof(cmd), "getprop %s", key);
    
    FILE* fp = popen(cmd, "r");
    if (!fp) return nullptr;
    
    char value[MAX_STRING_LENGTH];
    if (fgets(value, sizeof(value), fp)) {
        char* newline = strchr(value, '\n');
        if (newline) *newline = '\0';
        pclose(fp);
        return strdup(value);
    }
    
    pclose(fp);
    return nullptr;
}

bool set_system_property(const char* key, const char* value) {
    return false;
}

int32_t get_current_pid() {
    return getpid();
}

int32_t get_current_uid() {
    return getuid();
}

char* get_package_name() {
    char cmdline[MAX_STRING_LENGTH];
    if (read_file_to_buffer(CMDLINE_PATH, cmdline, sizeof(cmdline)) > 0) {
        return strdup(cmdline);
    }
    return nullptr;
}

char* get_process_name() {
    return get_package_name();
}

bool is_process_running(const char* process_name) {
    if (!process_name) return false;
    
    DIR* dir = opendir("/proc");
    if (!dir) return false;
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            char cmdline_path[MAX_PATH_LENGTH];
            snprintf(cmdline_path, sizeof(cmdline_path), "/proc/%s/cmdline", entry->d_name);
            
            char cmdline[MAX_STRING_LENGTH];
            if (read_file_to_buffer(cmdline_path, cmdline, sizeof(cmdline)) > 0) {
                if (strstr(cmdline, process_name)) {
                    closedir(dir);
                    return true;
                }
            }
        }
    }
    
    closedir(dir);
    return false;
}

bool is_library_loaded(const char* library_name) {
    if (!library_name) return false;
    
    char* maps = read_file(MAPS_PATH);
    if (!maps) return false;
    
    bool found = strstr(maps, library_name) != nullptr;
    free(maps);
    return found;
}

bool is_thread_running(const char* thread_name) {
    if (!thread_name) return false;
    
    DIR* task_dir = opendir("/proc/self/task");
    if (!task_dir) return false;
    
    struct dirent* entry;
    while ((entry = readdir(task_dir)) != nullptr) {
        if (isdigit(entry->d_name[0])) {
            char comm_path[MAX_PATH_LENGTH];
            snprintf(comm_path, sizeof(comm_path), "/proc/self/task/%s/comm", entry->d_name);
            
            char comm[256];
            if (read_file_to_buffer(comm_path, comm, sizeof(comm)) > 0) {
                char* newline = strchr(comm, '\n');
                if (newline) *newline = '\0';
                
                if (strstr(comm, thread_name)) {
                    closedir(task_dir);
                    return true;
                }
            }
        }
    }
    
    closedir(task_dir);
    return false;
}

char* jstring_to_string(JNIEnv* env, jstring jstr) {
    if (!env || !jstr) return nullptr;
    
    const char* str = env->GetStringUTFChars(jstr, nullptr);
    if (!str) return nullptr;
    
    char* result = strdup(str);
    env->ReleaseStringUTFChars(jstr, str);
    return result;
}

jstring string_to_jstring(JNIEnv* env, const char* str) {
    if (!env || !str) return nullptr;
    return env->NewStringUTF(str);
}

jbyteArray bytes_to_jbytearray(JNIEnv* env, const uint8_t* bytes, size_t len) {
    if (!env || !bytes) return nullptr;
    
    jbyteArray array = env->NewByteArray(len);
    if (!array) return nullptr;
    
    env->SetByteArrayRegion(array, 0, len, (const jbyte*)bytes);
    return array;
}

uint8_t* jbytearray_to_bytes(JNIEnv* env, jbyteArray array, size_t* out_len) {
    if (!env || !array) return nullptr;
    
    jsize len = env->GetArrayLength(array);
    if (len <= 0) return nullptr;
    
    uint8_t* bytes = (uint8_t*)malloc(len);
    if (!bytes) return nullptr;
    
    env->GetByteArrayRegion(array, 0, len, (jbyte*)bytes);
    
    if (out_len) *out_len = len;
    return bytes;
}

bool check_jni_exception(JNIEnv* env) {
    if (!env) return false;
    return env->ExceptionCheck();
}

void clear_jni_exception(JNIEnv* env) {
    if (env && env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}

jclass find_class_safe(JNIEnv* env, const char* name) {
    if (!env || !name) return nullptr;
    jclass clazz = env->FindClass(name);
    if (check_jni_exception(env)) {
        clear_jni_exception(env);
        return nullptr;
    }
    return clazz;
}

jmethodID get_method_id_safe(JNIEnv* env, jclass clazz, const char* name, const char* signature) {
    if (!env || !clazz || !name || !signature) return nullptr;
    jmethodID method = env->GetMethodID(clazz, name, signature);
    if (check_jni_exception(env)) {
        clear_jni_exception(env);
        return nullptr;
    }
    return method;
}

jfieldID get_field_id_safe(JNIEnv* env, jclass clazz, const char* name, const char* signature) {
    if (!env || !clazz || !name || !signature) return nullptr;
    jfieldID field = env->GetFieldID(clazz, name, signature);
    if (check_jni_exception(env)) {
        clear_jni_exception(env);
        return nullptr;
    }
    return field;
}

jmethodID get_static_method_id_safe(JNIEnv* env, jclass clazz, const char* name, const char* signature) {
    if (!env || !clazz || !name || !signature) return nullptr;
    jmethodID method = env->GetStaticMethodID(clazz, name, signature);
    if (check_jni_exception(env)) {
        clear_jni_exception(env);
        return nullptr;
    }
    return method;
}

jfieldID get_static_field_id_safe(JNIEnv* env, jclass clazz, const char* name, const char* signature) {
    if (!env || !clazz || !name || !signature) return nullptr;
    jfieldID field = env->GetStaticFieldID(clazz, name, signature);
    if (check_jni_exception(env)) {
        clear_jni_exception(env);
        return nullptr;
    }
    return field;
}

uint64_t get_current_timestamp_ms() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return (uint64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void sleep_ms(uint32_t ms) {
    usleep(ms * 1000);
}

void log_debug(const char* tag, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_DEBUG, tag ? tag : LOG_TAG, fmt, args);
    va_end(args);
}

void log_info(const char* tag, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_INFO, tag ? tag : LOG_TAG, fmt, args);
    va_end(args);
}

void log_warn(const char* tag, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_WARN, tag ? tag : LOG_TAG, fmt, args);
    va_end(args);
}

void log_error(const char* tag, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_ERROR, tag ? tag : LOG_TAG, fmt, args);
    va_end(args);
}

char* trim_whitespace(char* str) {
    if (!str) return nullptr;
    
    while (isspace(*str)) str++;
    
    if (*str == '\0') return str;
    
    char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    *(end + 1) = '\0';
    
    return str;
}

void to_lowercase(char* str) {
    if (!str) return;
    for (char* p = str; *p; p++) {
        *p = tolower(*p);
    }
}

void to_uppercase(char* str) {
    if (!str) return;
    for (char* p = str; *p; p++) {
        *p = toupper(*p);
    }
}

uint32_t hash_string(const char* str) {
    if (!str) return 0;
    
    uint32_t hash = 5381;
    int c;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    
    return hash;
}

bool strings_equal_case_insensitive(const char* a, const char* b) {
    if (!a || !b) return false;
    return strcasecmp(a, b) == 0;
}

int execute_command(const char* cmd, char* output, size_t output_size) {
    if (!cmd) return -1;
    
    FILE* fp = popen(cmd, "r");
    if (!fp) return -1;
    
    if (output && output_size > 0) {
        size_t bytes_read = fread(output, 1, output_size - 1, fp);
        output[bytes_read] = '\0';
    }
    
    int status = pclose(fp);
    return WEXITSTATUS(status);
}

bool check_command_exists(const char* cmd) {
    if (!cmd) return false;
    char test_cmd[MAX_STRING_LENGTH];
    snprintf(test_cmd, sizeof(test_cmd), "which %s >/dev/null 2>&1", cmd);
    return execute_command(test_cmd, nullptr, 0) == 0;
}
