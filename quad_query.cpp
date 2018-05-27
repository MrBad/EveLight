
#include "filled_rectangle.h"
#include "game.h"
#include "gl_program.h"
#include "quad_tree.h"
#include "renderer.h"
#include <string>

class QTRect : public FilledRectangle, public QTEntity {
public:
    QTRect(float x, float y, float width, float height, const Color& color)
        : FilledRectangle(x, y, width, height, color)
    {
    }
    ~QTRect() {}

    AABB GetAABB() { return Rectangle::GetAABB(); }

    Color initialColor;
};

class QuadQuery : public Game {
public:
    QuadQuery(const std::string& title, int width, int height)
        : Game(title, width, height)
    {
    }
    bool onGameInit() override final;
    bool onGameUpdate(uint dt) override final;
    void onGameDelete() override final;

private:
    GLProgram mProgram;
    Renderer mRenderer;
    QuadTree mQTree;
    std::vector<QTRect*> mRects;
    QTRect* mQueryBox;

    void UpdateCamera(uint dt);
};

bool QuadQuery::onGameInit()
{
    mProgram.Create("res/shaders/simple");
    mRenderer.Init();
    mCamera.SetPos(mWindow.GetWidth() / 2, mWindow.GetHeight() / 2);
    mQTree.Init(AABB(0, 0, mWindow.GetWidth(), mWindow.GetHeight()), false);

    for (uint i = 0; i < 1000; i++) {
        float x = 1 + rand() % (mWindow.GetWidth() - 20);
        float y = 1 + rand() % (mWindow.GetHeight() - 20);
        Color color(rand() % 255, rand() % 255, rand() % 255, 100);
        QTRect* rect = new QTRect(x, y, 10, 10, color);
        rect->initialColor = color;
        mRects.push_back(rect);
        mQTree.Add(rect);
    }

    Color color(0, 200, 0, 100);
    float width = 200,
          height = 200;
    mQueryBox = new QTRect(
        (mWindow.GetWidth() - width) / 2,
        (mWindow.GetHeight() - height) / 2,
        width, height, color);
    mQueryBox->initialColor = color;
    mRects.push_back(mQueryBox);
    // mQueryBox->QTObj = mQTree.Add(mQueryBox->GetAABB(), mQueryBox);

    for (uint i = 0; i < mRects.size(); i++)
        mRenderer.Add(mRects[i]);

    return true;
}

void QuadQuery::onGameDelete()
{
    for (uint i = 0; i < mRects.size(); i++)
        delete mRects[i];
}

void QuadQuery::UpdateCamera(uint dt)
{
    float scaleSpeed = 1.0f + (float)dt / 1000;

    if (mInMgr.isKeyPressed(K_q))
        mCamera.SetScale(mCamera.GetScale() * scaleSpeed);
    else if (mInMgr.isKeyPressed(K_e))
        mCamera.SetScale(mCamera.GetScale() / scaleSpeed);
}

bool QuadQuery::onGameUpdate(uint dt)
{
    (void)dt;
    // mRenderer.Clear();

    UpdateCamera(dt);
    // Restore original colors
    for (uint i = 0; i < mRects.size(); i++)
        mRects[i]->SetColor(mRects[i]->initialColor);

    // Update query box position.
    float qBoxSpeed = 0.5f;
    glm::vec2 dir(0, 0);
    if (mInMgr.isKeyPressed(K_w))
        dir.y = 1;
    else if (mInMgr.isKeyPressed(K_s))
        dir.y = -1;
    if (mInMgr.isKeyPressed(K_a))
        dir.x = -1;
    else if (mInMgr.isKeyPressed(K_d))
        dir.x = 1;
    glm::vec2 pos = mQueryBox->GetPos();
    pos += dt * qBoxSpeed * dir;
    pos.x = pos.x + mQueryBox->GetWidth() > mWindow.GetWidth()
        ? mWindow.GetWidth() - mQueryBox->GetWidth()
        : pos.x < 0 ? 0 : pos.x;
    pos.y = pos.y + mQueryBox->GetHeight() > mWindow.GetHeight()
        ? mWindow.GetHeight() - mQueryBox->GetHeight()
        : pos.y < 0 ? 0 : pos.y;
    mQueryBox->SetPos(pos);

    std::vector<QTEntity*> results;
    int res = mQTree.GetIntersections(mQueryBox->GetAABB(), results);
    for (int i = 0; i < res; i++) {
        QTRect* rect = (QTRect*)results[i];
        rect->SetColor(Color(255, 0, 0, 255));
    }
    mCamera.SetMatrix(mProgram.getId(), "MVP");
    mRenderer.Draw();

    printf("fps: %d, %3d intersections\n", mFPS, res);

    return true;
}

int main()
{
    QuadQuery QuadQuery("Query the quad tree", 800, 600);
    QuadQuery.Start();

    return 0;
}