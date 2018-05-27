#pragma once

#include "aabb.h"
#include "renderable.h"
#include <glm/glm.hpp>

class Game;

class Entity {
public:
    Entity(float x, float y, float width, float height)
        : mType(0)
        , mPos(x, y)
        , mDim(width, height)
        , mVelocity(0)
    {
    }
    virtual ~Entity() {}

    virtual void SetPos(const glm::vec2& pos) { mPos = pos; }
    const glm::vec2& GetPos() const { return mPos; }

    const glm::vec2& GetDim() const { return mDim; }
    float GetWidth() const { return mDim.x; }
    float GetHeight() const { return mDim.y; }

    AABB GetAABB() const
    {
        return AABB(mPos.x, mPos.y, mPos.x + mDim.x, mPos.y + mDim.y);
    }

    void SetVelocity(const glm::vec2& velocity) { mVelocity = velocity; }
    const glm::vec2& GetVelocity() { return mVelocity; }

    int GetType() const { return mType; }

    virtual void Update(Game* game, uint ticks)
    {
        (void)game;
        (void)ticks;
    }

protected:
    int mType; // this will be game specific.
    glm::vec2 mPos;
    glm::vec2 mDim;
    glm::vec2 mVelocity;
};
