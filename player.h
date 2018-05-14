#pragma once

#include "entity.h"
#include <glm/glm.hpp>

class Player : public Entity {
public:
    Player(float x, float y, float width, float height, uint texId);
    ~Player() {}
    void Update(Game* game, uint ticks) override;

private:
    float mAnimSpeed, mSpeed;
    bool CheckCollisions(Game* game, uint ticks, glm::vec2& newPos);
    void BrickCollision(glm::vec2& newPos, Entity* brick);
    void BallCollision(Game* game, uint ticks, glm::vec2& newPos, Entity* other);
};
