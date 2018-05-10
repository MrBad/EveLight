#pragma once

#include "ball.h"
#include "game.h"
#include "gl_program.h"
#include "mesh.h"
#include "rectangle.h"
#include "renderer.h"
#include "sprite.h"
#include "texture.h"
#include "texture_manager.h"
#include <GL/glew.h>

class Balls : public Game {
public:
    Balls(const string& title, int width, int height)
        : Game(title, width, height)
    {
    }
    ~Balls() {}
    bool onGameInit() override final;
    bool onGameUpdate(uint32_t diffTicks) override final;
    void onGameDelete() override final;

private:
    GLProgram mProgram;
    TextureManager mTexMgr;
    Renderer mRenderer;

    vector<Sprite*> mBricks;
    vector<Ball*> mBalls;
    uint mMapX, mMapY;

    void updateCamera(uint ticks);
    void buildMap();
};
