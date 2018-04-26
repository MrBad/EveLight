#ifndef MY_GAME_H
#define MY_GAME_H

#include "game.h"
#include "entity.h"

class Balls : public Game
{
    public:
        Balls(const string &title, int width, int height) : Game(title, width, height) {}
        ~Balls() {}
        bool onGameInit() override final;
        bool onGameUpdate(uint32_t diffTicks) override final;
        void onGameDelete() override final;

    private:
        SDL_Renderer *mRenderer;
        SDL_Rect mRect;
        Entity mPlayer;
};

#endif

