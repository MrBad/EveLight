#pragma once
#include <glm/glm.hpp>
#include "entity.h"

class Square
{
  public:
    Square();
    ~Square();
    void Init(float x, float y, float width, float height);
    void SetPos(float x, float y);
    void Draw();
  private:
    float mX, mY, mWidth, mHeight;
    bool mDirty;
    void Update();
    GLuint mVB0, mVAO;
};