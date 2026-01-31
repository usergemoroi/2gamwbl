#ifndef LIBCLIENT_STRING_UTILS_H
#define LIBCLIENT_STRING_UTILS_H

#include <string>
#include <vector>
#include <sstream>

namespace client {
namespace utils {

class StringUtils {
public:
    // String manipulation
    static std::string trim(const std::string& str);
    static std::string trimLeft(const std::string& str);
    static std::string trimRight(const std::string& str);
    
    static std::string toLower(const std::string& str);
    static std::string toUpper(const std::string& str);
    
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::vector<std::string> split(const std::string& str, const std::string& delimiter);
    
    static std::string join(const std::vector<std::string>& strings, const std::string& delimiter);
    
    static std::string replace(const std::string& str, const std::string& from, const std::string& to);
    static std::string replaceAll(const std::string& str, const std::string& from, const std::string& to);
    
    static bool startsWith(const std::string& str, const std::string& prefix);
    static bool endsWith(const std::string& str, const std::string& suffix);
    static bool contains(const std::string& str, const std::string& substring);
    
    static size_t count(const std::string& str, const std::string& substring);
    
    // String validation
    static bool isNumeric(const std::string& str);
    static bool isAlpha(const std::string& str);
    static bool isAlphaNumeric(const std::string& str);
    static bool isHex(const std::string& str);
    
    // String conversion
    static int toInt(const std::string& str, int default_value = 0);
    static long toLong(const std::string& str, long default_value = 0);
    static float toFloat(const std::string& str, float default_value = 0.0f);
    static double toDouble(const std::string& str, double default_value = 0.0);
    static bool toBool(const std::string& str, bool default_value = false);
    
    static std::string fromInt(int value);
    static std::string fromLong(long value);
    static std::string fromFloat(float value, int precision = -1);
    static std::string fromDouble(double value, int precision = -1);
    static std::string fromBool(bool value);
    
    // String formatting
    static std::string format(const char* fmt, ...);
    
    template<typename... Args>
    static std::string format(const std::string& fmt, Args... args) {
        return format(fmt.c_str(), args...);
    }
    
    // String obfuscation/deobfuscation
    static std::string obfuscate(const std::string& str, uint8_t key = 0x42);
    static std::string deobfuscate(const std::string& str, uint8_t key = 0x42);
    
    // URL encoding/decoding
    static std::string urlEncode(const std::string& str);
    static std::string urlDecode(const std::string& str);
    
    // String padding
    static std::string padLeft(const std::string& str, size_t width, char pad = ' ');
    static std::string padRight(const std::string& str, size_t width, char pad = ' ');
    
    // String comparison
    static bool equalsIgnoreCase(const std::string& str1, const std::string& str2);
    static int compareIgnoreCase(const std::string& str1, const std::string& str2);
    
    // Utility functions
    static std::string repeat(const std::string& str, size_t count);
    static std::string reverse(const std::string& str);
    
    static std::string substring(const std::string& str, size_t start, size_t length = std::string::npos);
    
    static size_t indexOf(const std::string& str, const std::string& substring, size_t start = 0);
    static size_t lastIndexOf(const std::string& str, const std::string& substring);
};

} // namespace utils
} // namespace client

#endif // LIBCLIENT_STRING_UTILS_H
