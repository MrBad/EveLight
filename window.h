#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <SDL2/SDL.h>

using namespace std;

class Window
{
  public:
    Window();
    ~Window();
    bool Init();
    void setClearColor(float r, float g, float b, float a);
    bool setSwapInterval(int interval);
    void Clear();
    SDL_Window *getSDLWindow() { return mWindow; }
    int getWidth() { return mWidth; }
    int getHeight() { return mHeight; }
    void Update();
    void setTitle(const string &title);
    void setSize(int width, int height);

  private:
    string mTitle;
    int mWidth;
    int mHeight;
    SDL_Window *mWindow;
    SDL_GLContext mGLContext;
};

#endif
