#include "renderer.h"
#include <algorithm>
#include <iostream>

/**
 * TODO: when adding new Renderable to the queue, generate/find it's appropiate batch in the same time
 *  and get loose of dirty flag. Also, on delete, remove it from vertices and batch, shrinking indexes buffer and vertices.
 * When adding to a batch, resize and buffer the data to GPU (new size). On vertices upload, on each frame
 * use glMapBuffer.
 */

Renderer::Renderer()
    : mVertexArray(0)
    , mVertexBuffer(0)
    , mIndexBuffer(0)
    , mDirty(true)
{
}

void Renderer::Init()
{
    enum shader_attrs {
        POSITION,
        COLOR,
        UV,
        SHADER_NUM_ATTRS
    };

    glGenVertexArrays(1, &mVertexArray);
    glGenBuffers(1, &mVertexBuffer);
    glGenBuffers(1, &mIndexBuffer);

    /// Generate buffers and their layout
    glBindVertexArray(mVertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

    glEnableVertexAttribArray(POSITION);
    glEnableVertexAttribArray(COLOR);
    glEnableVertexAttribArray(UV);

    glVertexAttribPointer(
        POSITION, sizeof(mVertices[0].pos) / sizeof(float), GL_FLOAT, GL_FALSE,
        sizeof(mVertices[0]), (void*)offsetof(Vertex, pos));

    glVertexAttribPointer(
        COLOR, sizeof(mVertices[0].color) / sizeof(uint8_t), GL_UNSIGNED_BYTE, GL_TRUE,
        sizeof(mVertices[0]), (void*)offsetof(Vertex, color));

    glVertexAttribPointer(
        UV, sizeof(mVertices[0].uv) / sizeof(float), GL_FLOAT, GL_FALSE,
        sizeof(mVertices[0]), (void*)offsetof(Vertex, uv));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &mVertexArray);
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mIndexBuffer);
}

void Renderer::Add(Renderable* renderable)
{
    mQueue.push_back(renderable);
    mDirty = true;
}

bool Renderer::Delete(Renderable* renderable)
{
    auto it = std::remove(mQueue.begin(), mQueue.end(), renderable);
    if (it != mQueue.end()) {
        mQueue.erase(it);
        mDirty = true;
        return true;
    }
    return false;
}

/**
 * Gets a batch having drawType and textureId.
 * If it cannot find one, will create one with given vertices indexes offsets
 */
RenderBatch* Renderer::GetBatch(
    DrawType drawType, GLuint textureId, uint verticesOffs, uint indexesOffs)
{
    RenderBatch batch(verticesOffs, indexesOffs, drawType, textureId);
    auto it = std::find(mBatches.begin(), mBatches.end(), batch);

    if (it == mBatches.end()) {
        // std::cout << "new batch" << std::endl;
        RenderBatch batch(verticesOffs, indexesOffs, drawType, textureId);
        mBatches.push_back(batch);
        return &mBatches.back();
    }

    return &(*it);
}

void Renderer::Clear()
{
    mVertices.clear();
    mIndexes.clear();
    mBatches.clear();
    mQueue.clear();
}

/**
 * Builds batches, so we can have only one vertex buffer and one index buffer, 
 * but keeping different offsets into them
 * This way the buffering to GPU will be done in one chunk (actually 2) and draw
 * calls will be minimized.
 * TODO - clean code, I don't think I need vertex offset, nor vertex size
 *      - use glMapBuffer and write vertices and indexes directly on a GPU mapped memory.
 */
void Renderer::BuildBatches()
{
    mVertices.clear();
    mIndexes.clear();
    mBatches.clear();

    DrawType lastDrawType = D_NONE;
    GLuint lastTextureId = 0;
    uint verticesOffs = 0, indexesOffs = 0;
    RenderBatch* batch = nullptr;

    for (uint i = 0; i < mQueue.size(); i++) {
        if (lastDrawType != mQueue[i]->mDrawType || lastTextureId != mQueue[i]->textureId) {
            lastDrawType = mQueue[i]->mDrawType;
            lastTextureId = mQueue[i]->textureId;
            batch = GetBatch(lastDrawType, lastTextureId, verticesOffs, indexesOffs);
        }
        uint indexGOffs = mVertices.size();

        mVertices.insert(mVertices.end(), mQueue[i]->mVertices.begin(), mQueue[i]->mVertices.end());
        verticesOffs += mQueue[i]->mVertices.size();
        batch->numVertices += mQueue[i]->mVertices.size();

        for (uint j = 0; j < mQueue[i]->mIndexes.size(); j++)
            mIndexes.emplace_back(indexGOffs + mQueue[i]->mIndexes[j]);
        indexesOffs += mQueue[i]->mIndexes.size();
        batch->numIndexes += mQueue[i]->mIndexes.size();
    }
    mDirty = false;
}

/**
 * Vertices needs to be rebuilt each frame, because the world is dynamic.
 */
void Renderer::RebuildVertices()
{
    mVertices.clear();
    for (uint i = 0; i < mQueue.size(); i++)
        for (uint j = 0; j < mQueue[i]->mVertices.size(); j++)
            mVertices.emplace_back(mQueue[i]->mVertices[j]);
}

/**
 * Draw the batches
 */
void Renderer::Draw()
{

    if (mDirty)
        BuildBatches();
    else
        RebuildVertices();

    glBindVertexArray(mVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(mVertices[0]), &mVertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexes.size() * sizeof(mIndexes[0]), &mIndexes[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    for (uint i = 0; i < mBatches.size(); i++) {
        GLuint drawType;
        switch (mBatches[i].drawType) {
        case D_POINTS:
            drawType = GL_POINTS;
            break;
        case D_LINES:
            drawType = GL_LINES;
            break;
        case D_TRIANGLES:
            drawType = GL_TRIANGLES;
            break;
        default:
            return;
        }
        glBindTexture(GL_TEXTURE_2D, mBatches[i].textureId);
        glDrawElements(drawType, mBatches[i].numIndexes, GL_UNSIGNED_INT, (void*)(mBatches[i].indexOffs * sizeof(int)));
    }
}

void Renderer::PrintBatches()
{
    for (uint i = 0; i < mBatches.size(); i++) {
        std::cout << "dtype: " << (mBatches[i].drawType == D_LINES ? "lines" : "triangles")
                  << ", texture id: " << mBatches[i].textureId
                  << ", verticesOffs: " << mBatches[i].verticesOffs
                  << ", numVertices: " << mBatches[i].numVertices
                  << ", indexOffs: " << mBatches[i].indexOffs
                  << ", numIndexes: " << mBatches[i].numIndexes
                  << std::endl;
    }
}
