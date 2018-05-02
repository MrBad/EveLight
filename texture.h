#pragma once

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Texture
{
  public:
    Texture();
    Texture(const string &name, uint width, uint height, std::vector<uint8_t> &imageBuf);
    ~Texture();
    uint getWidth() { return mWidth; }
    uint getHeight() { return mHeight; }
    uint getId() { return mId; }
    std::string &getName() { return mName; }

  private:
    void Upload(std::vector<unsigned char> &imageBuf);
    GLuint mId;
    uint mWidth, mHeight;
    std::string mName;
};
