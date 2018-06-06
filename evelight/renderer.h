#pragma once

#include "rectangle.h"
#include "vertex.h"

namespace evl {

struct RenderBatch {
    uint verticesOffs,
        numVertices; // offset in vertices buffer, number of vertices
    uint indexOffs, numIndexes; // offset in index buffer, number of indexes
    DrawType drawType; // draw type
    GLuint textureId; // textureId if applicable

    RenderBatch() { textureId = 0; }

    RenderBatch(uint verticesOffs, uint indexOffs, DrawType drawType, GLuint textureId)
        : verticesOffs(verticesOffs)
        , numVertices(0)
        , indexOffs(indexOffs)
        , numIndexes(0)
        , drawType(drawType)
        , textureId(textureId)
    {
    }
    bool operator==(const RenderBatch& other)
    {
        return this->drawType == other.drawType && this->textureId == other.textureId;
    }
};

class Renderer {
public:
    Renderer();
    ~Renderer();
    void Init();
    void Add(Renderable* renderable);
    bool Delete(Renderable* renderable);
    void Clear();
    void Draw();
    void BuildBatches();
    void RebuildVertices();

private:
    GLuint mVertexArray, mVertexBuffer, mIndexBuffer;
    std::vector<Vertex> mVertices;
    std::vector<int> mIndexes;
    std::vector<Renderable*> mQueue;
    std::vector<RenderBatch> mBatches;
    bool mDirty;

    RenderBatch* GetBatch(DrawType draqType, GLuint textureId, uint verticesOffs, uint indexesOffs);
    void PrintBatches();
};
}
