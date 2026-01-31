#include "../include/render_engine.h"
#include <android/log.h>
#include <thread>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "RenderEngine", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "RenderEngine", __VA_ARGS__)

namespace client {

const char* RenderEngine::kVertexShaderSource = R"(
#version 300 es
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoord = aTexCoord;
}
)";

const char* RenderEngine::kFragmentShaderSource = R"(
#version 300 es
precision mediump float;
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D texture1;
uniform float opacity;
void main() {
    FragColor = texture(texture1, TexCoord);
    FragColor.a *= opacity;
}
)";

RenderEngine::RenderEngine() 
    : initialized_(false), rendering_(false), visible_(true), should_stop_(false),
      native_window_(nullptr), screen_width_(1920), screen_height_(1080),
      opacity_(0.8f), fps_(0.0f), frame_count_(0), last_frame_time_(0) {}

RenderEngine::~RenderEngine() { shutdown(); }

bool RenderEngine::initialize() {
    if (initialized_) return true;
    initialized_ = true;
    LOGI("RenderEngine initialized");
    return true;
}

void RenderEngine::shutdown() {
    stopRendering();
    destroySurface();
    initialized_ = false;
}

bool RenderEngine::createSurface(ANativeWindow* window, int width, int height) {
    native_window_ = window;
    screen_width_ = width;
    screen_height_ = height;
    return initEGL() && initGL();
}

void RenderEngine::destroySurface() {
    cleanupGL();
    if (native_window_) {
        ANativeWindow_release(native_window_);
        native_window_ = nullptr;
    }
}

bool RenderEngine::hasSurface() const {
    return gl_context_.surface != EGL_NO_SURFACE;
}

void RenderEngine::startRendering() {
    if (rendering_) return;
    should_stop_ = false;
    rendering_ = true;
    render_thread_ = std::make_unique<std::thread>(&RenderEngine::renderThreadFunc, this);
}

void RenderEngine::stopRendering() {
    should_stop_ = true;
    if (render_thread_ && render_thread_->joinable()) {
        render_thread_->join();
    }
    rendering_ = false;
}

void RenderEngine::renderFrame() {
    clearScreen();
    swapBuffers();
    frame_count_++;
}

void RenderEngine::drawTick() { renderFrame(); }
void RenderEngine::drawData(const uint8_t* data, int width, int height) { renderFrame(); }
void RenderEngine::setScreenSize(int width, int height) { screen_width_ = width; screen_height_ = height; }
void RenderEngine::getScreenSize(int& width, int& height) const { width = screen_width_; height = screen_height_; }
void RenderEngine::setVisibility(bool visible) { visible_ = visible; }
void RenderEngine::setOpacity(float opacity) { opacity_ = opacity; }

bool RenderEngine::initEGL() {
    gl_context_.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (gl_context_.display == EGL_NO_DISPLAY) return false;
    eglInitialize(gl_context_.display, nullptr, nullptr);
    return createEGLContext() && createEGLSurface(native_window_);
}

void RenderEngine::cleanupEGL() {
    if (gl_context_.display != EGL_NO_DISPLAY) {
        eglMakeCurrent(gl_context_.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (gl_context_.context != EGL_NO_CONTEXT) {
            eglDestroyContext(gl_context_.display, gl_context_.context);
        }
        eglTerminate(gl_context_.display);
    }
    gl_context_.reset();
}

bool RenderEngine::createEGLContext() {
    EGLint attribs[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT, EGL_NONE};
    EGLint num_configs;
    eglChooseConfig(gl_context_.display, attribs, &gl_context_.config, 1, &num_configs);
    EGLint ctx_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    gl_context_.context = eglCreateContext(gl_context_.display, gl_context_.config, EGL_NO_CONTEXT, ctx_attribs);
    return gl_context_.context != EGL_NO_CONTEXT;
}

bool RenderEngine::createEGLSurface(ANativeWindow* window) {
    gl_context_.surface = eglCreateWindowSurface(gl_context_.display, gl_context_.config, window, nullptr);
    return gl_context_.surface != EGL_NO_SURFACE && makeCurrent();
}

void RenderEngine::destroyEGLSurface() {
    if (gl_context_.surface != EGL_NO_SURFACE) {
        eglDestroySurface(gl_context_.display, gl_context_.surface);
        gl_context_.surface = EGL_NO_SURFACE;
    }
}

bool RenderEngine::makeCurrent() {
    return eglMakeCurrent(gl_context_.display, gl_context_.surface, gl_context_.surface, gl_context_.context) == EGL_TRUE;
}

void RenderEngine::swapBuffers() { eglSwapBuffers(gl_context_.display, gl_context_.surface); }
bool RenderEngine::initGL() { glViewport(0, 0, screen_width_, screen_height_); return true; }
void RenderEngine::cleanupGL() {}
bool RenderEngine::compileShaders() { return true; }
bool RenderEngine::createBuffers() { return true; }
void RenderEngine::setupVertexAttributes() {}
GLuint RenderEngine::loadShader(GLenum type, const char* src) { return 0; }
GLuint RenderEngine::createProgram(const char* vs, const char* fs) { return 0; }
void RenderEngine::deleteShaders() {}
bool RenderEngine::loadTexture(const uint8_t* data, int w, int h) { return true; }
void RenderEngine::updateTexture(const uint8_t* data, int w, int h) {}
void RenderEngine::deleteTextures() {}
void RenderEngine::clearScreen() { glClear(GL_COLOR_BUFFER_BIT); }
void RenderEngine::drawQuad() {}
void RenderEngine::drawOverlay() {}
void RenderEngine::updateFPS() {}
void RenderEngine::renderThreadFunc() {
    while (!should_stop_) {
        if (visible_) renderFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

} // namespace client
