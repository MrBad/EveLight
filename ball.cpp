#include "ball.h"

Ball::Ball(float x, float y, float width, float height, uint texId)
    : Entity(x, y, width, height, texId)
{
    SetStatic(false);
}

void Ball::Update(Game *game, uint ticks)
{
    (void)game;
    SetPos(mX + mVelocity.x * ticks, mY + mVelocity.y * ticks);
}
