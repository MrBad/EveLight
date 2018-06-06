#include "multi_sprite.h"

namespace evl {

MultiSprite::MultiSprite(float x, float y, float width, float height, const Color& color, uint texId)
    : Sprite(x, y, width, height, color, texId)
    , mNumX(1)
    , mNumY(1)
{
}

MultiSprite::MultiSprite(float width, float height, uint texId)
    : Sprite(width, height, texId)
    , mNumX(1)
    , mNumY(1)
{
}

void MultiSprite::SetNumFrames(int numX, int numY)
{
    mNumX = numX;
    mNumY = numY;
    SetFrame(0, 0);
}

void MultiSprite::SetFrame(int x, int y)
{
    y = mNumY - y - 1; // reverse, to count from top left
    float fx = 1.0f / mNumX,
          fy = 1.0f / mNumY;

    AABB uv(x * fx, y * fy, (x + 1) * fx, (y + 1) * fy);
    SetUV(uv);
}
}
