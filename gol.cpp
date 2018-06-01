/**
 * Simple Game of Life demo
 * DOCS: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
 */
#include "filled_rectangle.h"
#include "game.h"
#include "gl_program.h"
#include "grid.h"
#include "renderer.h"
#include <iostream>

class GOL : public Game {
public:
    GOL(const std::string& title, int width, int height)
        : Game(title, width, height)
    {
    }
    bool onGameInit();
    bool onGameUpdate(uint diffTicks);
    void onGameDelete();
    int NumNeighbours(const std::vector<char>& prevCells, int x, int y);

private:
    GLProgram mProgram;
    Renderer mRenderer;

    const int CELL_SIZE = 8;
    int mCellsX, mCellsY;
    std::vector<char> mCells;
    std::vector<FilledRectangle> mRects;
    Grid mGrid;
    uint mTimer;
};

bool GOL::onGameInit()
{
    mCellsX = mWindow.GetWidth() / CELL_SIZE;
    mCellsY = mWindow.GetHeight() / CELL_SIZE;
    mCells.resize(mCellsX * mCellsY);
    memset(&mCells[0], 0, sizeof(mCells[0]) * mCells.size());

    for (int i = 1; i < mCellsX - 1; i++) {
        for (int j = 1; j < mCellsY - 1; j++) {
            if (rand() % 4 == 0)
                mCells[i + j * mCellsX] = 1;
        }
    }

    mGrid = Grid(0, 0, mWindow.GetWidth(), mWindow.GetHeight(),
        CELL_SIZE,
        Color(128, 160, 128, 80));

    mProgram.Create("res/shaders/simple");
    mRenderer.Init();
    mCamera.SetPos(mWindow.GetWidth() / 2, mWindow.GetHeight() / 2);

    return true;
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

bool GOL::onGameUpdate(uint dt)
{
    mTimer += dt;

    std::vector<char> prevCells = mCells;

    if (mTimer > 200) {
        mTimer = 0;
        // TODO(vio): Right now this is run by a single CPU core.
        //   Write a test that use a really big map, with camera that can 
        //     zoom in and move, and use multiple threads / CPU cores
        //     to compute the new value of the cell in parallel.
        //   Idea is to use in the future multiple threads/workers (which can 
        //     be on a different server), that are doing the work of processing
        //     things in parallel.
        for (int x = 1; x < mCellsX - 1; x++) {
            for (int y = 1; y < mCellsY - 1; y++) {

                int neighbours = NumNeighbours(prevCells, x, y);

                if (prevCells[x + mCellsX * y] > 0) {
                    if (neighbours < 2 || neighbours > 3)
                        mCells[x + mCellsX * y] = 0;
                } else {
                    if (neighbours == 3)
                        mCells[x + mCellsX * y] = 1;
                }
            }
        }

        mRenderer.Clear();
        mRects.clear();

        for (int x = 0; x < mCellsX; x++) {
            for (int y = 0; y < mCellsY; y++) {
                if (mCells[x + mCellsX * y]) {
                    FilledRectangle rect(
                        x * CELL_SIZE, y * CELL_SIZE,
                        CELL_SIZE, CELL_SIZE,
                        Color(128, 160, 128, 255));
                    mRects.push_back(rect);
                }
            }
        }

        for (uint i = 0; i < mRects.size(); i++)
            mRenderer.Add(&mRects[i]);

        mRenderer.Add(&mGrid);

        printf("fps: %d\n", mFPS);
    }

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
