#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "mesh.h"

class Rectangle : public Mesh
{
  public:
    Rectangle() {}
    Rectangle(float x, float y, float width, float height);
    void SetPos(float x, float y);
    void SetColor(const Color &color);
    void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

  protected:
    enum { LB, LT, RT, RB };
    float mX, mY, mWidth, mHeight;
    Color mColor;
    void Update();
};


class FilledRectangle : public Rectangle
{
  public:
    FilledRectangle(float x, float y, float width, float height);
};
