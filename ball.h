#pragma once

#include "sprite.h"
#include <glm/glm.hpp>

class Ball : public Sprite {
public:
    Ball(float x, float y, float width, float height, const Color& color, uint texId);
    ~Ball();
    void SetVelocity(const glm::vec2& velocity);
    glm::vec2 GetVelocity() { return mVelocity; }
    void Update(uint ticks);

private:
    glm::vec2 mVelocity;
};