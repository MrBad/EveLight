#include <iostream>
#include <GL/glew.h>
#include "window.h"

Window::Window()
{
    mWidth = 0;
    mHeight = 0;
    mTitle.clear();
}

bool Window::Init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cerr << "Cannot init SDL: " + string(SDL_GetError());
        return false;
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
    if (!(mWindow = SDL_CreateWindow(
              mTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
              mWidth, mHeight, SDL_WINDOW_OPENGL)))
    {
        cerr << "Cannot create window: " + string(SDL_GetError());
        return false;
    }
    if (!(mGLContext = SDL_GL_CreateContext(mWindow)))
    {
        cerr << "Cannot create GL context: " + string(SDL_GetError());
        return false;
    }
    if (glewInit() != GLEW_OK)
    {
        cerr << "Cannot init glew";
        return false;
    }
    SetSwapInterval(0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    setClearColor(0, 0, 0.3, 1);
    Clear();

    return true;
}

void Window::setTitle(const string &title)
{
    mTitle = title;
    SDL_SetWindowTitle(mWindow, mTitle.c_str());
}

void Window::setSize(int width, int height)
{
    mWidth = width;
    mHeight = height;
    SDL_SetWindowSize(mWindow, mWidth, mHeight);
}

void Window::setClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void Window::Clear()
{
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Window::SetSwapInterval(int interval)
{
    return SDL_GL_SetSwapInterval(interval) == 0;
}

void Window::Update()
{
    SDL_GL_SwapWindow(mWindow);
}

Window::~Window()
{
    SDL_GL_DeleteContext(mGLContext);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
