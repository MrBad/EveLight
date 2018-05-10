#pragma once

#include "camera.h"
#include "input_manager.h"
#include "window.h"

class Game {
public:
    Game(const string& title, int width, int height);
    ~Game() {}
    bool Start();
    virtual bool onGameInit() { return true; };
    virtual bool onGameUpdate(uint diffTicks) = 0;
    virtual void onGameDelete() {}

protected:
    void Loop();
    void UpdateFPS(uint ticks);
    enum {
        GAME_NONE,
        GAME_PLAYING,
        GAME_OVER
    } mState;
    Window mWindow;
    InputManager mInMgr;
    Camera mCamera;
    uint mFPSTicks;
    uint mFPSNumFrames;
    uint mFPS;
};
