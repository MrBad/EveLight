#pragma once
#include "rectangle.h"
#include "texture.h"
#include "gl_program.h"

class Sprite : public FilledRectangle
{
    public:
    Sprite(float x, float y, float width, float height, Texture *t, GLProgram &p);
    ~Sprite();
    void Draw() override;
    private:
        Texture *mTexture;
        GLProgram *mProgram;
};