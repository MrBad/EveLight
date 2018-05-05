#include "filled_rectangle.h"

FilledRectangle::FilledRectangle(float x, float y, float width, float height, const Color &color)
{
    mVertices.resize(4);
    mIndexes = { LB, LT, RT, RT, RB, LB };
    mDrawType = D_TRIANGLES;
    mX = x;
    mY = y;
    mWidth = width;
    mHeight = height;
    mColor = color;
    BuildVertices();
}
