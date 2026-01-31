#ifndef LIBMSAOAIDSEC_UTILS_H
#define LIBMSAOAIDSEC_UTILS_H

#include <jni.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

char* safe_strdup(const char* str);
char* safe_strndup(const char* str, size_t n);
void safe_free(void** ptr);

bool file_exists(const char* path);
bool is_readable(const char* path);
bool directory_exists(const char* path);

char* read_file(const char* path);
char* read_file_line(const char* path, const char* prefix);
int read_file_to_buffer(const char* path, char* buffer, size_t size);

bool starts_with(const char* str, const char* prefix);
bool ends_with(const char* str, const char* suffix);
bool contains(const char* str, const char* substr);

char* get_system_property(const char* key);
bool set_system_property(const char* key, const char* value);

int32_t get_current_pid();
int32_t get_current_uid();
char* get_package_name();
char* get_process_name();

bool is_process_running(const char* process_name);
bool is_library_loaded(const char* library_name);
bool is_thread_running(const char* thread_name);

char* jstring_to_string(JNIEnv* env, jstring jstr);
jstring string_to_jstring(JNIEnv* env, const char* str);
jbyteArray bytes_to_jbytearray(JNIEnv* env, const uint8_t* bytes, size_t len);
uint8_t* jbytearray_to_bytes(JNIEnv* env, jbyteArray array, size_t* out_len);

bool check_jni_exception(JNIEnv* env);
void clear_jni_exception(JNIEnv* env);

jclass find_class_safe(JNIEnv* env, const char* name);
jmethodID get_method_id_safe(JNIEnv* env, jclass clazz, const char* name, const char* signature);
jfieldID get_field_id_safe(JNIEnv* env, jclass clazz, const char* name, const char* signature);
jmethodID get_static_method_id_safe(JNIEnv* env, jclass clazz, const char* name, const char* signature);
jfieldID get_static_field_id_safe(JNIEnv* env, jclass clazz, const char* name, const char* signature);

jobject call_static_object_method_safe(JNIEnv* env, jclass clazz, jmethodID method, ...);
jint call_static_int_method_safe(JNIEnv* env, jclass clazz, jmethodID method, ...);
jstring call_static_string_method_safe(JNIEnv* env, jclass clazz, jmethodID method, ...);

jobject get_static_object_field_safe(JNIEnv* env, jclass clazz, jfieldID field);
jint get_static_int_field_safe(JNIEnv* env, jclass clazz, jfieldID field);
jstring get_static_string_field_safe(JNIEnv* env, jclass clazz, jfieldID field);

uint64_t get_current_timestamp_ms();
void sleep_ms(uint32_t ms);

void log_debug(const char* tag, const char* fmt, ...);
void log_info(const char* tag, const char* fmt, ...);
void log_warn(const char* tag, const char* fmt, ...);
void log_error(const char* tag, const char* fmt, ...);

char* trim_whitespace(char* str);
void to_lowercase(char* str);
void to_uppercase(char* str);

uint32_t hash_string(const char* str);
bool strings_equal_case_insensitive(const char* a, const char* b);

void hexdump(const uint8_t* data, size_t len);
char* bytes_to_hex(const uint8_t* data, size_t len);
uint8_t* hex_to_bytes(const char* hex, size_t* out_len);

int execute_command(const char* cmd, char* output, size_t output_size);
bool check_command_exists(const char* cmd);

#ifdef __cplusplus
}
#endif

#endif // LIBMSAOAIDSEC_UTILS_H
