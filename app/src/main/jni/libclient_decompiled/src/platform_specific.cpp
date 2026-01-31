#include "../include/internal/platform_specific.h"
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include <chrono>
#include <cstdio>

namespace client {
namespace platform {

// FileSystem implementations
bool FileSystem::exists(const std::string& path) {
    return access(path.c_str(), F_OK) == 0;
}

bool FileSystem::isFile(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

bool FileSystem::isDirectory(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

bool FileSystem::isReadable(const std::string& path) {
    return access(path.c_str(), R_OK) == 0;
}

bool FileSystem::isWritable(const std::string& path) {
    return access(path.c_str(), W_OK) == 0;
}

bool FileSystem::createDirectory(const std::string& path, bool recursive) {
    if (exists(path)) return isDirectory(path);
    
    if (recursive) {
        size_t pos = path.find('/');
        while (pos != std::string::npos) {
            std::string dir = path.substr(0, pos);
            if (!dir.empty() && !exists(dir)) {
                if (mkdir(dir.c_str(), 0755) != 0) {
                    return false;
                }
            }
            pos = path.find('/', pos + 1);
        }
    }
    
    return mkdir(path.c_str(), 0755) == 0;
}

bool FileSystem::remove(const std::string& path) {
    return ::remove(path.c_str()) == 0;
}

uint64_t FileSystem::getFileSize(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 ? st.st_size : 0;
}

uint64_t FileSystem::getModificationTime(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 ? st.st_mtime : 0;
}

bool FileSystem::readFile(const std::string& path, std::string& data) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return false;
    
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    data.resize(size);
    file.read(&data[0], size);
    
    return file.good();
}

bool FileSystem::readFile(const std::string& path, std::vector<uint8_t>& data) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return false;
    
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    data.resize(size);
    file.read(reinterpret_cast<char*>(data.data()), size);
    
    return file.good();
}

bool FileSystem::writeFile(const std::string& path, const std::string& data) {
    std::ofstream file(path, std::ios::binary);
    if (!file) return false;
    
    file.write(data.data(), data.size());
    return file.good();
}

bool FileSystem::writeFile(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream file(path, std::ios::binary);
    if (!file) return false;
    
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    return file.good();
}

std::vector<std::string> FileSystem::listFiles(const std::string& directory) {
    std::vector<std::string> files;
    DIR* dir = opendir(directory.c_str());
    if (!dir) return files;
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_REG) {
            files.push_back(entry->d_name);
        }
    }
    closedir(dir);
    
    return files;
}

// Process implementations
pid_t Process::getCurrentPid() {
    return getpid();
}

pid_t Process::getParentPid() {
    return getppid();
}

std::string Process::getCurrentProcessName() {
    char buffer[256];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) {
        buffer[len] = '\0';
        return std::string(buffer);
    }
    return "";
}

bool Process::isProcessRunning(pid_t pid) {
    return kill(pid, 0) == 0;
}

bool Process::isProcessRunning(const std::string& process_name) {
    return getProcessPid(process_name) != -1;
}

std::vector<pid_t> Process::getAllProcesses() {
    std::vector<pid_t> pids;
    DIR* dir = opendir("/proc");
    if (!dir) return pids;
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_DIR) {
            pid_t pid = atoi(entry->d_name);
            if (pid > 0) {
                pids.push_back(pid);
            }
        }
    }
    closedir(dir);
    
    return pids;
}

std::string Process::getProcessName(pid_t pid) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/cmdline", pid);
    
    std::string cmdline;
    if (FileSystem::readFile(path, cmdline)) {
        // Replace null bytes with spaces
        for (char& c : cmdline) {
            if (c == '\0') c = ' ';
        }
        return cmdline;
    }
    return "";
}

std::string Process::getProcessCmdline(pid_t pid) {
    return getProcessName(pid);
}

pid_t Process::getProcessPid(const std::string& process_name) {
    auto pids = getAllProcesses();
    for (pid_t pid : pids) {
        std::string name = getProcessName(pid);
        if (name.find(process_name) != std::string::npos) {
            return pid;
        }
    }
    return -1;
}

std::vector<std::string> Process::getProcessList() {
    std::vector<std::string> names;
    auto pids = getAllProcesses();
    for (pid_t pid : pids) {
        names.push_back(getProcessName(pid));
    }
    return names;
}

std::string Process::readProcFile(pid_t pid, const std::string& filename) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/%s", pid, filename.c_str());
    
    std::string content;
    FileSystem::readFile(path, content);
    return content;
}

int Process::getProcessUid(pid_t pid) {
    std::string status = readProcFile(pid, "status");
    size_t pos = status.find("Uid:");
    if (pos != std::string::npos) {
        int uid;
        if (sscanf(status.c_str() + pos + 4, "%d", &uid) == 1) {
            return uid;
        }
    }
    return -1;
}

// System implementations
std::string System::getProperty(const std::string& key) {
    // Stub - would use __system_property_get on Android
    return "";
}

uint64_t System::getCurrentTimeMillis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

uint64_t System::getCurrentTimeMicros() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

void System::sleep(uint64_t milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void System::usleep(uint64_t microseconds) {
    std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
}

int System::getCPUCount() {
    return sysconf(_SC_NPROCESSORS_ONLN);
}

uint64_t System::getTotalMemory() {
    // Stub - would read /proc/meminfo
    return 0;
}

uint64_t System::getAvailableMemory() {
    // Stub - would read /proc/meminfo
    return 0;
}

std::string System::getArchitecture() {
    #if defined(__aarch64__)
        return "arm64-v8a";
    #elif defined(__arm__)
        return "armeabi-v7a";
    #elif defined(__x86_64__)
        return "x86_64";
    #elif defined(__i386__)
        return "x86";
    #else
        return "unknown";
    #endif
}

bool System::isRooted() {
    return FileSystem::exists("/system/bin/su") ||
           FileSystem::exists("/system/xbin/su");
}

bool System::isEmulator() {
    std::string hardware = getProperty("ro.hardware");
    return hardware.find("goldfish") != std::string::npos ||
           hardware.find("ranchu") != std::string::npos;
}

} // namespace platform
} // namespace client
