#ifndef GAME_H
#define GAME_H

#include "window.h"
#include "input_manager.h"

using namespace std;

class Game
{
  public:
    Game(const string &title, int width, int height);
    ~Game() {}
    bool Start();
    virtual bool onGameInit() { return true; };
    virtual bool onGameUpdate(uint32_t diffTicks) = 0;
    virtual void onGameDelete() {}

  protected:
    void Loop();
    void UpdateFPS(uint32_t ticks);
    enum
    {
        GAME_NONE,
        GAME_PLAYING,
        GAME_OVER
    } mState;
    Window mWindow;
    InputManager mInMgr;
    uint32_t mFPSTicks;
    uint32_t mFPSNumFrames;
    uint32_t mFPS;
};

#endif
