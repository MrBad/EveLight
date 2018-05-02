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
    std::vector<uint8_t> imageBuf = { 255,0,0,255, 0,255,0,255, 0,255,0,255, 0,0,255,255 };
    texture = new Texture("default", 2, 2, imageBuf);
    mTexMgr.Add("test", texture);
    mTexMgr.Add("earth", "res/textures/earth.png");
    mTexMgr.Add("wallpaper", "res/textures/wallpaper.png");

    Rectangle *rect = new Rectangle(-0.95, -0.95, 0.45, 0.45);
    rect->SetColor(Color(255, 0, 0, 255));
    mRects.push_back(rect);

    rect = new FilledRectangle(-0.25, -0.25, 0.45, 0.45);
    rect->SetColor(0, 128, 0, 255);
    mRects.push_back(rect);
    
    rect = new Sprite(0.5, 0.5, 0.45, 0.45, mTexMgr.Get("earth"), mProgram);
    rect->SetColor(255, 255, 255, 255);
    mRects.push_back(rect);

    rect = new Sprite(0.5, -0.5, 0.45, 0.45, mTexMgr.Get("test"), mProgram);
    rect->SetColor(255, 255, 255, 255);
    mRects.push_back(rect);

    rect = new FilledRectangle(-0.85, -0.85, 0.45, 0.45);
    rect->SetColor(0, 128, 128, 180);
    mRects.push_back(rect);
    return true;
}

bool Balls::onGameUpdate(uint32_t ticks)
{
    (void)ticks;

    for (uint i = 0; i < mRects.size(); i++)
        mRects[i]->Draw();

    cout << mFPS << endl;

    return true;
}

void Balls::onGameDelete()
{
}
