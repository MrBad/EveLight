/**
 * A small test game
 */
#include "balls.h"
#include "aabb.h"
#include "ball.h"
#include "player.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <iostream>
#include <random>
#include <vector>

const int TSZ = 40;
const int BSZ = 32;
const int NUM_BALLS = 0;

void Balls::buildMap()
{

    std::string map = "####################\n"
                      "#                  #\n"
                      "#   ####    ###    #\n"
                      "#   #              #\n"
                      "#   ####           #\n"
                      "#   #      ######  #\n"
                      "#   ####           #\n"
                      "#                  #\n"
                      "#    #             #\n"
                      "#                  #\n"
                      "#                  #\n"
                      "#    ##            #\n"
                      "#            ###   #\n"
                      "#                  #\n"
                      "####################\n";

    uint x = 0, y = 0;
    Entity* brick;

    for (uint i = 0; i < map.size(); i++) {
        switch (map[i]) {
        case '#':
            brick = new Entity(x * TSZ, y * TSZ, TSZ, TSZ, mTexMgr.Get("brick")->getId());
            brick->SetType(BRICK);
            mRenderer.Add(brick);
            mEntities.push_back(brick);
            x++;
            break;
        case ' ':
            x++;
            break;
        case '\n':
            mMapX = max(mMapX, x);
            x = 0;
            y++;
            break;
        default:
            std::cout << "Unknown symbol: " << map[i] << " at " << x << ", " << y;
        }
    }
    mMapY = y;
    std::cout << mMapX << ", " << mMapY << std::endl;
}

bool Balls::onGameInit()
{
    mProgram.Create("res/shaders/simple");

    mTexMgr.Add("circle", "res/textures/circle.png");
    mTexMgr.Add("brick", "res/textures/brick.png");
    mTexMgr.Add("player", "res/textures/hero.png");

    mCamera.SetPos(mWindow.getWidth() / 2, mWindow.getHeight() / 2);

    mRenderer.Init();

    buildMap();

    std::mt19937 rng(time(NULL));
    std::uniform_int_distribution<int> gen(1, 255);
    std::normal_distribution<float> fgen(-1, 1);

    // Init balls
    for (int i = 0; i < NUM_BALLS; i++) {
        float radius = BSZ / 2 + gen(rng) % BSZ / 2;

        Ball* ball = new Ball(
            fgen(rng) + (1 + gen(rng) % (mMapX - 2)) * TSZ,
            fgen(rng) + (1 + gen(rng) % (mMapY - 2)) * TSZ,
            radius, radius, mTexMgr.Get("circle")->getId());
        ball->SetColor(Color(gen(rng), gen(rng), gen(rng), 200));
        ball->SetVelocity(glm::vec2(0.05f * fgen(rng), 0.05f * fgen(rng)));
        ball->SetType(BALL);
        mRenderer.Add(ball);
        mEntities.push_back(ball);
    }

    // Player
    mPlayer = new Player(100, 100, 32, 64, mTexMgr.Get("player")->getId());
    mPlayer->SetNumFrames(6, 4);
    mPlayer->SetType(PLAYER);
    mRenderer.Add(mPlayer);
    mEntities.push_back(mPlayer);

    return true;
}

void Balls::CameraUpdate(uint ticks)
{
    // float speed = 0.3f * ticks;
    float scaleSpeed = 1.0f + (float)ticks / 1000;

    if (mInMgr.isKeyPressed(K_q))
        mCamera.SetScale(mCamera.GetScale() * scaleSpeed);
    else if (mInMgr.isKeyPressed(K_e))
        mCamera.SetScale(mCamera.GetScale() / scaleSpeed);

    // Fallow the player
    mCamera.SetPos(mPlayer->GetX(), mPlayer->GetY());

    // Send camera matrix to opengl
    mCamera.SetMatrix(mProgram.getId(), "MVP");
}

