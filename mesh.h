#pragma once

#include "gl_program.h"
#include "vertex.h"
#include <vector>

// TODO: construct from file mesh.
class Mesh {
public:
    Mesh();
    ~Mesh();
    void AddVertex(const Vertex& vertex);
    void AddIndex(const int index);
    void SetVertices(const std::vector<Vertex>& vertices);
    void SetIndexes(const std::vector<int>& indexes);
    void SetDrawType(GLenum drawType);
    void SetProgramId(GLuint progrmId);
    virtual void Draw();

protected:
    bool mDirty;
    std::vector<Vertex> mVertices;
    std::vector<int> mIndexes;
    GLenum mDrawType;
    GLuint mVertexArray, mVertexBuffer, mIndexBuffer;
    GLuint mProgramId;

private:
    void Update();
};
