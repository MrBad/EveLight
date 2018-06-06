/**
 * Simple Game of Life demo
 * DOCS: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
 * TODO(vio): use this structure to split computing on several network nodes.
 */
#include "evelight/filled_rectangle.h"
#include "evelight/game.h"
#include "evelight/gl_program.h"
#include "evelight/grid.h"
#include "evelight/renderer.h"
#include <iostream>
#include <thread>

using namespace evl;

#define NUM_WORKERS 4

class GOL : public Game {
public:
    GOL(const std::string& title, int width, int height)
        : Game(title, width, height)
    {
    }
    bool onGameInit();
    bool onGameUpdate(uint diffTicks);
    void onGameDelete();

    void ProcessBatch(uint offset, uint numCells);

private:
    GLProgram mProgram;
    Renderer mRenderer;

    const int CELL_SIZE = 8;
    const int SCRSZ = 16;

    int mCellsX, mCellsY;
    std::vector<char> mCells;
    std::vector<char> mPrevCells;
    std::vector<FilledRectangle> mRects;
    Grid mGrid;
    uint mTimer;
    std::thread mWorkers[NUM_WORKERS];

    int NumNeighbours(const std::vector<char>& prevCells, int x, int y);
    void UpdateCamera(uint dt);
};

bool GOL::onGameInit()
{
    mCellsX = mWindow.GetWidth() * SCRSZ / CELL_SIZE;
    mCellsY = mWindow.GetHeight() * SCRSZ / CELL_SIZE;
    std::cout << "numXCells: " << mCellsX << ", numYCells: "
              << mCellsY << std::endl;

    mCells.resize(mCellsX * mCellsY);
    mPrevCells = mCells;
    std::cout << "Buf Size: " << mCells.size() << std::endl;
    memset(&mCells[0], 0, sizeof(mCells[0]) * mCells.size());

    for (int i = 1; i < mCellsX - 1; i++) {
        for (int j = 1; j < mCellsY - 1; j++) {
            if (rand() % 4 == 0)
                mCells[i + j * mCellsX] = 1;
        }
    }

    mGrid = Grid(0, 0, mWindow.GetWidth() * SCRSZ, mWindow.GetHeight() * SCRSZ,
        CELL_SIZE,
        Color(128, 160, 128, 80));

    mProgram.Create("res/shaders/simple");
    mRenderer.Init();
    mCamera.SetPos(mWindow.GetWidth() / 2, mWindow.GetHeight() / 2);

    return true;
}

void GOL::UpdateCamera(uint dt)
{
    float scaleSpeed = 1.0f + (float)dt / 1000;

    if (mInMgr.isKeyPressed(K_q))
        mCamera.SetScale(mCamera.GetScale() * scaleSpeed);
    else if (mInMgr.isKeyPressed(K_e))
        mCamera.SetScale(mCamera.GetScale() / scaleSpeed);

    glm::vec2 velocity(0);
    if (mInMgr.isKeyPressed(K_w))
        velocity.y = 1;
    else if (mInMgr.isKeyPressed(K_s))
        velocity.y = -1;
    if (mInMgr.isKeyPressed(K_a))
        velocity.x = -1;
    else if (mInMgr.isKeyPressed(K_d))
        velocity.x = 1;

    velocity *= dt * 0.5;
    glm::vec2 pos = mCamera.GetPos() + velocity;

    mCamera.SetPos(pos);
}

int GOL::NumNeighbours(const std::vector<char>& prevCells, int x, int y)
{
    int sum = 0;

    assert(x > 0);
    assert(y > 0);
    assert((uint)(x + mCellsX * y) < prevCells.size());

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i == 0 && j == 0)
                continue;
            int idx = (x + j) + mCellsX * (y + i);
            sum += prevCells[idx];
        }
    }

    return sum;
}

void GOL::ProcessBatch(uint offset, uint numCells)
{
    uint end = offset + numCells;
    for (uint i = offset; i < end; i++) {
        uint x = i % mCellsX;
        if (x == 0 || x == (uint)mCellsX - 1)
            continue;
        uint y = i / mCellsX;
        if (y == 0 || y == (uint)mCellsY - 1)
            continue;

        int neighbours = NumNeighbours(mPrevCells, x, y);

        if (mPrevCells[x + mCellsX * y] > 0) {
            if (neighbours < 2 || neighbours > 3)
                mCells[x + mCellsX * y] = 0;
        } else {
            if (neighbours == 3)
                mCells[x + mCellsX * y] = 1;
        }
    }
}

bool GOL::onGameUpdate(uint dt)
{
    mTimer += dt;

    if (true || mTimer > 20) {
        mPrevCells = mCells;
        mTimer = 0;
        // Using multiple threads / CPU cores
        //     to compute the new value of the cell, in parallel.
        //   Idea is to use in the future multiple workers (which can
        //     be on a different server), that are doing the work of processing
        //     things in parallel.
        uint numCells = mCells.size() / NUM_WORKERS;
        for (int i = 0; i < NUM_WORKERS; i++) {
            mWorkers[i] = std::thread(&GOL::ProcessBatch, this, i * numCells, numCells);
        }
        for (int i = 0; i < NUM_WORKERS; i++) {
            mWorkers[i].join();
        }

        mRenderer.Clear();
        mRects.clear();

        // clamp camera and do camera culling
        AABB camAABB = mCamera.GetAABB();
        uint minX = camAABB.minX < 0 ? 0 : camAABB.minX;
        uint minY = camAABB.minY < 0 ? 0 : camAABB.minY;
        uint maxX = camAABB.maxX > mWindow.GetWidth() * SCRSZ
            ? mWindow.GetWidth() * SCRSZ
            : camAABB.maxX;
        uint maxY = camAABB.maxY > mWindow.GetHeight() * SCRSZ
            ? mWindow.GetHeight() * SCRSZ
            : camAABB.maxY;

        for (uint x = minX / CELL_SIZE; x < maxX / CELL_SIZE; x++) {
            for (uint y = minY / CELL_SIZE; y < maxY / CELL_SIZE; y++) {
                if (mCells[x + mCellsX * y]) {
                    mRects.emplace_back(
                        x * CELL_SIZE, y * CELL_SIZE,
                        CELL_SIZE, CELL_SIZE,
                        Color(128, 160, 128, 255));
                }
            }
        }

        for (uint i = 0; i < mRects.size(); i++)
            mRenderer.Add(&mRects[i]);

        mRenderer.Add(&mGrid);
    }

    std::cout << "fps: " << mFPS << std::endl;

    UpdateCamera(dt);
    mCamera.SetMatrix(mProgram.getId(), "MVP");
    mRenderer.Draw();

    return true;
}

void GOL::onGameDelete() {}

int main()
{
    GOL game("Game of Life", 800, 600);
    game.Start();

    return 0;
}
