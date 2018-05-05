#pragma once

#include <glm/glm.hpp>
#include <iostream>

struct Color
{
    uint8_t r, g, b, a;
    Color() { r = 255, g = 0, b = 0, a = 255; };
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : r(red), g(green), b(blue), a(alpha) {}
};

struct Vertex
{
    glm::vec2 pos;
    Color color;
    glm::vec2 uv;

    void SetPos(float x, float y) { pos.x = x, pos.y = y; }
    void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { color.r = r, color.g = g, color.b = b, color.a = a; }
    void SetColor(Color &color) { this->color = color; }
    void SetUV(float x, float y) { uv.x = x, uv.y = y; }
    // Vertex(){}
    // Vertex(const Vertex &other) {
    //     pos = other.pos;
    //     color = other.color;
    //     uv = other.uv;
    //     std::cout << "Vertex copy" << std::endl;
    // }
};
