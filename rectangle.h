#pragma once

#include "aabb.h"
#include "renderable.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class Rectangle : public Renderable {
public:
    Rectangle() {}
    Rectangle(float x, float y, float width, float height, const Color& color);
    void SetPos(float x, float y);
    void SetColor(const Color& color);
    void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    float GetX() { return mX; }
    float GetY() { return mY; }
    float GetWidth() { return mWidth; }
    float GetHeight() { return mHeight; }
    AABB GetAABB() { return AABB(mX, mY, mX + mWidth, mY + mHeight); }

protected:
    enum { LB, LT, RT, RB };
    float mX, mY, mWidth, mHeight;
    Color mColor;
    void BuildVertices();
};
