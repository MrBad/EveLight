#pragma once

#include <iostream>
#include <vector>
#include "vertex.h"

enum DrawType { D_NONE, D_POINTS, D_LINES, D_TRIANGLES, D_MAX };

struct Renderable
{
    std::vector<Vertex> mVertices;
    std::vector<int> mIndexes;
    DrawType mDrawType;
    GLuint textureId;
    Renderable() : textureId(0){};
};
