/**
 * A small test game
 */
#include "balls.h"
#include "aabb.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <iostream>
#include <random>
#include <vector>

const int TSZ = 40;
const int BSZ = 32;
const int NUM_BALLS = 50;

void Balls::buildMap()
{

    std::string map = "####################\n"
                      "#                  #\n"
                      "#   ####           #\n"
                      "#   #              #\n"
                      "#   ####           #\n"
                      "#   #      ###     #\n"
                      "#   ####           #\n"
                      "#                  #\n"
                      "#                  #\n"
                      "#                  #\n"
                      "#                  #\n"
                      "#    ##            #\n"
                      "#            ###   #\n"
                      "#                  #\n"
                      "####################\n";

    uint x = 0, y = 0;
    const Color white(255, 255, 255, 255);
    Sprite* sprite;

    for (uint i = 0; i < map.size(); i++) {
        switch (map[i]) {
        case '#':
            sprite = new Sprite(
                x * TSZ, y * TSZ, TSZ, TSZ, white,
                mTexMgr.Get("brick")->getId());
            mRenderer.Add(sprite);
            mBricks.push_back(sprite);
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

    mCamera.SetPos(mWindow.getWidth() / 2, mWindow.getHeight() / 2);

    mRenderer.Init();
    buildMap();

    std::mt19937 rng(time(NULL));
    std::uniform_int_distribution<int> gen(1, 255);
    std::normal_distribution<float> fgen(-1, 1);

    // Add balls to map //
    for (int i = 0; i < NUM_BALLS; i++) {
        const Color color(gen(rng), gen(rng), gen(rng), 200);
        float radius = BSZ / 2 + gen(rng) % BSZ / 2;

        Ball* ball = new Ball(
            fgen(rng) + (1 + gen(rng) % (mMapX - 2)) * TSZ,
            fgen(rng) + (1 + gen(rng) % (mMapY - 2)) * TSZ,
            radius, radius, color, mTexMgr.Get("circle")->getId());

        ball->SetVelocity(glm::vec2(0.05f * fgen(rng), 0.05f * fgen(rng)));
        mRenderer.Add(ball);
        mBalls.push_back(ball);
    }

    return true;
}

void Balls::updateCamera(uint ticks)
{
    float speed = 0.3f * ticks;
    float scaleSpeed = 1.0f + (float)ticks / 1000;

    if (mInMgr.isKeyPressed(K_q))
        mCamera.SetScale(mCamera.GetScale() * scaleSpeed);
    else if (mInMgr.isKeyPressed(K_e))
        mCamera.SetScale(mCamera.GetScale() / scaleSpeed);
    if (mInMgr.isKeyPressed(K_w))
        mCamera.SetPos(mCamera.GetX(), mCamera.GetY() + speed);
    else if (mInMgr.isKeyPressed(K_s))
        mCamera.SetPos(mCamera.GetX(), mCamera.GetY() - speed);
    if (mInMgr.isKeyPressed(K_a))
        mCamera.SetPos(mCamera.GetX() - speed, mCamera.GetY());
    else if (mInMgr.isKeyPressed(K_d))
        mCamera.SetPos(mCamera.GetX() + speed, mCamera.GetY());
    // send camera matrix to opengl //
    mCamera.SetMatrix(mProgram.getId(), "MVP");
}

bool Balls::onGameUpdate(uint32_t ticks)
{
    updateCamera(ticks);
    for (uint i = 0; i < mBalls.size(); i++) {
        mBalls[i]->Update(ticks);
        for (uint j = 0; j < mBricks.size(); j++) {
            // Ball Brick collision
            Ball* ball = mBalls[i];
            Sprite* brick = mBricks[j];
            AABB ballAABB = ball->GetAABB();
            AABB brickAABB = brick->GetAABB();
            if (!(ballAABB.Intersects(brickAABB)))
                continue;

            glm::vec2 v2Distance = ballAABB.GetDistance(brickAABB);
            float newX = ball->GetX(), newY = ball->GetY();
            if (fabsf(v2Distance.x) > fabsf(v2Distance.y)) {
                if (v2Distance.x < 0) {
                    // std::cout << "left coll" << std::endl;
                    newX = brick->GetX() + brick->GetWidth();
                } else if (v2Distance.x > 0) {
                    // std::cout << "right coll" << std::endl;
                    newX = brick->GetX() - ball->GetWidth();
                }
                ball->SetVelocity(ball->GetVelocity() * glm::vec2(-1, 1));
            } else {
                if (v2Distance.y < 0) {
                    // std::cout << "bottom coll" << std::endl;
                    newY = brick->GetY() + brick->GetHeight();
                } else if (v2Distance.y > 0) {
                    // std::cout << "top coll" << std::endl;
                    newY = brick->GetY() - ball->GetHeight();
                }
                ball->SetVelocity(ball->GetVelocity() * glm::vec2(1, -1));
            }
            ball->SetPos(newX, newY);
            assert(newX > 0 && newX < mMapX * TSZ);
            assert(newY > 0 && newY < mMapY * TSZ);
        }
        // Ball - Ball collision
        for (uint j = i + 1; j < mBalls.size(); j++) {
            Ball* a = mBalls[i];
            Ball* b = mBalls[j];
            float minDist = a->GetRadius() + b->GetRadius();
            const glm::vec2 v2Dist(
                a->GetX() + a->GetRadius() - (b->GetX() + b->GetRadius()),
                a->GetY() + a->GetRadius() - (b->GetY() + b->GetRadius()));
            float dist = glm::length(v2Dist);
            if (minDist < dist)
                continue;
            if (!dist) // XXX bad position, avoid 0
                continue;

            float depth = minDist - dist;
            glm::vec2 v2ColDept = glm::normalize(v2Dist) * depth;
            v2ColDept *= 0.5;
            a->SetPos(a->GetX() + v2ColDept.x, a->GetY() + v2ColDept.y);
            b->SetPos(b->GetX() - v2ColDept.x, b->GetY() - v2ColDept.y);

            float arc = a->GetRadius() * a->GetRadius() * a->GetRadius();
            float brc = b->GetRadius() * b->GetRadius() * b->GetRadius();
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

    mRenderer.Draw();

    cout << mFPS << endl;

    return true;
}

void Balls::onGameDelete()
{
    for (uint i = 0; i < mBricks.size(); i++)
        delete mBricks[i];
    for (uint i = 0; i < mBalls.size(); i++)
        delete mBalls[i];
}
