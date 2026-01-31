#include "../include/game_detector.h"
#include "../include/internal/platform_specific.h"
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "GameDetector", __VA_ARGS__)

namespace client {

GameDetector::GameDetector() 
    : initialized_(false), auto_detecting_(false), should_stop_(false),
      game_status_(GameStatus::NOT_DETECTED), enable_cache_(true), cache_timeout_ms_(5000) {
}

GameDetector::~GameDetector() {
    shutdown();
}

bool GameDetector::initialize() {
    if (initialized_) return true;
    initializeTargetGames();
    initialized_ = true;
    LOGI("Game detector initialized");
    return true;
}

void GameDetector::shutdown() {
    stopAutoDetection();
    initialized_ = false;
}

bool GameDetector::findGame(const std::string& package_name) {
    if (!initialized_) return false;
    bool found = platform::Process::isProcessRunning(package_name);
    if (found) {
        current_game_.package_name = package_name;
        current_game_.is_running = true;
        game_status_ = GameStatus::DETECTED;
    }
    return found;
}

bool GameDetector::isGameRunning(const std::string& package_name) const {
    return platform::Process::isProcessRunning(package_name);
}

GameInfo GameDetector::getGameInfo(const std::string& package_name) const {
    std::lock_guard<std::mutex> lock(game_mutex_);
    return current_game_;
}

void GameDetector::startAutoDetection() {
    if (auto_detecting_) return;
    should_stop_ = false;
    auto_detecting_ = true;
    detection_thread_ = std::make_unique<std::thread>(&GameDetector::detectionThreadFunc, this);
}

void GameDetector::stopAutoDetection() {
    if (!auto_detecting_) return;
    should_stop_ = true;
    if (detection_thread_ && detection_thread_->joinable()) {
        detection_thread_->join();
    }
    detection_thread_.reset();
    auto_detecting_ = false;
}

void GameDetector::addTargetGame(const TargetGame& game) {
    std::lock_guard<std::mutex> lock(targets_mutex_);
    target_games_.push_back(game);
}

void GameDetector::removeTargetGame(const std::string& package_name) {
    std::lock_guard<std::mutex> lock(targets_mutex_);
    target_games_.erase(
        std::remove_if(target_games_.begin(), target_games_.end(),
            [&](const TargetGame& g) { return g.package_name == package_name; }),
        target_games_.end());
}

std::vector<TargetGame> GameDetector::getTargetGames() const {
    std::lock_guard<std::mutex> lock(targets_mutex_);
    return target_games_;
}

bool GameDetector::isTargetGame(const std::string& package_name) const {
    std::lock_guard<std::mutex> lock(targets_mutex_);
    return std::any_of(target_games_.begin(), target_games_.end(),
        [&](const TargetGame& g) { return g.package_name == package_name; });
}

std::string GameDetector::getCurrentGamePackage() const {
    std::lock_guard<std::mutex> lock(game_mutex_);
    return current_game_.package_name;
}

bool GameDetector::hasGameDetected() const {
    return game_status_ == GameStatus::DETECTED || game_status_ == GameStatus::RUNNING;
}

void GameDetector::detectionThreadFunc() {
    while (!should_stop_) {
        checkRunningGames();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

void GameDetector::checkRunningGames() {
    std::lock_guard<std::mutex> lock(targets_mutex_);
    for (const auto& target : target_games_) {
        if (isProcessRunning(target.package_name)) {
            GameInfo info;
            info.package_name = target.package_name;
            info.is_running = true;
            notifyGameDetected(info);
            break;
        }
    }
}

void GameDetector::notifyGameDetected(const GameInfo& info) {
    if (game_detected_callback_) {
        game_detected_callback_(info);
    }
}

void GameDetector::notifyGameStopped(const std::string& package_name) {
    if (game_stopped_callback_) {
        game_stopped_callback_(package_name);
    }
}

void GameDetector::initializeTargetGames() {
    TargetGame pubg_cn;
    pubg_cn.package_name = "com.tencent.ig";
    pubg_cn.display_name = "PUBG Mobile CN";
    pubg_cn.region = "China";
    target_games_.push_back(pubg_cn);
    
    TargetGame pubg_global;
    pubg_global.package_name = "com.pubg.imobile";
    pubg_global.display_name = "PUBG Mobile Global";
    pubg_global.region = "Global";
    target_games_.push_back(pubg_global);
}

std::vector<std::string> GameDetector::scanRunningProcesses() {
    return platform::Process::getProcessList();
}

bool GameDetector::isProcessRunning(const std::string& package_name) {
    return platform::Process::isProcessRunning(package_name);
}

int GameDetector::getProcessPid(const std::string& package_name) {
    return platform::Process::getProcessPid(package_name);
}

int GameDetector::getProcessUid(const std::string& package_name) {
    pid_t pid = getProcessPid(package_name);
    return platform::Process::getProcessUid(pid);
}

void GameDetector::setGameDetectedCallback(GameDetectedCallback callback) {
    std::lock_guard<std::mutex> lock(callback_mutex_);
    game_detected_callback_ = callback;
}

void GameDetector::setGameStoppedCallback(GameStoppedCallback callback) {
    std::lock_guard<std::mutex> lock(callback_mutex_);
    game_stopped_callback_ = callback;
}

} // namespace client
