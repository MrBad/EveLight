/**
 * A small test game
 */
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <random>
#include "entity.h"
#include "balls.h"
#include "texture.h"
#include "sprite.h"

bool Balls::onGameInit()
{
    mProgram.Create("res/shaders/simple");
    mTexMgr.Add("circle", "res/textures/circle.png");
    mTexMgr.Add("earth", "res/textures/earth.png");

    mCamera.SetPos(10 * mWindow.getWidth() / 2, 10 * mWindow.getHeight() / 2);

    mRenderer.Init();

    std::mt19937 rng(time(NULL));
    std::uniform_int_distribution<int> gen(0, mWindow.getWidth() * 30);

    for (int i = 0; i < 300000; i++)
    {
        const Color color(gen(rng) % 255, gen(rng) % 255, gen(rng) % 255, 200);
        FilledRectangle *obj = new FilledRectangle(gen(rng), gen(rng), 32, 32, color);
        mRenderer.Add(obj);
    }
    for (int i = 0; i < 300000; i++)
    {
        const Color color(gen(rng) % 255, gen(rng) % 255, gen(rng) % 255, 200);
        Rectangle *obj = new Rectangle(gen(rng), gen(rng), 32, 32, color);
        mRenderer.Add(obj);
    }
    for (int i = 0; i < 300000; i++)
    {
        const Color color(gen(rng) % 255, gen(rng) % 255, gen(rng) % 255, 200);
        uint textureId = i < 150000 ? mTexMgr.Get("circle")->getId() : mTexMgr.Get("earth")->getId();
        Sprite *obj = new Sprite(gen(rng), gen(rng), 32, 32, color, textureId);
        mRenderer.Add(obj);
    }

    return true;
}

void Balls::updateCamera(uint ticks)
{
    float speed = 0.3f * ticks;
    float scaleSpeed = 1.0f + (float)ticks / 1000;

    if (mInMgr.isKeyPressed(K_q))
        mCamera.SetScale(mCamera.GetScale() * scaleSpeed);
    else if (mInMgr.isKeyPressed(K_e))
        mCamera.SetScale(mCamera.GetScale() / scaleSpeed);
    if (mInMgr.isKeyPressed(K_w))
        mCamera.SetPos(mCamera.GetX(), mCamera.GetY() + speed);
    else if (mInMgr.isKeyPressed(K_s))
        mCamera.SetPos(mCamera.GetX(), mCamera.GetY() - speed);
    if (mInMgr.isKeyPressed(K_a))
        mCamera.SetPos(mCamera.GetX() - speed, mCamera.GetY());
    else if (mInMgr.isKeyPressed(K_d))
        mCamera.SetPos(mCamera.GetX() + speed, mCamera.GetY());
    // send camera matrix to opengl //
    mCamera.SetMatrix(mProgram.getId(), "MVP");
}

bool Balls::onGameUpdate(uint32_t ticks)
{
    updateCamera(ticks);

    mRenderer.Draw();

    cout << mFPS << endl;

    return true;
}

void Balls::onGameDelete()
{
}
