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

    return (minX < other.maxX && maxX > other.minX && minY < other.maxY && maxY > other.minY);
}

bool AABB::FitsIn(const AABB& other)
{
    return (minX > other.minX && maxX <= other.maxX && minY > other.minY && maxY <= other.maxY);
}

glm::vec2 AABB::GetDistance(const AABB& other)
{
    glm::vec2 center(minX + 0.5f * (maxX - minX), minY + 0.5f * (maxY - minY));
    glm::vec2 otherCenter(
        other.minX + 0.5f * (other.maxX - other.minX),
        other.minY + 0.5f * (other.maxY - other.minY));

    return otherCenter - center;
}

/**
 * Returns a vector of the intersected area of the two AABB
 *  Experimental.
 */
glm::vec2 AABB::GetIntersectionDepth(const AABB& other)
{
    glm::vec2 depth(GetWidth(), GetHeight()); // XXX to avoid 0 depth

    if (minX < other.maxX && other.maxX < maxX)
        depth.x = -(other.maxX - minX);
    else if (maxX > other.minX && other.minX > minX)
        depth.x = maxX - other.minX;
    if (minY < other.maxY && other.maxY < maxY)
        depth.y = -(other.maxY - minY);
    else if (maxY > other.minY && other.minY > minY)
        depth.y = maxY - other.minY;
    assert(depth.x != 0.0f);
    assert(depth.y != 0.0f);
    return depth;
}
