#include "ball.h"

Ball::Ball(float x, float y, float width, float height, const Color& color, uint texId)
    : Sprite(x, y, width, height, color, texId)
    , mVelocity(0)
{
}

void Ball::SetVelocity(const glm::vec2& velocity) { mVelocity = velocity; }

void Ball::Update(uint ticks)
{
    SetPos(mX + mVelocity.x * ticks, mY + mVelocity.y * ticks);
}

Ball::~Ball() {}