#include <iostream>
#include "game.h"
#include "timer.h"

Game::Game(const string &title, int width, int height)
{
    mWindow.setTitle(title);
    mWindow.setSize(width, height);
    mFPSNumFrames = 0;
    mFPS = 0;
    mFPSTicks = 0;
    mState = GAME_NONE;
}

bool Game::Start()
{
    if (!mWindow.Init())
        return false;
    mState = GAME_PLAYING;
    if (onGameInit())
        Loop();
    onGameDelete();

    return true;
}

void Game::UpdateFPS(uint32_t ticks)
{
    mFPSTicks += ticks;
    mFPSNumFrames++;
    if (mFPSTicks >= 1000)
    {
        mFPS = mFPSNumFrames;
        mFPSNumFrames = 0;
        mFPSTicks = 0;
    }
}

void Game::Loop()
{
    Timer timer(SDL_GetTicks());
    uint32_t diffTicks;

    while (mState == GAME_PLAYING)
    {
        diffTicks = timer.Update(SDL_GetTicks());
        UpdateFPS(diffTicks);
        mInMgr.Update();
        if (mInMgr.isQuitRequested() || mInMgr.isKeyPressed(K_ESCAPE))
            mState = GAME_OVER;
        mWindow.Clear();
        if (!onGameUpdate(diffTicks))
            mState = GAME_OVER;
        mWindow.Update();
    }
}
