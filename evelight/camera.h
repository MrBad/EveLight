#pragma once

#include "aabb.h"
#include <glm/glm.hpp>
#include <string>

namespace evl {

class Camera {
public:
    Camera();
    Camera(float screenWidth, float screenHeight, float scale, float speed);
    void SetPos(float x, float y);
    void SetPos(const glm::vec2& pos);
    glm::vec2 GetPos() { return mPos; }
    glm::mat4 GetMatrix();
    void SetMatrix(uint programId, const std::string& name);
    void SetScale(float scale)
    {
        mScale = scale;
        mDirty = true;
    }
    float GetScale() { return mScale; }
    float GetX() { return mPos.x; }
    float GetY() { return mPos.y; }
    AABB GetAABB();

private:
    void ComputeMatrix();
    glm::vec2 mPos;
    glm::vec2 mScrDim;
    float mScale, mSpeed;
    glm::mat4 mMatrix;
    bool mDirty;
};
}
