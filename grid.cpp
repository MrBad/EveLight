#include "grid.h"

Grid::Grid()
{
}

Grid::Grid(float x, float y, float width, float height, int step, const Color& color)
    : mX(x)
    , mY(y)
    , mWidth(width)
    , mHeight(height)
    , mStep(step)
    , mColor(color)
{
    mVertices.resize(1 + 2 * (int)(width + height) / step);
    mIndexes.resize(1 + 2 * (int)(width + height) / step);
    mDrawType = D_LINES;

    BuildVertices();
}

void Grid::BuildVertices()
{
    int i = 0;

    for (int x = 0; x < (int)mWidth; x += mStep) {
        mVertices[i].SetPos(x, 0);
        mVertices[i].SetColor(mColor);
        mIndexes[i] = i;
        i++;
        mVertices[i].SetPos(x, mHeight);
        mVertices[i].SetColor(mColor);
        mIndexes[i] = i;
        i++;
    }

    for (int y = 0; y < (int)mHeight; y += mStep) {
        mVertices[i].SetPos(0, y);
        mVertices[i].SetColor(mColor);
        mIndexes[i] = i;
        i++;
        mVertices[i].SetPos(mWidth, y);
        mVertices[i].SetColor(mColor);
        mIndexes[i] = i;
        i++;
    }
}
