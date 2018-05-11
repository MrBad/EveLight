#pragma once

#include "aabb.h"
#include "sprite.h"

class MultiSprite : public Sprite {
public:
    MultiSprite(float width, float height, uint texId);
    MultiSprite(float x, float y, float width, float height, const Color& color, uint texId);
    void SetNumFrames(int numX, int numY);
    void SetFrame(int x, int y);

private:
    int mNumX, mNumY;
};
