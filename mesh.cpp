// DOC: http://sunandblackcat.com/tipFullView.php?l=eng&topicid=18&topic=OpenGL-VAO-VBO-EBO
#include <iostream>
#include "mesh.h"

Mesh::Mesh()
{
    mDirty = false;
    mVertexArray = mVertexBuffer = mIndexBuffer = 0;
    mDrawType = GL_LINES;
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mVertexArray);
    glDeleteBuffers(1, &mIndexBuffer);
}

void Mesh::SetVertices(const vector<Vertex> &vertices)
{
    mVertices = vertices;
    mDirty = true;
}

void Mesh::SetIndexes(const vector<int> &indexes)
{
    mIndexes = indexes;
    mDirty = true;
}

void Mesh::SetDrawType(GLenum drawType)
{
    mDrawType = drawType;
}

void Mesh::AddVertex(const Vertex& vertex)
{
    mVertices.push_back(vertex);
    mDirty = true;
}

void Mesh::AddIndex(const int index)
{
    mIndexes.push_back(index);
    mDirty = true;
}

void Mesh::Update()
{
    enum shader_attrs { POSITION, COLOR, SHADER_NUM_ATTRS };
        
    if (!mVertexArray)
    {
        glGenVertexArrays(1, &mVertexArray);
        glGenBuffers(1, &mVertexBuffer);
        glGenBuffers(1, &mIndexBuffer);
    }
    glBindVertexArray(mVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(mVertices[0]), &mVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexes.size() * sizeof(mIndexes[0]), &mIndexes[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(POSITION);
    glEnableVertexAttribArray(COLOR);
    /* attribute index, number of "coordinates" per vertex (size), type of "coordinate", 
     * normalized, stride, pointer */
    glVertexAttribPointer(
        POSITION, sizeof(mVertices[0].pos) / sizeof(float), GL_FLOAT, GL_FALSE,
        sizeof(mVertices[0]), (void*)offsetof(Vertex, pos));

    glVertexAttribPointer(
        COLOR, sizeof(mVertices[0].color) / sizeof(uint8_t), GL_UNSIGNED_BYTE, GL_TRUE,
        sizeof(mVertices[0]), (void*)offsetof(Vertex, color));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    mDirty = false;
}

void Mesh::Draw()
{
    if (mDirty)
        Update();
    
    glBindVertexArray(mVertexArray);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glDrawElements(mDrawType, mIndexes.size(), GL_UNSIGNED_INT, nullptr);
}
