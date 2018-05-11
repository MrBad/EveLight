#pragma once

#include "aabb.h"
#include "renderable.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class Rectangle : public Renderable {
public:
    Rectangle()
        : mUV(0, 0, 1, 1)
    {
    }
    Rectangle(float x, float y, float width, float height, const Color& color);
    void SetPos(float x, float y);
    void SetPos(const glm::vec2& pos);
    glm::vec2 GetPos() { return glm::vec2(mX, mY); }
    void SetColor(const Color& color);
    void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void SetUV(AABB uv);
    float GetX() { return mX; }
    float GetY() { return mY; }
    float GetWidth() { return mWidth; }
    float GetHeight() { return mHeight; }
    AABB GetAABB() { return AABB(mX, mY, mX + mWidth, mY + mHeight); }

protected:
    enum { LB,
        LT,
        RT,
        RB };
    float mX, mY, mWidth, mHeight;
    Color mColor;
    AABB mUV;
    void BuildVertices();
};
