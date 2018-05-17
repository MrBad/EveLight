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

const int TSZ = 32;
const int BSZ = 32;
const int NUM_BALLS = 20;

void Balls::buildMap()
{

    std::string map = "####################\n"
                      "#                  #\n"
                      "#                  #\n"
                      "#   ####    ###### #\n"
                      "#   #            # #\n"
                      "#   ####         # #\n"
                      "#   #      ####### #\n"
                      "#   ####           #\n"
                      "#                  #\n"
                      "#    #         #####\n"
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
        // ball->SetVelocity(glm::vec2(0.0001f * fgen(rng), 0.0001f * fgen(rng)));
        ball->SetType(BALL);
        mRenderer.Add(ball);
        mEntities.push_back(ball);
    }

    // Player
    mPlayer = new Player(300, 300, 32, 64, mTexMgr.Get("player")->getId());
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
    mCamera.SetPos(
        mPlayer->GetX() + mPlayer->GetWidth() / 2,
        mPlayer->GetY() + mPlayer->GetHeight() / 2);

    // Send camera matrix to opengl
    mCamera.SetMatrix(mProgram.getId(), "MVP");
}

bool Balls::onGameUpdate(uint32_t ticks)
{
    for (uint i = 0; i < mEntities.size(); i++)
        mEntities[i]->Update(this, ticks);

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
