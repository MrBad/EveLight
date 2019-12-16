#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include "camera.h"
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

namespace evl {

Camera::Camera() {}

Camera::Camera(float screenWidth, float screenHeight, float scale, float speed)
{
    mScrDim.x = screenWidth;
    mScrDim.y = screenHeight;
    mScale = scale;
    mSpeed = speed;
    mDirty = true;
}

void Camera::SetPos(float x, float y)
{
    mPos.x = x;
    mPos.y = y;
    mDirty = true;
}

void Camera::SetPos(const glm::vec2& pos)
{
    mPos = pos;
    mDirty = true;
}

void Camera::ComputeMatrix()
{
    glm::mat4 Projection = glm::ortho(0.0f, mScrDim.x, 0.0f, mScrDim.y, -1.0f, 1.0f);
    glm::mat4 View = glm::mat4(1);
    glm::mat4 Scale = glm::scale(glm::mat4(1), glm::vec3(mScale));
    glm::mat4 Translate = glm::translate(
        glm::mat4(1),
        glm::vec3(-mPos.x * mScale + mScrDim.x / 2, -mPos.y * mScale + mScrDim.y / 2, 0));
    glm::mat4 Model = Translate * Scale;

    mMatrix = Projection * View * Model;
    // std::cout << glm::to_string(mMatrix) << std::endl;
    mDirty = false;
}

glm::mat4 Camera::GetMatrix()
{
    if (mDirty)
        ComputeMatrix();

    return mMatrix;
}

void Camera::SetMatrix(uint programId, const std::string& name)
{
    GLint pLoc = glGetUniformLocation(programId, name.c_str());
    if (mDirty)
        ComputeMatrix();
    glUniformMatrix4fv(pLoc, 1, GL_FALSE, &mMatrix[0][0]);
}

AABB Camera::GetAABB()
{
    return AABB(
        (mPos.x - mScrDim.x / 2 / mScale),
        (mPos.y - mScrDim.y / 2 / mScale),
        (mPos.x + mScrDim.x / 2 / mScale),
        (mPos.y + mScrDim.y / 2 / mScale));
}
}
