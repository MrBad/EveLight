#include "entity.h"

Entity::Entity(float x, float y, float width, float height, uint texId)
   : MultiSprite(x, y, width, height, Color(255, 255, 255, 255), texId)
   , mIsStatic(true)
   , mType(0)
   , mVelocity(0)
   
{
}
