#include "player.h"
#include "balls.h"
#include "game.h"

Player::Player(float x, float y, float width, float height, uint texId)
    : Entity(x, y, width, height, texId)
    , mAnimSpeed(0.01f)
    , mSpeed(0.15f)
{
    SetStatic(false);
}

void Player::Update(Game* game, uint ticks)
{
    enum {
        P_STOP,
        P_WALK
    };
    enum {
        F_DOWN,
        F_LEFT,
        F_UP,
        F_RIGHT
    };

    glm::vec2 velocity = glm::vec2(0);

    if (game->mInMgr.isKeyPressed(K_w))
        velocity.y = 1;
    else if (game->mInMgr.isKeyPressed(K_s))
        velocity.y = -1;
    if (game->mInMgr.isKeyPressed(K_a))
        velocity.x = -1;
    else if (game->mInMgr.isKeyPressed(K_d))
        velocity.x = 1;

    velocity *= mSpeed;

    glm::vec2 pos = GetPos();
    pos += velocity * (float)ticks;
    CheckCollisions(game, ticks, pos);
    //SetPos(pos);

    // this is too texture dependent
    int pFace = F_DOWN,
        pState = P_WALK;
    static float frameX = 0;

    if (velocity.x < 0)
        pFace = F_LEFT;
    else if (velocity.x > 0)
        pFace = F_RIGHT;
    else if (velocity.y < 0)
        pFace = F_DOWN;
    else if (velocity.y > 0)
        pFace = F_UP;
    else
        pState = P_STOP;

    frameX = pState * ((frameX + mAnimSpeed * ticks));
    if (frameX > 5.0f)
        frameX = 0;

    SetFrame(floor(frameX) + 1, pFace);
    if (velocity.x || velocity.y)
        mVelocity = velocity;
}

bool Player::CheckCollisions(Game* game, uint ticks, glm::vec2& newPos)
{
    bool collided = false;
    const std::vector<Entity*> entities = game->GetEntities();
    for (uint i = 0; i < entities.size(); i++) {
        if (entities[i] == this)
            continue;
        if (GetAABB().Intersects(entities[i]->GetAABB())) {
            collided = true;
            switch (entities[i]->GetType()) {
            case BRICK:
                BrickCollision(newPos, entities[i]);
                break;
            case BALL:
                BallCollision(game, ticks, newPos, entities[i]);
                break;
            }
        }
        SetPos(newPos);
        if (GetAABB().Intersects(entities[i]->GetAABB())) {
            // printf("Player collision was not solved with %s\n",
            //     entities[i]->GetType() == BALL ? "ball" : "brick");
        }
    }

    return collided;
}

void Player::BrickCollision(glm::vec2& newPos, Entity* brick)
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

    assert(depth.x != 0.0f);
    assert(depth.y != 0.0f);
    // TODO - compute the other axis based on velocity vector and do diff also
    // for a more accurate point.
    if (fabs(depth.x) < fabs(depth.y))
        newPos.x -= depth.x;
    else
        newPos.y -= depth.y;
}

void Player::BallCollision(Game* game, uint ticks, glm::vec2& newPos, Entity* ball)
{
}