#pragma once

#include "vertex.h"
#include <GL/glew.h>
#include <vector>

enum DrawType {
    D_NONE,
    D_POINTS,
    D_LINES,
    D_TRIANGLES,
    D_MAX
};

struct Renderable {
    std::vector<Vertex> mVertices;
    std::vector<int> mIndexes;
    DrawType mDrawType;
    GLuint textureId;
    int mType;
    Renderable()
        : mDrawType(D_NONE)
        , textureId(0){};
};
