#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace evl {

class Window {
public:
    ~Window();
    bool Init();
    void setClearColor(float r, float g, float b, float a);
    bool setSwapInterval(int interval);
    void Clear();
    SDL_Window* getSDLWindow() { return mWindow; }
    int GetWidth() { return mWidth; }
    int GetHeight() { return mHeight; }
    void Update();
    void setTitle(const std::string& title);
    void setSize(int width, int height);

private:
    std::string mTitle;
    int mWidth;
    int mHeight;
    SDL_Window* mWindow;
    SDL_GLContext mGLContext;
};
}
