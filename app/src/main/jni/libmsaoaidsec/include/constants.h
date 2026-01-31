#ifndef LIBMSAOAIDSEC_CONSTANTS_H
#define LIBMSAOAIDSEC_CONSTANTS_H

#define MSAOAIDSEC_VERSION "1.0.0"
#define MSAOAIDSEC_BUILD "670KB"

#define ANDROID_MIN_SDK 21
#define ANDROID_TARGET_SDK 34

#define MAX_STRING_LENGTH 1024
#define MAX_PATH_LENGTH 4096
#define MAX_PACKAGE_NAME_LENGTH 256

#define TRACERPID_PATH "/proc/self/status"
#define MAPS_PATH "/proc/self/maps"
#define CMDLINE_PATH "/proc/self/cmdline"
#define STATUS_PATH "/proc/self/status"

#define SU_BINARY_PATHS \
    "/system/bin/su", \
    "/system/xbin/su", \
    "/sbin/su", \
    "/system/su", \
    "/system/bin/.ext/.su", \
    "/system/usr/we-need-root/su-backup", \
    "/system/xbin/mu", \
    "/data/local/su", \
    "/data/local/bin/su", \
    "/data/local/xbin/su"

#define SUPERUSER_APKS \
    "com.noshufou.android.su", \
    "com.thirdparty.superuser", \
    "eu.chainfire.supersu", \
    "com.koushikdutta.superuser", \
    "com.topjohnwu.magisk"

#define XPOSED_CLASSES \
    "de.robv.android.xposed.XposedBridge", \
    "de.robv.android.xposed.XC_MethodHook", \
    "de.robv.android.xposed.XposedHelpers"

#define FRIDA_LIBRARIES \
    "frida-agent", \
    "frida-gadget", \
    "frida-server", \
    "re.frida.server"

#define FRIDA_THREADS \
    "gdbus", \
    "gmain", \
    "gum-js-loop", \
    "pool-frida"

#define EMULATOR_FILES \
    "/dev/socket/qemud", \
    "/dev/qemu_pipe", \
    "/system/lib/libc_malloc_debug_qemu.so", \
    "/sys/qemu_trace", \
    "/system/bin/qemu-props", \
    "/dev/goldfish_pipe"

#define EMULATOR_PROPERTIES \
    "ro.kernel.qemu", \
    "ro.hardware", \
    "ro.product.device", \
    "ro.build.product", \
    "init.svc.qemud", \
    "qemu.hw.mainkeys"

#define EMULATOR_BRANDS \
    "generic", \
    "unknown", \
    "Android", \
    "google_sdk", \
    "Genymotion"

#define OAID_MANUFACTURER_XIAOMI "Xiaomi"
#define OAID_MANUFACTURER_HUAWEI "HUAWEI"
#define OAID_MANUFACTURER_HONOR "HONOR"
#define OAID_MANUFACTURER_SAMSUNG "samsung"
#define OAID_MANUFACTURER_OPPO "OPPO"
#define OAID_MANUFACTURER_VIVO "vivo"
#define OAID_MANUFACTURER_ONEPLUS "OnePlus"
#define OAID_MANUFACTURER_MEIZU "Meizu"
#define OAID_MANUFACTURER_ASUS "asus"
#define OAID_MANUFACTURER_LENOVO "LENOVO"

#define OAID_PACKAGE_XIAOMI "com.android.id"
#define OAID_PACKAGE_HUAWEI "com.huawei.hwid"
#define OAID_PACKAGE_SAMSUNG "com.samsung.android.deviceidservice"
#define OAID_PACKAGE_OPPO "com.heytap.openid"
#define OAID_PACKAGE_VIVO "com.vivo.identifier"

#define ANDROID_ID_SECURE_SETTINGS "android_id"
#define SETTINGS_SECURE_CLASS "android/provider/Settings$Secure"

#define JAVA_CLASS_CONTEXT "android/content/Context"
#define JAVA_CLASS_ACTIVITY_THREAD "android/app/ActivityThread"
#define JAVA_CLASS_APPLICATION "android/app/Application"
#define JAVA_CLASS_SYSTEM_PROPERTIES "android/os/SystemProperties"
#define JAVA_CLASS_BUILD "android/os/Build"
#define JAVA_CLASS_BUILD_VERSION "android/os/Build$VERSION"
#define JAVA_CLASS_INTENT_FILTER "android/content/IntentFilter"
#define JAVA_CLASS_INTENT "android/content/Intent"
#define JAVA_CLASS_CONTEXT_WRAPPER "android/content/ContextWrapper"
#define JAVA_CLASS_PROCESS "android/os/Process"

#define JAVA_METHOD_GET_APPLICATION_INFO "getApplicationInfo"
#define JAVA_METHOD_CURRENT_APPLICATION_THREAD "currentActivityThread"
#define JAVA_METHOD_GET_APPLICATION "getApplication"
#define JAVA_METHOD_GET_STRING "getString"
#define JAVA_METHOD_GET_INT "getInt"
#define JAVA_METHOD_MY_PID "myPid"
#define JAVA_METHOD_MY_UID "myUid"
#define JAVA_METHOD_REGISTER_RECEIVER "registerReceiver"

#define JAVA_SIGNATURE_GET_APPLICATION_INFO "()Landroid/content/pm/ApplicationInfo;"
#define JAVA_SIGNATURE_CURRENT_ACTIVITY_THREAD "()Landroid/app/ActivityThread;"
#define JAVA_SIGNATURE_GET_APPLICATION "()Landroid/app/Application;"
#define JAVA_SIGNATURE_GET_STRING "(Ljava/lang/String;)Ljava/lang/String;"
#define JAVA_SIGNATURE_GET_INT "(Ljava/lang/String;I)I"
#define JAVA_SIGNATURE_MY_PID "()I"
#define JAVA_SIGNATURE_MY_UID "()I"
#define JAVA_SIGNATURE_REGISTER_RECEIVER "(Landroid/content/BroadcastReceiver;Landroid/content/IntentFilter;)Landroid/content/Intent;"

#define JAVA_FIELD_SDK_INT "SDK_INT"
#define JAVA_FIELD_RELEASE "RELEASE"
#define JAVA_FIELD_MANUFACTURER "MANUFACTURER"
#define JAVA_FIELD_MODEL "MODEL"
#define JAVA_FIELD_BRAND "BRAND"
#define JAVA_FIELD_HARDWARE "HARDWARE"
#define JAVA_FIELD_DEVICE "DEVICE"
#define JAVA_FIELD_PRODUCT "PRODUCT"
#define JAVA_FIELD_FINGERPRINT "FINGERPRINT"
#define JAVA_FIELD_ID "ID"
#define JAVA_FIELD_FLAGS "flags"

#define BATTERY_ACTION "android.intent.action.BATTERY_CHANGED"
#define LINKER_PATH "/apex/com.android.runtime/bin/linker64"

#define LOG_TAG "libmsaoaidsec"

#define SECURITY_CHECK_INTERVAL_MS 5000
#define MAX_SECURITY_CHECK_RETRIES 3

#define DEBUG_MARKER "TracerPid:"
#define DEBUG_VALUE_DETECTED 0

#endif // LIBMSAOAIDSEC_CONSTANTS_H
