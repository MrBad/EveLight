#pragma once

#include "texture.h"
#include <iostream>
#include <map>

class TextureManager {
public:
    TextureManager();
    ~TextureManager();
    Texture* Get(const std::string& name);
    bool Add(const std::string& name, Texture* texture);
    Texture* Add(const std::string& genericName, const std::string& path);

private:
    std::map<const std::string, Texture*> mTextures;
};