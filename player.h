#pragma once

#include "entity.h"
#include "multi_sprite.h"
#include <glm/glm.hpp>

class Player : public Entity {
public:
    Player(float x, float y, float width, float height, uint texId);

    void SetSpriteNumFrames(uint x, uint y) { mSprite.SetNumFrames(x, y); }
    void SetPos(const glm::vec2& pos);

    void Update(Game* game, uint ticks) override;
    Renderable* GetRenderable() { return &mSprite; }

private:
    float mAnimSpeed;
    float mSpeed;
    MultiSprite mSprite;

    bool CheckCollisions(Game* game, uint ticks, glm::vec2& newPos);
    void BrickCollision(glm::vec2& newPos, Entity* brick);
    void BallCollision(Game* game, uint ticks, glm::vec2& newPos, Entity* other);
};
