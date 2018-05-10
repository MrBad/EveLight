#pragma once

#include <glm/glm.hpp>

class AABB {
public:
    AABB(float x1, float y1, float x2, float y2);
    bool Intersects(const AABB& other);
    bool FitsIn(const AABB& other);
    /* Gets the distance between center of rectangles. */
    glm::vec2 GetDistance(const AABB& other);

private:
    float mMinX, mMinY, mMaxX, mMaxY;
};
