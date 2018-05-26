#include "texter.h"

Texter::Texter(Camera* camera, Renderer* renderer, Texture* texture)
    : mCamera(camera)
    , mRenderer(renderer)
    , mTexture(texture)
    , mNumXGlyphs(1)
    , mNumYGlyphs(1)
    , mFontSize(24)
    , mLetterSpacing(0.5f)
    , mColor(255, 255, 255, 200)
{
}

Texter::~Texter()
{
}

void Texter::SetNumGlyphs(uint numXGlyphs, uint numYGlyphs)
{
    mNumXGlyphs = numXGlyphs;
    mNumYGlyphs = numYGlyphs;
}

void Texter::Clear()
{
    // XXX Not the fastest. Better way is to use a separate renderer.
    for (uint i = 0; i < mGlyphs.size(); i++)
        mRenderer->Delete(&mGlyphs[i]);

    mGlyphs.clear();
}

void Texter::PrintAt(int x, int y, const std::string& text)
{
    AABB camAABB = mCamera->GetAABB();
    float uvX = 1.0f / mNumXGlyphs;
    float uvY = 1.0f / mNumYGlyphs;
    float camScale = mCamera->GetScale();
    float fontHeight = mFontSize / camScale;
    float fontWidth = fontHeight;

    for (uint i = 0; i < text.length(); i++) {
        float posX = x / camScale + camAABB.minX + fontWidth * mLetterSpacing * i;
        float posY = camAABB.maxY - y / camScale - fontHeight;
        Sprite glyph(posX, posY, fontWidth, fontHeight, mColor, mTexture->getId());
        uint idX = text[i] % mNumXGlyphs;
        uint idY = mNumYGlyphs - 1 - (text[i] / mNumXGlyphs);
        AABB uv = AABB(idX * uvX, idY * uvY, (idX + 1) * uvX, (idY + 1) * uvY);
        glyph.SetUV(uv);
        // add to sprite list
        mGlyphs.push_back(glyph);
    }
}

void Texter::Flush()
{
    // add glyphs to renderer
    for (uint i = 0; i < mGlyphs.size(); i++)
        mRenderer->Add(&mGlyphs[i]);
}

void Texter::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    mColor = { r, g, b, a };
}
