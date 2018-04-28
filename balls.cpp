/**
 * A small test game
 */
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "entity.h"
#include "balls.h"

bool Balls::onGameInit()
{
    mProgram.Create("res/shaders/simple");

    Rectangle *rect = new Rectangle(-0.95, -0.95, 0.45, 0.45);
    rect->SetColor(Color(255, 0, 0, 255));
    mRects.push_back(rect);

    rect = new Rectangle(-0.25, -0.25, 0.45, 0.45);
    rect->SetColor(0, 255,0, 255);
    mRects.push_back(rect);

    rect = new Rectangle(0.5, 0.5, 0.45, 0.45);
    rect->SetColor(0, 0, 255, 255);
    mRects.push_back(rect);

    rect = new FilledRectangle(0.5, -0.5, 0.45, 0.45);
    rect->SetColor(128, 0, 100, 255);
    mRects.push_back(rect);

    return true;
}

bool Balls::onGameUpdate(uint32_t ticks)
{
    (void)ticks;

    for (uint i = 0; i < mRects.size(); i++)
        mRects[i]->Draw();

    //cout << mFPS << endl;

    return true;
}

void Balls::onGameDelete()
{
}
