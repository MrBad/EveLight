#pragma once

#include <cstdint>
#include <glm/glm.hpp>

namespace evl {

struct Color {
    uint8_t r, g, b, a;
    Color()
        : r(255)
        , g(255)
        , b(255)
        , a(255)
    {
    }
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
        : r(red)
        , g(green)
        , b(blue)
        , a(alpha)
    {
    }
};

struct Vertex {
    glm::vec2 pos;
    Color color;
    glm::vec2 uv;

    void SetPos(float x, float y) { pos.x = x, pos.y = y; }
    void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { color.r = r, color.g = g, color.b = b, color.a = a; }
    void SetColor(Color& color) { this->color = color; }
    void SetUV(float x, float y) { uv.x = x, uv.y = y; }
};
}
