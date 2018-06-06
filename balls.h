#pragma once

#include "evelight/entity.h"
#include "evelight/game.h"
#include "evelight/gl_program.h"
#include "evelight/multi_sprite.h"
#include "evelight/rectangle.h"
#include "evelight/renderer.h"
#include "evelight/sprite.h"
#include "evelight/texter.h"
#include "evelight/texture.h"
#include "evelight/texture_manager.h"
#include "player.h"
#include <GL/glew.h>

enum EntityType {
    NO_ENTITY,
    BALL,
    BRICK,
    PLAYER
};

class Balls : public Game {
public:
    Balls(const std::string& title, int width, int height)
        : Game(title, width, height)
    {
    }
    ~Balls() {}
    bool onGameInit() override final;
    bool onGameUpdate(uint diffTicks) override final;
    void onGameDelete() override final;

private:
    GLProgram mProgram;
    TextureManager mTexMgr;
    Renderer mRenderer;
    Texter mTexter;

    Player* mPlayer;

    uint mMapX, mMapY;

    void CameraUpdate(uint ticks);
    void buildMap();
};
