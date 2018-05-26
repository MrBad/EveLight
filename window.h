#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>

using namespace std;

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
    void setTitle(const string& title);
    void setSize(int width, int height);

private:
    string mTitle;
    int mWidth;
    int mHeight;
    SDL_Window* mWindow;
    SDL_GLContext mGLContext;
};

#endif
