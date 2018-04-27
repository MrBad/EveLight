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

    mSquares[0].Init(-1, -1, 0.5, 0.5);
    mSquares[1].Init(-0.25, -0.25, 0.5, 0.5);
    mSquares[2].Init(0.5, 0.5, 0.5, 0.5);

    return true;
}

bool Balls::onGameUpdate(uint32_t ticks)
{
    (void)ticks;

    for (int i = 0; i < 3; i++)
        mSquares[i].Draw();

    //cout << mFPS << endl;

    return true;
}

void Balls::onGameDelete()
{
}
