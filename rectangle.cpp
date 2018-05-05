#include "vertex.h"
#include "rectangle.h"

Rectangle::Rectangle(float x, float y, float width, float height, const Color& color)
    : mX(x), mY(y), mWidth(width), mHeight(height), mColor(color)
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

void Rectangle::SetColor(const Color &color)
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

    mVertices[LB].SetUV(0, 0);
    mVertices[LT].SetUV(0, 1);
    mVertices[RT].SetUV(1, 1);
    mVertices[RB].SetUV(1, 0);
}
