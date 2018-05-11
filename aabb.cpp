#include "aabb.h"

AABB::AABB(float x1, float y1, float x2, float y2)
    : minX(x1)
    , minY(y1)
    , maxX(x2)
    , maxY(y2)
{
}

bool AABB::Intersects(const AABB& other)
{
    assert(minX < maxX);
    assert(minY < maxY);
    assert(other.minX < other.maxX);
    assert(other.minY < other.maxY);

    return (minX < other.maxX &&
            maxX > other.minX &&
            minY < other.maxY &&
            maxY > other.minY);
}

bool AABB::FitsIn(const AABB& other)
{
    return (minX > other.minX &&
            maxX <= other.maxX &&
            minY > other.minY &&
            maxY <= other.maxY);
}

glm::vec2 AABB::GetDistance(const AABB& other)
{
    glm::vec2 center(minX + 0.5f * (maxX - minX), minY + 0.5f * (maxY - minY));
    glm::vec2 otherCenter(
        other.minX + 0.5f * (other.maxX - other.minX),
        other.minY + 0.5f * (other.maxY - other.minY));

    return otherCenter - center;
}
