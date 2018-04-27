#ifndef MY_GAME_H
#define MY_GAME_H

#include <GL/glew.h>
#include "game.h"
#include "entity.h"
#include "gl_program.h"

class Balls : public Game
{
    public:
        Balls(const string &title, int width, int height) : Game(title, width, height) {}
        ~Balls() {}
        bool onGameInit() override final;
        bool onGameUpdate(uint32_t diffTicks) override final;
        void onGameDelete() override final;

    private:
        Entity mPlayer;
        GLProgram mProgram;
        GLuint mVertexArray;
        GLuint mVertexBuffer;
};

#endif

