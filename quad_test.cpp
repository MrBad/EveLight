#include "game.h"
#include "quad_tree.h"
#include "renderer.h"

class DynRect : public FilledRectangle, public QTEntity {
public:
    DynRect(float x, float y, float width, float height, const Color& color)
        : FilledRectangle(x, y, width, height, color)
        , mVelocity(0.0f, 0.0f)
    {
    }
    ~DynRect() {}
    void SetVelocity(const glm::vec2& velocity) { mVelocity = velocity; }
    const glm::vec2& GetVelocity() const { return mVelocity; }
    // QuadTree interface implementation
    AABB GetAABB() { return Rectangle::GetAABB(); }

private:
    glm::vec2 mVelocity;
};

class QuadTest : public Game {
public:
    QuadTest(const string& title, int width, int height)
        : Game(title, width, height)
    {
    }
    ~QuadTest() {}
    bool onGameInit() override final;
    bool onGameUpdate(uint dt) override final;
    void onGameDelete() override final;

private:
    GLProgram mProgram;
    Renderer mRenderer;
    std::vector<DynRect*> mRects;
    std::vector<Rectangle*> mQuadRects;
    QuadTree mQTree;
};

bool QuadTest::onGameInit()
{
    mProgram.Create("res/shaders/simple");
    mCamera.SetPos(mWindow.getWidth() / 2, mWindow.getHeight() / 2);
    mRenderer.Init();
    mQTree.Init(AABB(0, 0, mWindow.getWidth(), mWindow.getHeight()), true);

    DynRect* rect;
    // rect = new DynRect(10, 10, 30, 30, Color(128, 255, 255, 200));
    // rect->SetVelocity(glm::vec2(0.1f, 0.1f));
    // mRects.push_back(rect);
    // rect->QTObj = mQTree.Add(rect->GetAABB(), rect);

    // rect = new DynRect(30, 30, 30, 30, Color(128, 255, 128, 200));
    // rect->SetVelocity(glm::vec2(0.1f, 0.0f));
    // mRects.push_back(rect);
    // rect->QTObj = mQTree.Add(rect->GetAABB(), rect);

    // rect = new DynRect(150, 120, 30, 30, Color(255, 128, 255, 200));
    // rect->SetVelocity(glm::vec2(0.1f, 0.1f));
    // mRects.push_back(rect);
    // rect->QTObj = mQTree.Add(rect->GetAABB(), rect);

    // rect = new DynRect(110, 120, 30, 30, Color(255, 255, 128, 200));
    // rect->SetVelocity(glm::vec2(0.05f, 0.1f));
    // mRects.push_back(rect);
    // rect->QTObj = mQTree.Add(rect->GetAABB(), rect);

    // rect = new DynRect(110, 80, 30, 30, Color(128, 128, 255, 200));
    // rect->SetVelocity(glm::vec2(0.05f, 0.05f));
    // mRects.push_back(rect);
    // rect->QTObj = mQTree.Add(rect->GetAABB(), rect);

    for (uint i = 0; i < 20000; i++) {
        float x = 1 + rand() % 100;
        float y = 1 + rand() % 100;
        Color color(rand() % 255, rand() % 255, rand() % 255, 200);
        rect = new DynRect(x, y, 2, 2, color);
        rect->SetVelocity(glm::vec2((float)rand() / RAND_MAX / 1, (float)rand() / RAND_MAX / 1));
        mRects.push_back(rect);
        mQTree.Add(rect);
    }

    return true;
}

bool QuadTest::onGameUpdate(uint dt)
{
    (void)dt;

    mRenderer.Clear();

    for (uint i = 0; i < mRects.size(); i++) {
        glm::vec2 pos = mRects[i]->GetPos();
        pos += (float)dt * mRects[i]->GetVelocity();
        // pos.x = pos.x + mRects[i]->GetWidth() >= mWindow.getWidth() ? 1 : pos.x;
        // pos.y = pos.y + mRects[i]->GetHeight() >= mWindow.getHeight() ? 1 : pos.y;
        mRects[i]->SetPos(pos);
        mQTree.Update(mRects[i]);
    }

    for (uint i = 0; i < mQuadRects.size(); i++)
        delete mQuadRects[i];

    mQuadRects.clear();

    uint numObjs = 0;
    uint maxDepth = 0;
    // Print QuadTree nodes boundaries
    auto f = [&](QTNode* qNode) {
        AABB limits = qNode->GetLimits();
        Rectangle* rect = new Rectangle(limits.minX, limits.minY,
            limits.maxX - limits.minX, limits.maxY - limits.minY,
            Color(128, 128, 128, 64));

        mQuadRects.push_back(rect);
        mRenderer.Add(rect);
        numObjs++;
        maxDepth = max(maxDepth, qNode->GetDepth());
        return true;
    };
    mQTree.Traverse(f);

    // Add rectangles to the renderer
    for (uint i = 0; i < mRects.size(); i++)
        mRenderer.Add(mRects[i]);

    printf("NumNodes: %3d, MaxDepth: %d, fps: %d\n", numObjs, maxDepth, mFPS);

    mCamera.SetMatrix(mProgram.getId(), "MVP");
    mRenderer.Draw();

    return true;
}

void QuadTest::onGameDelete()
{
    for (uint i = 0; i < mRects.size(); i++)
        delete mRects[i];
    for (uint i = 0; i < mQuadRects.size(); i++)
        delete mQuadRects[i];
}

int main()
{
    QuadTest QuadTest("Testing quad tree", 800, 600);
    QuadTest.Start();

    return 0;
}
