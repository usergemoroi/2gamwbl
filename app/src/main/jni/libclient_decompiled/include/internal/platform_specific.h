#ifndef LIBCLIENT_PLATFORM_SPECIFIC_H
#define LIBCLIENT_PLATFORM_SPECIFIC_H

#include <string>
#include <vector>
#include <cstdint>
#include <sys/types.h>

namespace client {
namespace platform {

class FileSystem {
public:
    static bool exists(const std::string& path);
    static bool isFile(const std::string& path);
    static bool isDirectory(const std::string& path);
    static bool isReadable(const std::string& path);
    static bool isWritable(const std::string& path);
    
    static bool createDirectory(const std::string& path, bool recursive = true);
    static bool removeDirectory(const std::string& path, bool recursive = false);
    static bool remove(const std::string& path);
    
    static bool copy(const std::string& source, const std::string& destination);
    static bool move(const std::string& source, const std::string& destination);
    
    static uint64_t getFileSize(const std::string& path);
    static uint64_t getModificationTime(const std::string& path);
    
    static std::vector<std::string> listFiles(const std::string& directory);
    static std::vector<std::string> listDirectories(const std::string& directory);
    
    static bool readFile(const std::string& path, std::vector<uint8_t>& data);
    static bool readFile(const std::string& path, std::string& data);
    static bool writeFile(const std::string& path, const std::vector<uint8_t>& data);
    static bool writeFile(const std::string& path, const std::string& data);
    
    static std::string getFileName(const std::string& path);
    static std::string getFileExtension(const std::string& path);
    static std::string getDirectory(const std::string& path);
    static std::string getBaseName(const std::string& path);
    
    static std::string combinePath(const std::string& path1, const std::string& path2);
    static std::string normalizePath(const std::string& path);
    static std::string getAbsolutePath(const std::string& path);
    
    static std::string getTempDirectory();
    static std::string getCurrentDirectory();
};

class Process {
public:
    static pid_t getCurrentPid();
    static pid_t getParentPid();
    static std::string getCurrentProcessName();
    
    static bool isProcessRunning(pid_t pid);
    static bool isProcessRunning(const std::string& process_name);
    
    static std::vector<pid_t> getAllProcesses();
    static std::vector<std::string> getProcessList();
    
    static std::string getProcessName(pid_t pid);
    static std::string getProcessCmdline(pid_t pid);
    static pid_t getProcessPid(const std::string& process_name);
    
    static bool killProcess(pid_t pid, int signal = 15);
    
    static std::string readProcFile(pid_t pid, const std::string& filename);
    static bool readProcStatus(pid_t pid, std::string& status);
    
    static int getProcessUid(pid_t pid);
    static int getProcessGid(pid_t pid);
};

class System {
public:
    static std::string getProperty(const std::string& key);
    static bool setProperty(const std::string& key, const std::string& value);
    
    static std::string getEnvironmentVariable(const std::string& name);
    static bool setEnvironmentVariable(const std::string& name, const std::string& value);
    
    static uint64_t getCurrentTimeMillis();
    static uint64_t getCurrentTimeMicros();
    static uint64_t getUptime();
    
    static void sleep(uint64_t milliseconds);
    static void usleep(uint64_t microseconds);
    
    static int getCPUCount();
    static uint64_t getTotalMemory();
    static uint64_t getAvailableMemory();
    
    static std::string getArchitecture();
    static std::string getKernelVersion();
    static std::string getOSVersion();
    
    static bool isRooted();
    static bool isEmulator();
    static bool isDebugBuild();
};

class Thread {
public:
    static int getCurrentThreadId();
    static std::string getCurrentThreadName();
    
    static bool setThreadName(const std::string& name);
    static bool setThreadPriority(int priority);
    
    static int getThreadPriority();
};

class Library {
public:
    static void* load(const std::string& path);
    static void unload(void* handle);
    static void* getSymbol(void* handle, const std::string& symbol_name);
    
    static std::string getLibraryPath(void* address);
    static std::vector<std::string> getLoadedLibraries();
    
    static bool isLibraryLoaded(const std::string& library_name);
};

class Network {
public:
    static bool isNetworkAvailable();
    static bool isWiFiConnected();
    static bool isMobileDataConnected();
    
    static std::string getIPAddress();
    static std::string getWiFiSSID();
    static std::string getMACAddress();
    
    static bool hasInternet();
};

} // namespace platform
} // namespace client

#endif // LIBCLIENT_PLATFORM_SPECIFIC_H
