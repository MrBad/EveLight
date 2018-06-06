#pragma once

#include "rectangle.h"

namespace evl {

class FilledRectangle : public Rectangle {
public:
    FilledRectangle(float x, float y, float width, float height, const Color& color);
};
}
