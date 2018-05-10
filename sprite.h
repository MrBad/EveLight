#pragma once

#include "filled_rectangle.h"
#include "gl_program.h"
#include "texture.h"

class Sprite : public FilledRectangle {
public:
    Sprite(float x, float y, float width, float height, const Color& color, uint texId);
    ~Sprite() {}
    void Draw();
};
