#pragma once

#include "balls.h"
#include "entity.h"
#include "sprite.h"
#include <glm/glm.hpp>

class Ball : public Entity {
public:
    Ball(float x, float y, float radius, uint texId)
        : Entity(x, y, 2 * radius, 2 * radius)
        , mRadius(radius)
        , mSprite(2 * radius, 2 * radius, texId)
    {
        mType = BALL;
        mSprite.SetPos(x, y);
    }

    float GetRadius() { return mRadius; }
    void Update(Game* game, uint ticks) override;
    bool CheckCollisions(Game* game, uint ticks, glm::vec2& newPos);
    Renderable* GetRenderable() { return &mSprite; }
    void SetColor(const Color& color)
    {
        mSprite.SetColor(color);
    }
    void SetPos(const glm::vec2& pos) override;

private:
    void BrickCollision(glm::vec2& newPos, Entity* brick);
    void BallCollision(glm::vec2& newPos, Ball* other);

    float mRadius;
    Sprite mSprite;
};