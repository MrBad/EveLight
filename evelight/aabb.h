#pragma once

#include <glm/glm.hpp>

namespace evl {

struct AABB {
    float minX, minY, maxX, maxY;

    AABB(float x1, float y1, float x2, float y2);
    bool Intersects(const AABB& other);
    bool FitsIn(const AABB& other) const;
    /* Gets the distance between center of rectangles. */
    glm::vec2 GetDistance(const AABB& other) const;
    float GetWidth() { return maxX - minX; }
    float GetHeight() { return maxY - minY; }
    glm::vec2 GetMin() { return glm::vec2(minX, minY); }
    glm::vec2 GetMax() { return glm::vec2(maxX, maxY); }
    glm::vec2 GetCenter() { return glm::vec2(minX + (maxX - minX) / 2, minY + (maxY - minY) / 2); }
    glm::vec2 GetIntersectionDepth(const AABB& other);
};
}
