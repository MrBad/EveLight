#pragma once

#include <glm/glm.hpp>

class AABB {
public:
    AABB(float x1, float y1, float x2, float y2);
    bool Intersects(const AABB& other);
    bool FitsIn(const AABB& other);
    /* Gets the distance between center of rectangles. */
    glm::vec2 GetDistance(const AABB& other);
    float GetWidth() { return maxX - minX; }
    float GetHeight() { return maxY - minY; }
    glm::vec2 GetMin() { return glm::vec2(minX, minY); }
    glm::vec2 GetMax() { return glm::vec2(maxX, maxY); }

    float minX, minY, maxX, maxY;
};
