#include "ball.h"
#include "balls.h"

Ball::Ball(float x, float y, float width, float height, uint texId)
    : Entity(x, y, width, height, texId)
{
    SetStatic(false);
}

void Ball::Update(Game* game, uint ticks)
{
    (void)game;
    //SetPos(mX + mVelocity.x * ticks, mY + mVelocity.y * ticks);
    glm::vec2 pos = GetPos();
    pos += mVelocity * (float)ticks;
    CheckCollisions(game, ticks, pos);
}

bool Ball::CheckCollisions(Game* game, uint ticks, glm::vec2& newPos)
{
    (void) ticks;
    bool collided = false;
    const std::vector<Entity*> entities = game->GetEntities();
    for (uint i = 0; i < entities.size(); i++) {
        if (entities[i] == this || entities[i]->GetType() == PLAYER)
            continue;
        if (GetAABB().Intersects(entities[i]->GetAABB())) {
            collided = true;
            switch (entities[i]->GetType()) {
            case BRICK:
                BrickCollision(newPos, entities[i]);
                break;
            case BALL:
                BallCollision(newPos, entities[i]);
                break;
            }
        }
        SetPos(newPos);
        // if (GetAABB().Intersects(entities[i]->GetAABB())) {
        //     printf("Ball collision not solved with %s\n",
        //         entities[i]->GetType() == BALL ? "ball" : "brick");
        // }
    }

    return collided;
}

void Ball::BrickCollision(glm::vec2& newPos, Entity* brick)
{
    AABB pAABB = GetAABB();
    AABB bAABB = brick->GetAABB();

    glm::vec2 depth(GetWidth(), GetHeight());

    if (pAABB.minX < bAABB.maxX && bAABB.maxX < pAABB.maxX)
        depth.x = -(bAABB.maxX - pAABB.minX);
    else if (pAABB.maxX > bAABB.minX && bAABB.minX > pAABB.minX)
        depth.x = pAABB.maxX - bAABB.minX;
    if (pAABB.minY < bAABB.maxY && bAABB.maxY < pAABB.maxY)
        depth.y = -(bAABB.maxY - pAABB.minY);
    else if (pAABB.maxY > bAABB.minY && bAABB.minY > pAABB.minY)
        depth.y = pAABB.maxY - bAABB.minY;

    if (fabs(depth.x) < fabs(depth.y)) {
        mVelocity.x *= -1;
        newPos.x -= depth.x;
    } else {
        mVelocity.y *= -1;
        newPos.y -= depth.y;
    }
}

void Ball::BallCollision(glm::vec2& newPos, Entity* other)
{
    float aRadius = GetRadius();
    float bRadius = other->GetWidth() / 2;
    float minDist = aRadius + bRadius;
    const glm::vec2 v2Dist(
        mX + aRadius - (other->GetX() + bRadius),
        mY + aRadius - (other->GetY() + bRadius));
    float dist = glm::length(v2Dist);
    if (minDist < dist)
        return;
    if (!dist) // XXX bad position, avoid 0
        return;

    float depth = minDist - dist;
    glm::vec2 v2ColDept = glm::normalize(v2Dist) * depth;
    v2ColDept *= 0.5;
    newPos += v2ColDept;
    other->SetPos(other->GetX() - v2ColDept.x, other->GetY() - v2ColDept.y);

    float arc = aRadius * aRadius * aRadius;
    float brc = bRadius * bRadius * bRadius;
    // v1New = (v1 * (vol1 - vol2) + 2 * vol2 * v2) / (vol1 + vol2) //
    glm::vec2 aVNew = mVelocity * (arc - brc);
    aVNew = aVNew + other->GetVelocity() * (2 * brc);
    aVNew = aVNew / (arc + brc);

    glm::vec2 bVNew = other->GetVelocity() * (brc - arc);
    bVNew = bVNew + mVelocity * (2 * arc);
    bVNew = bVNew / (arc + brc);

    mVelocity = aVNew;
    other->SetVelocity(bVNew);
}
