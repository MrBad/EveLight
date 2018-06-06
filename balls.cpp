/**
 * A small test game
 */
#include "balls.h"
#include "evelight/aabb.h"
#include "ball.h"
#include "evelight/quad_tree.h"
#include "evelight/texture.h"
#include <glm/glm.hpp>
#include <iostream>
#include <random>
#include <vector>

const int TSZ = 128;
const int BSZ = 32;
const int NUM_BALLS = 200;

class Brick : public Entity {
public:
    Brick(float x, float y, float width, float height, uint texId)
        : Entity(x, y, width, height)
        , mSprite(width, height, texId)
    {
        mType = BRICK;
        mSprite.SetPos(mPos);
    }
    Renderable* GetRenderable() { return &mSprite; }

private:
    Sprite mSprite;
};

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
    mMapX = 0;
    Brick* brick;

    for (uint i = 0; i < map.size(); i++) {
        switch (map[i]) {
        case '#':
            brick = new Brick(x * TSZ, y * TSZ, TSZ, TSZ, mTexMgr.Get("brick")->getId());
            mRenderer.Add(brick->GetRenderable());
            mEntities.push_back(brick);
            x++;
            break;
        case ' ':
            x++;
            break;
        case '\n':
            mMapX = std::max(mMapX, x);
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
    mTexMgr.Add("font", "res/textures/font.png");

    mCamera.SetPos(mWindow.GetWidth() / 2, mWindow.GetHeight() / 2);

    mRenderer.Init();
    mTexter = Texter(&mCamera, &mRenderer, mTexMgr.Get("font"));
    mTexter.SetNumGlyphs(16, 16);

    buildMap();

    std::mt19937 rng(time(NULL));
    std::uniform_int_distribution<int> gen(1, 255);
    std::normal_distribution<float> fgen(-1, 1);

    // Init balls
    for (int i = 0; i < NUM_BALLS; i++) {
        float radius = BSZ / 2 + gen(rng) % BSZ / 2;

        Ball* ball = new Ball(
            fgen(rng) + (1 + gen(rng) % (mMapX - 2)) * TSZ + radius,
            fgen(rng) + (1 + gen(rng) % (mMapY - 2)) * TSZ + radius,
            radius, mTexMgr.Get("circle")->getId());

        ball->SetColor(Color(gen(rng), gen(rng), gen(rng), 200));
        ball->SetVelocity(glm::vec2(0.05f * fgen(rng), 0.05f * fgen(rng)));
        // ball->SetVelocity(glm::vec2(0.0001f * fgen(rng), 0.0001f * fgen(rng)));
        mRenderer.Add(ball->GetRenderable());
        mEntities.push_back((Entity*)ball);
    }

    // Player
    mPlayer = new Player(300, 300, 32, 64, mTexMgr.Get("player")->getId());
    mPlayer->SetSpriteNumFrames(6, 4);
    mRenderer.Add(mPlayer->GetRenderable());
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

    // Follow the player
    mCamera.SetPos(
        mPlayer->GetPos().x + mPlayer->GetWidth() / 2,
        mPlayer->GetPos().y + mPlayer->GetHeight() / 2);

    // Send camera matrix to opengl
    mCamera.SetMatrix(mProgram.getId(), "MVP");
}

bool Balls::onGameUpdate(uint32_t ticks)
{
    for (uint i = 0; i < mEntities.size(); i++)
        mEntities[i]->Update(this, ticks);

    CameraUpdate(ticks);

    mTexter.Clear();
    mTexter.SetColor(128, 200, 128, 255);
    mTexter.PrintAt(0, 0, "FPS: " + std::to_string(mFPS));

    mTexter.Flush();
    mRenderer.Draw();

    // cout << mFPS << endl;

    return true;
}

void Balls::onGameDelete()
{
    for (uint i = 0; i < mEntities.size(); i++)
        delete mEntities[i];
}
