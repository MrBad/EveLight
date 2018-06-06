#include "player.h"
#include "ball.h"
#include "balls.h"
#include "evelight/game.h"

Player::Player(float x, float y, float width, float height, uint texId)
    : Entity(x, y, width, height)
    , mAnimSpeed(0.01f)
    , mSpeed(0.3f)
    , mSprite(width, height, texId)
{
    mType = PLAYER;
    mSprite.SetPos(mPos);
}

void Player::SetPos(const glm::vec2& pos)
{
    mPos = pos;
    mSprite.SetPos(pos);
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

    mSprite.SetFrame(floor(frameX) + 1, pFace);
    // update velocity if is not 0 (so we keep track of previous one)
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
            printf("Player collision not solved with %s\n",
                entities[i]->GetType() == BALL ? "ball" : "brick");
        }
    }

    return collided;
}

void Player::BrickCollision(glm::vec2& newPos, Entity* brick)
{
    AABB pAABB = GetAABB();
    AABB bAABB = brick->GetAABB();

    glm::vec2 depth = pAABB.GetIntersectionDepth(bAABB);

    // TODO - compute the other axis based on velocity vector and do diff also
    // for a more accurate point.
    if (fabs(depth.x) < fabs(depth.y))
        newPos.x -= depth.x;
    else
        newPos.y -= depth.y;
}

void Player::BallCollision(Game* game, uint ticks, glm::vec2& newPos, Entity* ball)
{
    glm::vec2 depth = GetAABB().GetIntersectionDepth(ball->GetAABB());

    glm::vec2 ballNewPos = ball->GetPos();

    if (fabs(depth.x) < fabs(depth.y)) {
        ballNewPos.x += depth.x;
    } else {
        ballNewPos.y += depth.y;
    }
    // check if pushed ball collides with something else
    ((Ball*)ball)->CheckCollisions(game, ticks, ballNewPos);

    if (GetAABB().Intersects(ball->GetAABB())) {
        // step back player
        depth = GetAABB().GetIntersectionDepth(ball->GetAABB());
        if (fabs(depth.x) < fabs(depth.y)) {
            newPos.x -= depth.x;
        } else {
            newPos.y -= depth.y;
        }
    }
}
