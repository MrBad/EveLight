#pragma once

#include "multi_sprite.h"
#include <glm/glm.hpp>

class Game;

class Entity : public MultiSprite {
public:
    Entity(float x, float y, float width, float height, uint texId);
    virtual ~Entity() {}
    void SetVelocity(const glm::vec2& velocity) { mVelocity = velocity; }
    const glm::vec2 GetVelocity() { return mVelocity; }
    bool isStatic() { return mIsStatic; }
    bool isDynamic() { return !mIsStatic; }
    void SetStatic(bool isStatic) { mIsStatic = isStatic; }
    void SetType(int type) { mType = type; }
    int GetType() { return mType; }
    virtual void Update(Game* game, uint ticks)
    {
        (void)game;
        (void)ticks;
    }

protected:
    bool mIsStatic;
    int mType; // this will be game specific.
    glm::vec2 mVelocity;
};
