#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>

class Entity
{
  public:
    Entity();
    ~Entity() {}
    void setPos(float x, float y);
    float getX() { return mX; }
    float getY() { return mY; }
    void setX(float x) { mX = x; }
    void setY(float y) { mY = y; }
    void setSize(float width, float height);
    float getHeight() { return mHeight; }
    float getWidth() { return mWidth; }
    SDL_Rect getRect();

  private:
    bool mDynamic;
    float mX, mY, mWidth, mHeight;
};

#endif
