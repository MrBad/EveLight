#include "texture.h"
#include "lodepng/lodepng.h"

Texture::~Texture()
{
    cout << "Texture deleted\n";
    glDeleteTextures(1, &mId);
}

Texture::Texture(const string& name, uint width, uint height, std::vector<uint8_t>& imageBuf)
{
    mName = name;
    mWidth = width;
    mHeight = height;
    Upload(imageBuf);
}

void Texture::Upload(const std::vector<unsigned char>& imageBuf)
{
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &mId);
    glBindTexture(GL_TEXTURE_2D, mId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        mWidth, mHeight,
        0, GL_RGBA, GL_UNSIGNED_BYTE, &imageBuf[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}
