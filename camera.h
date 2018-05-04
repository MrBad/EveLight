#pragma once

#include <glm/glm.hpp>

class Camera
{
  public:
    Camera();
    Camera(float screenWidth, float screenHeight, float scale, float speed);
    void SetPos(float x, float y);
    glm::mat4 GetMatrix();
    void SetMatrix(uint programId, const std::string &name);
    void SetScale(float scale) { mScale = scale; mDirty = true;}
    float GetScale() { return mScale; }
    float GetX() { return mPos.x; }
    float GetY() { return mPos.y; }

  private:
    void ComputeMatrix();
    glm::vec2 mPos;
    glm::vec2 mScrDim;
    float mScale, mSpeed;
    glm::mat4 mMatrix;
    bool mDirty;
};
