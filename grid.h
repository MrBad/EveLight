#include "aabb.h"
#include "renderable.h"

class Grid : public Renderable {
public:
    Grid();
    Grid(float x, float y, float width, float height, int step, const Color& color);

private:
    float mX, mY, mWidth, mHeight;
    int mStep;
    Color mColor;

    void BuildVertices();
};
