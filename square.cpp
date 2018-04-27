#include "vertex.h"
#include "square.h"
#include <iostream>

using namespace std;

Square::Square()
{
    mX = mY = 0;
    mWidth = mHeight = 0.5;
}

Square::~Square()
{
    glDeleteBuffers(1, &mVB0);
    glDeleteBuffers(1, &mVAO);
}

void Square::Init(float x, float y, float width, float height)
{
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVB0);
    mX = x;
    mY = y;
    mWidth = width;
    mHeight = height;
    mDirty = true;
}

void Square::Update()
{
    if (!mDirty)
        return;
    cout << "Updating square with vbo: " << mVB0 << ", vao: " << mVAO  << endl;
    Vertex ventrices[6];
    ventrices[0].SetPos(mX + mWidth, mY + mHeight);
    ventrices[1].SetPos(mX, mY + mHeight);
    ventrices[2].SetPos(mX, mY);
    ventrices[3].SetPos(mX, mY);
    ventrices[4].SetPos(mX + mWidth, mY);
    ventrices[5].SetPos(mX + mWidth, mY + mHeight);

    glBindVertexArray(mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mVB0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ventrices), ventrices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    mDirty = false;
}

void Square::SetPos(float x, float y)
{
    mX = x;
    mY = y;
    mDirty = true;
}

void Square::Draw()
{
    if (mDirty)
        Square::Update();

    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
