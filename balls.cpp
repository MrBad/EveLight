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

    float points[] = {
        0.5f, 0.5f,
        0.0f, 0.5f,
        0.0f, 0.0f,

        0.0f, 0.0f,
        0.5f, 0.0f,
        0.5f, 0.5f,
    };

    mVertexArray = 0;
    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    mVertexBuffer = 0;
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLint posAttr = mProgram.getAttribute("position");
    glEnableVertexAttribArray(posAttr);
    glVertexAttribPointer(posAttr, 2, GL_FLOAT, GL_FALSE, 0, 0);

    return true;
}

bool Balls::onGameUpdate(uint32_t ticks)
{
    (void)ticks;

    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(mVertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //std::cout << mFPS << std::endl;
    return true;
}

void Balls::onGameDelete()
{
}
