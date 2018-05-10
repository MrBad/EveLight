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

const int TSZ = 64;

void Balls::buildMap()
{

    std::string map = "####################\n"
                      "#                  #\n"
                      "#                  #\n"
                      "#                  #\n"
                      "#                  #\n"
                      "#                  #\n"
                      "#                  #\n"
                      "#                  #\n"
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
            std::cout << "unknown symbol: " << map[i] << " at " << x << ", " << y;
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
    std::uniform_int_distribution<int> fgen(-1000, 1000);

    for (int i = 0; i < 8000; i++) {
        const Color color(gen(rng), gen(rng), gen(rng), 200);
        Ball* ball = new Ball(
            (1 + gen(rng) % (mMapX - 2)) * TSZ, (1 + gen(rng) % (mMapY - 2)) * TSZ,
            32, 32, color, mTexMgr.Get("circle")->getId());
        ball->SetVelocity(glm::vec2(0.0001f * fgen(rng), 0.0001f * fgen(rng)));
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
            Ball* ball = mBalls[i];
            Sprite* brick = mBricks[j];
            AABB ballAABB = ball->GetAABB();
            AABB brickAABB = brick->GetAABB();
            if (!(ballAABB.Intersects(brickAABB)))
                continue;
            glm::vec2 v2Distance = ballAABB.GetDistance(brickAABB);
            // std::cout << "distance: " << v2Distance.x << ", " << v2Distance.y << std::endl;
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
