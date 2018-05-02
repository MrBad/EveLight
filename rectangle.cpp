#include "vertex.h"
#include "rectangle.h"
#include <iostream>

using namespace std;

Rectangle::Rectangle(float x, float y, float width, float height)
{
    mVertices.resize(4);
    mIndexes = { LB, LT, LT, RT, RT, RB, RB, LB };
    mDrawType = GL_LINES;
    mX = x;
    mY = y;
    mWidth = width;
    mHeight = height;
    Update();
}

void Rectangle::SetPos(float x, float y)
{
    if (mX == x && mY == y)
        return;
    mX = x;
    mY = y;
    Update();
}

void Rectangle::SetColor(const Color &color)
{
   mColor = color;
   Update();
}

void Rectangle::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    mColor = Color(r, g, b, a);
    Update();
}

void Rectangle::Update()
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
    mDirty = true;
}

FilledRectangle::FilledRectangle(float x, float y, float width, float height)
{
    mVertices.resize(4);
    mIndexes = { LB, LT, RT, RT, RB, LB };
    mDrawType = GL_TRIANGLES;
    mX = x;
    mY = y;
    mWidth = width;
    mHeight = height;
    Update();
}
