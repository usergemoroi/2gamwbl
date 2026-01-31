/*
 * Decompiled from libclient.so (arm64-v8a)
 * GameBlaster Pro v3.1 - Main Client Native Library Header
 */

#ifndef GAMEBLASTER_CLIENT_H
#define GAMEBLASTER_CLIENT_H

#include <jni.h>
#include <string>
#include <vector>
#include <memory>

namespace gameblaster {

class RenderEngine;
class GameDetector;
class PluginManager;
class NetworkClient;

// Render Engine - handles OpenGL ES 3.0 rendering
class RenderEngine {
public:
    RenderEngine();
    ~RenderEngine();
    
    bool initialize(ANativeWindow* window, int width, int height);
    void drawOverlay(const uint8_t* data, int width, int height);
    void tick();
    void cleanup();
    
private:
    bool initializeGLResources();
    GLuint compileShader(GLenum type, const char* source);
};

// Game Detector - detects target games
class GameDetector {
public:
    GameDetector();
    
    std::string findGame(JNIEnv* env, jobject context);
    std::string getCurrentGame() const;
};

// Plugin Manager - manages plugin updates and licensing
class PluginManager {
public:
    PluginManager();
    
    std::string getPluginVersion() const;
    std::string getPluginUrl() const;
    void setPluginUrl(const std::string& url);
    
    std::string getGameVersion() const;
    void setGameVersion(const std::string& version);
    
    bool update(const std::string& url);
    bool checkLicense(const std::string& key);
};

// Network Client - handles network communication
class NetworkClient {
public:
    NetworkClient();
    
    bool connect(const std::string& server);
    void disconnect();
    bool isConnected() const;
    
    std::string getTelegramUrl() const;
    void setTelegramUrl(const std::string& url);
    
    int64_t getServerTime();
};

} // namespace gameblaster

#endif // GAMEBLASTER_CLIENT_H