void Balls::DynamicStaticCollision(Entity* eDynamic, Entity* eStatic)
{
    // Ball | User Brick collision
    AABB dynAABB = eDynamic->GetAABB();
    AABB statAABB = eStatic->GetAABB();
    glm::vec2 distance = dynAABB.GetDistance(statAABB);
    glm::vec2 depth(0);
    glm::vec2 vel = glm::normalize(eDynamic->GetVelocity());
    if (fabsf(distance.x) > fabsf(distance.y)) {
        if (distance.x < 0)// dynamic obj left collision
            depth.x = (statAABB.maxX - dynAABB.minX);
        else if (distance.x > 0) // right
            depth.x = -(dynAABB.maxX - statAABB.minX);
        if (eDynamic->GetType() == BALL)
            eDynamic->SetVelocity(eDynamic->GetVelocity() * glm::vec2(-1, 1));
    }
    else {
        if (distance.y < 0) // bottom
            depth.y = statAABB.maxY - dynAABB.minY;
        else if (distance.y > 0) // top
            depth.y = -(dynAABB.maxY - statAABB.minY);
        if (eDynamic->GetType() == BALL)
            eDynamic->SetVelocity(eDynamic->GetVelocity()* glm::vec2(1, -1));
    }
    float dot = depth.x * vel.x + depth.y * vel.y;
    float vecMag = sqrt(vel.x * vel.x + vel.y * vel.y);
    depth = (dot / vecMag) * vel;
    eDynamic->SetPos(eDynamic->GetPos() + depth);
}

void Balls::DynamicDynamicCollision(Entity* a, Entity* b)
{
    if (a->GetType() == BALL && b->GetType() == BALL) {
        float aRadius = a->GetWidth() / 2;
        float bRadius = b->GetWidth() / 2;
        float minDist = aRadius + bRadius;
        const glm::vec2 v2Dist(
            a->GetX() + aRadius - (b->GetX() + bRadius),
            a->GetY() + aRadius - (b->GetY() + bRadius));
        float dist = glm::length(v2Dist);
        if (minDist < dist)
            return;
        if (!dist) // XXX bad position, avoid 0
            return;

        float depth = minDist - dist;
        glm::vec2 v2ColDept = glm::normalize(v2Dist) * depth;
        v2ColDept *= 0.5;
        a->SetPos(a->GetX() + v2ColDept.x, a->GetY() + v2ColDept.y);
        b->SetPos(b->GetX() - v2ColDept.x, b->GetY() - v2ColDept.y);

        float arc = aRadius * aRadius * aRadius;
        float brc = bRadius * bRadius * bRadius;
        // v1New = (v1 * (vol1 - vol2) + 2 * vol2 * v2) / (vol1 + vol2) //
        glm::vec2 aVNew = a->GetVelocity() * (arc - brc);
        aVNew = aVNew + b->GetVelocity() * (2 * brc);
        aVNew = aVNew / (arc + brc);

        glm::vec2 bVNew = b->GetVelocity() * (brc - arc);
        bVNew = bVNew + a->GetVelocity() * (2 * arc);
        bVNew = bVNew / (arc + brc);

        a->SetVelocity(aVNew);
        b->SetVelocity(bVNew);
    }
}

bool Balls::onGameUpdate(uint32_t ticks)
{
    for (uint i = 0; i < mEntities.size(); i++) {
        mEntities[i]->Update(this, ticks);
    }
    for (uint i = 0; i < mEntities.size(); i++) {
        Entity* a = mEntities[i];
        for (uint j = i + 1; j < mEntities.size(); j++) {
            Entity* b = mEntities[j];
            if (a->isStatic() && b->isStatic())
                continue;
            AABB aAABB = a->GetAABB();
            AABB bAABB = b->GetAABB();
            if (!(aAABB.Intersects(bAABB)))
                continue;
            // We have a collision
            if (!a->isStatic() && b->isStatic())
                DynamicStaticCollision(a, b);
            else if (a->isStatic() && !b->isStatic())
                DynamicStaticCollision(b, a);
            else
                DynamicDynamicCollision(a, b);
        }
    }

    CameraUpdate(ticks);

    mRenderer.Draw();

    // cout << mFPS << endl;

    return true;
}

void Balls::onGameDelete()
{
    for (uint i = 0; i < mEntities.size(); i++)
        delete mEntities[i];
}
