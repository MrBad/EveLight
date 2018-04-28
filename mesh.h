#pragma once

#include <vector>
#include "vertex.h"

using namespace std;

// TODO: construct from file mesh.
class Mesh
{
  public:
    Mesh();
    ~Mesh();
    void AddVertex(const Vertex& vertex);
    void AddIndex(const int index);
    void SetVertices(const vector<Vertex> &vertices);
    void SetIndexes(const vector<int> &indexes);
    void SetDrawType(GLenum drawType);
    void Draw();
  
  protected:
    bool mDirty;
    vector<Vertex> mVertices;
    vector<int> mIndexes;
    GLenum mDrawType;

  private:
    void Update();
    GLuint mVertexArray, mVertexBuffer, mIndexBuffer;
};
