#pragma once

#include "camera.h"
#include "renderer.h"
#include "sprite.h"
#include "texture.h"
#include "vertex.h"

class Texter {
public:
    Texter() {}
    Texter(Camera* camera, Renderer* renderer, Texture* texture);
    ~Texter();
    void SetNumGlyphs(uint numXGlyphs, uint numYGlyphs);
    void SetColor(const Color& color) { mColor = color; }
    void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void SetFontSize(uint fontSize) { mFontSize = fontSize; }
    void SetLetterSpacing(float letterSpacing) { mLetterSpacing = letterSpacing; }

    void Clear();
    void PrintAt(int x, int y, const std::string& text);
    void Flush();

private:
    Camera* mCamera;
    Renderer* mRenderer;
    Texture* mTexture;
    uint mNumXGlyphs, mNumYGlyphs;
    uint mFontSize;
    float mLetterSpacing;
    Color mColor;

    std::vector<Sprite> mGlyphs;
};
