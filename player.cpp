#include "player.h"
#include "game.h"

Player::Player(float x, float y, float width, float height, uint texId)
    : Entity(x, y, width, height, texId),
    mAnimSpeed(0.01f),
    mSpeed(0.3f)
{
    SetStatic(false);
}

void Player::Update(Game *game, uint ticks)
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
    else if (game->mInMgr.isKeyPressed(K_a))
        velocity.x = -1;
    else if (game->mInMgr.isKeyPressed(K_d))
        velocity.x = 1;
    
    velocity *= mSpeed;

    glm::vec2 pos = GetPos();
    pos += velocity * (float)ticks;
    SetPos(pos);

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
