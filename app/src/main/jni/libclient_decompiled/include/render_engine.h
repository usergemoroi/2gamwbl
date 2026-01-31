#ifndef LIBCLIENT_RENDER_ENGINE_H
#define LIBCLIENT_RENDER_ENGINE_H

#include "types.h"
#include <memory>
#include <thread>
#include <atomic>
#include <android/native_window.h>

namespace client {

class RenderEngine {
public:
    RenderEngine();
    ~RenderEngine();
    
    // Initialization and cleanup
    bool initialize();
    void shutdown();
    bool isInitialized() const { return initialized_; }
    
    // Surface management
    bool createSurface(ANativeWindow* window, int width, int height);
    void destroySurface();
    bool hasSurface() const;
    
    // Rendering control
    void startRendering();
    void stopRendering();
    bool isRendering() const { return rendering_; }
    
    // Frame rendering
    void renderFrame();
    void drawTick();
    void drawData(const uint8_t* data, int width, int height);
    
    // Configuration
    void setScreenSize(int width, int height);
    void getScreenSize(int& width, int& height) const;
    void setVisibility(bool visible);
    bool isVisible() const { return visible_; }
    void setOpacity(float opacity);
    float getOpacity() const { return opacity_; }
    
    // Performance metrics
    float getFPS() const { return fps_; }
    uint64_t getFrameCount() const { return frame_count_; }
    
private:
    // EGL management
    bool initEGL();
    void cleanupEGL();
    bool createEGLContext();
    bool createEGLSurface(ANativeWindow* window);
    void destroyEGLSurface();
    bool makeCurrent();
    void swapBuffers();
    
    // OpenGL setup
    bool initGL();
    void cleanupGL();
    bool compileShaders();
    bool createBuffers();
    void setupVertexAttributes();
    
    // Shader management
    GLuint loadShader(GLenum shader_type, const char* source);
    GLuint createProgram(const char* vertex_source, const char* fragment_source);
    void deleteShaders();
    
    // Texture management
    bool loadTexture(const uint8_t* data, int width, int height);
    void updateTexture(const uint8_t* data, int width, int height);
    void deleteTextures();
    
    // Drawing operations
    void clearScreen();
    void drawQuad();
    void drawOverlay();
    void updateFPS();
    
    // Rendering thread
    void renderThreadFunc();
    
    // State management
    std::atomic<bool> initialized_;
    std::atomic<bool> rendering_;
    std::atomic<bool> visible_;
    std::atomic<bool> should_stop_;
    
    // EGL state
    GLContext gl_context_;
    ANativeWindow* native_window_;
    
    // OpenGL resources
    ShaderData shader_data_;
    VertexBuffer vertex_buffer_;
    TextureData texture_data_;
    
    // Rendering parameters
    int screen_width_;
    int screen_height_;
    float opacity_;
    
    // Performance tracking
    float fps_;
    uint64_t frame_count_;
    uint64_t last_frame_time_;
    uint64_t frame_time_accum_;
    int frame_time_samples_;
    
    // Threading
    std::unique_ptr<std::thread> render_thread_;
    std::mutex render_mutex_;
    
    // Vertex data
    static constexpr float kQuadVertices[] = {
        // positions    // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    
    static constexpr uint16_t kQuadIndices[] = {
        0, 1, 2, 0, 2, 3
    };
    
    // Shader sources
    static const char* kVertexShaderSource;
    static const char* kFragmentShaderSource;
};

} // namespace client

#endif // LIBCLIENT_RENDER_ENGINE_H
