#pragma once

#include <vector>
#include "vertex.h"
#include "gl_program.h"

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
    void SetProgramId(GLuint progrmId);
    virtual void Draw();
  
  protected:
    bool mDirty;
    vector<Vertex> mVertices;
    vector<int> mIndexes;
    GLenum mDrawType;
    GLuint mVertexArray, mVertexBuffer, mIndexBuffer;
    GLuint mProgramId;

  private:
    void Update();
    
};
