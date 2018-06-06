#include "sprite.h"

namespace evl {

Sprite::Sprite(float x, float y, float width, float height, const Color& color, uint texId)
    : FilledRectangle(x, y, width, height, color)
{
    textureId = texId;
}

Sprite::Sprite(float width, float height, uint texId)
    : FilledRectangle(0, 0, width, height, Color(255, 255, 255, 255))
{
    textureId = texId;
}
}
