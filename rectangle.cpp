#include "rectangle.h"
#include "vertex.h"

Rectangle::Rectangle(float x, float y, float width, float height, const Color& color)
    : mX(x)
    , mY(y)
    , mWidth(width)
    , mHeight(height)
    , mColor(color)
    , mUV(0, 0, 1, 1)
{
    mVertices.resize(4);
    mIndexes = { LB, LT, LT, RT, RT, RB, RB, LB };
    mDrawType = D_LINES;
    BuildVertices();
}

void Rectangle::SetPos(float x, float y)
{
    if (mX == x && mY == y)
        return;
    mX = x;
    mY = y;
    BuildVertices();
}

void Rectangle::SetPos(const glm::vec2& pos)
{
    SetPos(pos.x, pos.y);
}

void Rectangle::SetColor(const Color& color)
{
    mColor = color;
    BuildVertices();
}

void Rectangle::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    mColor = Color(r, g, b, a);
    BuildVertices();
}

void Rectangle::BuildVertices()
{
    mVertices[LB].SetPos(mX, mY);
    mVertices[LT].SetPos(mX, mY + mHeight);
    mVertices[RT].SetPos(mX + mWidth, mY + mHeight);
    mVertices[RB].SetPos(mX + mWidth, mY);

    for (uint i = 0; i < mVertices.size(); i++)
        mVertices[i].SetColor(mColor);

    mVertices[LB].SetUV(mUV.minX, mUV.minY);
    mVertices[LT].SetUV(mUV.minX, mUV.maxY);
    mVertices[RT].SetUV(mUV.maxX, mUV.maxY);
    mVertices[RB].SetUV(mUV.maxX, mUV.minY);
}

void Rectangle::SetUV(AABB uv)
{
    mUV = uv;
    BuildVertices();
}
