#pragma once

#include "entity.h"
#include "game.h"
#include "gl_program.h"
#include "multi_sprite.h"
#include "rectangle.h"
#include "renderer.h"
#include "sprite.h"
#include "texture.h"
#include "texture_manager.h"
#include <GL/glew.h>

enum EntityType {
    BALL,
    BRICK,
    PLAYER
};

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

    std::vector<Entity*> mEntities;
    Entity* mPlayer;

    uint mMapX, mMapY;

    void CameraUpdate(uint ticks);
    void buildMap();
    void DynamicStaticCollision(Entity *eDynamic, Entity *eStatic);
    void DynamicDynamicCollision(Entity *a, Entity *b);
};
