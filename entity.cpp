#include "entity.h"

Entity::Entity()
{
    mX = mY = 0;
    mWidth = mHeight = 32;
    mDynamic = false;
}

void Entity::setPos(float x, float y)
{
    mX = x;
    mY = y;
}

void Entity::setSize(float width, float height)
{
    mWidth = width;
    mHeight = height;
}

SDL_Rect Entity::getRect()
{
    SDL_Rect rect;
    rect.x = (int)mX;
    rect.y = (int)mY;
    rect.w = (int)mWidth;
    rect.h = (int)mHeight;

    return rect;
}
