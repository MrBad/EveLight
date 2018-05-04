/**
 * A small test game
 */
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "entity.h"
#include "balls.h"
#include "texture.h"
#include "sprite.h"

bool Balls::onGameInit()
{
    mProgram.Create("res/shaders/simple");

    Texture *texture;
    std::vector<uint8_t> imageBuf = {255, 0, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255};
    texture = new Texture("default", 2, 2, imageBuf);
    mTexMgr.Add("default", texture);
    mTexMgr.Add("earth", "res/textures/earth.png");
    mTexMgr.Add("wallpaper", "res/textures/wallpaper.png");
    mTexMgr.Add("circle", "res/textures/circle.png");

    Rectangle *rect = new Rectangle(10, 10, mWindow.getWidth() / 2, mWindow.getHeight() / 2);
    rect->SetColor(Color(255, 0, 0, 255));
    mRects.push_back(rect);

    rect = new FilledRectangle(200, 200, 200, 200);
    rect->SetColor(0, 128, 0, 128);
    mRects.push_back(rect);

    rect = new Sprite(500, 400, 200, 150, mTexMgr.Get("earth"), mProgram);
    rect->SetColor(255, 255, 255, 255);
    mRects.push_back(rect);

    rect = new Sprite(500, 100, 150, 150, mTexMgr.Get("default"), mProgram);
    rect->SetColor(255, 255, 255, 200);
    mRects.push_back(rect);

    rect = new Sprite(400, 500, 150, 150, mTexMgr.Get("circle"), mProgram);
    rect->SetColor(100, 255, 100, 200);
    mRects.push_back(rect);

    rect = new FilledRectangle(450, 150, 200, 200);
    rect->SetColor(0, 128, 128, 128);
    mRects.push_back(rect);
    return true;
}

bool Balls::onGameUpdate(uint32_t ticks)
{
    (void)ticks;
    
    float speed = 0.3f * ticks;
    float scaleSpeed = 1.0f + (float)ticks/1000;

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

    for (uint i = 0; i < mRects.size(); i++)
        mRects[i]->Draw();

    //cout << mFPS << endl;

    return true;
}

void Balls::onGameDelete()
{
}
