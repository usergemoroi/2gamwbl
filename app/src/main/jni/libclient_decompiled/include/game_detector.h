#ifndef LIBCLIENT_GAME_DETECTOR_H
#define LIBCLIENT_GAME_DETECTOR_H

#include "types.h"
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <thread>
#include <atomic>

namespace client {

class GameDetector {
public:
    GameDetector();
    ~GameDetector();
    
    // Initialization
    bool initialize();
    void shutdown();
    
    // Game detection
    bool findGame(const std::string& package_name);
    bool isGameRunning(const std::string& package_name) const;
    GameInfo getGameInfo(const std::string& package_name) const;
    
    // Automatic detection
    void startAutoDetection();
    void stopAutoDetection();
    bool isAutoDetecting() const { return auto_detecting_; }
    
    // Target games management
    void addTargetGame(const TargetGame& game);
    void removeTargetGame(const std::string& package_name);
    std::vector<TargetGame> getTargetGames() const;
    bool isTargetGame(const std::string& package_name) const;
    
    // Callbacks
    using GameDetectedCallback = std::function<void(const GameInfo&)>;
    using GameStoppedCallback = std::function<void(const std::string&)>;
    
    void setGameDetectedCallback(GameDetectedCallback callback);
    void setGameStoppedCallback(GameStoppedCallback callback);
    
    // Game information
    std::string getCurrentGamePackage() const;
    bool hasGameDetected() const;
    GameStatus getGameStatus() const { return game_status_; }
    
private:
    // Process scanning
    std::vector<std::string> scanRunningProcesses();
    bool isProcessRunning(const std::string& package_name);
    int getProcessPid(const std::string& package_name);
    int getProcessUid(const std::string& package_name);
    
    // Game information retrieval
    bool getGameVersion(const std::string& package_name, std::string& version_name, int& version_code);
    uint64_t getProcessStartTime(int pid);
    
    // Detection thread
    void detectionThreadFunc();
    void checkRunningGames();
    void notifyGameDetected(const GameInfo& info);
    void notifyGameStopped(const std::string& package_name);
    
    // System information access
    bool readProcStatus(int pid, std::map<std::string, std::string>& status);
    bool readProcCmdline(int pid, std::string& cmdline);
    std::vector<int> getAllPids();
    
    // Target game list initialization
    void initializeTargetGames();
    bool matchesTargetGame(const std::string& package_name) const;
    
    // State
    std::atomic<bool> initialized_;
    std::atomic<bool> auto_detecting_;
    std::atomic<bool> should_stop_;
    GameStatus game_status_;
    
    // Current game information
    GameInfo current_game_;
    mutable std::mutex game_mutex_;
    
    // Target games
    std::vector<TargetGame> target_games_;
    mutable std::mutex targets_mutex_;
    
    // Detected games cache
    std::map<std::string, GameInfo> detected_games_;
    mutable std::mutex cache_mutex_;
    
    // Callbacks
    GameDetectedCallback game_detected_callback_;
    GameStoppedCallback game_stopped_callback_;
    std::mutex callback_mutex_;
    
    // Detection thread
    std::unique_ptr<std::thread> detection_thread_;
    int detection_interval_ms_;
    
    // Configuration
    bool enable_cache_;
    int cache_timeout_ms_;
};

} // namespace client

#endif // LIBCLIENT_GAME_DETECTOR_H
