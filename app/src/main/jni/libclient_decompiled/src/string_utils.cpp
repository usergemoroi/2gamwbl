#include "../include/internal/string_utils.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <cstdarg>
#include <cstdio>

namespace client {
namespace utils {

std::string StringUtils::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

std::string StringUtils::trimLeft(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    return first == std::string::npos ? "" : str.substr(first);
}

std::string StringUtils::trimRight(const std::string& str) {
    size_t last = str.find_last_not_of(" \t\n\r");
    return last == std::string::npos ? "" : str.substr(0, last + 1);
}

std::string StringUtils::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                  [](unsigned char c){ return std::tolower(c); });
    return result;
}

std::string StringUtils::toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                  [](unsigned char c){ return std::toupper(c); });
    return result;
}

std::vector<std::string> StringUtils::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<std::string> StringUtils::split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);
    
    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    tokens.push_back(str.substr(start));
    
    return tokens;
}

std::string StringUtils::join(const std::vector<std::string>& strings, const std::string& delimiter) {
    if (strings.empty()) return "";
    
    std::ostringstream oss;
    oss << strings[0];
    for (size_t i = 1; i < strings.size(); ++i) {
        oss << delimiter << strings[i];
    }
    return oss.str();
}

std::string StringUtils::replace(const std::string& str, const std::string& from, const std::string& to) {
    std::string result = str;
    size_t pos = result.find(from);
    if (pos != std::string::npos) {
        result.replace(pos, from.length(), to);
    }
    return result;
}

std::string StringUtils::replaceAll(const std::string& str, const std::string& from, const std::string& to) {
    std::string result = str;
    size_t pos = 0;
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.length(), to);
        pos += to.length();
    }
    return result;
}

bool StringUtils::startsWith(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

bool StringUtils::endsWith(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool StringUtils::contains(const std::string& str, const std::string& substring) {
    return str.find(substring) != std::string::npos;
}

size_t StringUtils::count(const std::string& str, const std::string& substring) {
    size_t count = 0;
    size_t pos = 0;
    while ((pos = str.find(substring, pos)) != std::string::npos) {
        ++count;
        pos += substring.length();
    }
    return count;
}

bool StringUtils::isNumeric(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

bool StringUtils::isAlpha(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isalpha);
}

bool StringUtils::isAlphaNumeric(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isalnum);
}

int StringUtils::toInt(const std::string& str, int default_value) {
    try {
        return std::stoi(str);
    } catch (...) {
        return default_value;
    }
}

long StringUtils::toLong(const std::string& str, long default_value) {
    try {
        return std::stol(str);
    } catch (...) {
        return default_value;
    }
}

float StringUtils::toFloat(const std::string& str, float default_value) {
    try {
        return std::stof(str);
    } catch (...) {
        return default_value;
    }
}

double StringUtils::toDouble(const std::string& str, double default_value) {
    try {
        return std::stod(str);
    } catch (...) {
        return default_value;
    }
}

bool StringUtils::toBool(const std::string& str, bool default_value) {
    std::string lower = toLower(trim(str));
    if (lower == "true" || lower == "1" || lower == "yes") return true;
    if (lower == "false" || lower == "0" || lower == "no") return false;
    return default_value;
}

std::string StringUtils::fromInt(int value) {
    return std::to_string(value);
}

std::string StringUtils::fromLong(long value) {
    return std::to_string(value);
}

std::string StringUtils::fromFloat(float value, int precision) {
    if (precision >= 0) {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%.*f", precision, value);
        return std::string(buffer);
    }
    return std::to_string(value);
}

std::string StringUtils::fromDouble(double value, int precision) {
    if (precision >= 0) {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%.*f", precision, value);
        return std::string(buffer);
    }
    return std::to_string(value);
}

std::string StringUtils::fromBool(bool value) {
    return value ? "true" : "false";
}

std::string StringUtils::format(const char* fmt, ...) {
    char buffer[2048];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    return std::string(buffer);
}

std::string StringUtils::obfuscate(const std::string& str, uint8_t key) {
    std::string result = str;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] ^= (key ^ (i & 0xFF));
    }
    return result;
}

std::string StringUtils::deobfuscate(const std::string& str, uint8_t key) {
    return obfuscate(str, key); // XOR is symmetric
}

bool StringUtils::equalsIgnoreCase(const std::string& str1, const std::string& str2) {
    return toLower(str1) == toLower(str2);
}

int StringUtils::compareIgnoreCase(const std::string& str1, const std::string& str2) {
    return toLower(str1).compare(toLower(str2));
}

std::string StringUtils::repeat(const std::string& str, size_t count) {
    std::string result;
    result.reserve(str.length() * count);
    for (size_t i = 0; i < count; ++i) {
        result += str;
    }
    return result;
}

std::string StringUtils::reverse(const std::string& str) {
    return std::string(str.rbegin(), str.rend());
}

size_t StringUtils::indexOf(const std::string& str, const std::string& substring, size_t start) {
    return str.find(substring, start);
}

size_t StringUtils::lastIndexOf(const std::string& str, const std::string& substring) {
    return str.rfind(substring);
}

} // namespace utils
} // namespace client
