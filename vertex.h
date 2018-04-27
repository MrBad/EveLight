#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Vertex
{
    public:
    void SetPos(float x, float y) { mX = x, mY = y; }
    private:
    float mX, mY;
};