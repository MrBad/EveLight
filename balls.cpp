/**
 * A small test game
 */
#include <iostream>
#include "entity.h"
#include "balls.h"

bool Balls::onGameInit()
{
    mRenderer = SDL_CreateRenderer(mWindow.getSDLWindow(), -1, 
        SDL_RENDERER_ACCELERATED/*|SDL_RENDERER_PRESENTVSYNC*/);
    mPlayer.setPos(0, 0);
    mPlayer.setSize(100, 100);

    return true;
}

bool Balls::onGameUpdate(uint32_t ticks)
{
    int d = 0.5 * ticks;
    if (mInMgr.isKeyPressed(K_w) && mPlayer.getY() >= d)
        mPlayer.setY(mPlayer.getY() - d);
    if (mInMgr.isKeyPressed(K_s) && mPlayer.getY() <= (mWindow.getHeight() - mPlayer.getHeight() - d))
        mPlayer.setY(mPlayer.getY() + d);
    if (mInMgr.isKeyPressed(K_a) && mPlayer.getX() >= d)
        mPlayer.setX(mPlayer.getX() - d);
    if (mInMgr.isKeyPressed(K_d) && mPlayer.getX() <= (mWindow.getWidth() - mPlayer.getWidth() - d))
        mPlayer.setX(mPlayer.getX() + d);

    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0x44, 0xFF);
    SDL_RenderClear(mRenderer);
    SDL_SetRenderDrawColor(mRenderer, 0, 0x44, 0, 0xFF);
    SDL_Rect rect = mPlayer.getRect();
    SDL_RenderFillRect(mRenderer, &rect);
    SDL_RenderPresent(mRenderer);
    std::cout << mFPS << std::endl;
    if (ticks < 20) SDL_Delay(20-ticks);
    return true;
}

void Balls::onGameDelete()
{
    SDL_DestroyRenderer(mRenderer);
}
