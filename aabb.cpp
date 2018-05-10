#include "aabb.h"

AABB::AABB(float x1, float y1, float x2, float y2)
    : mMinX(x1), mMinY(y1), mMaxX(x2), mMaxY(y2)
{
}

bool AABB::Intersects(const AABB &other)
{
    assert(mMinX < mMaxX);
    assert(mMinY < mMaxY);
    assert(other.mMinX < other.mMaxX);
    assert(other.mMinY < other.mMaxY);

    return (mMinX < other.mMaxX &&
            mMaxX > other.mMinX &&
            mMinY < other.mMaxY &&
            mMaxY > other.mMinY);
}

bool AABB::FitsIn(const AABB &other)
{
    return (mMinX > other.mMinX &&
            mMaxX <= other.mMaxX &&
            mMinY > other.mMinY &&
            mMaxY <= other.mMaxY);
}

glm::vec2 AABB::GetDistance(const AABB &other)
{
    glm::vec2 center(mMinX + 0.5f * (mMaxX - mMinX), mMinY + 0.5f * (mMaxY - mMinY));
    glm::vec2 otherCenter(
        other.mMinX + 0.5f * (other.mMaxX - other.mMinX),
        other.mMinY + 0.5f * (other.mMaxY - other.mMinY));

    return otherCenter - center;
}
