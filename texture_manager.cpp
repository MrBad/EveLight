#include "texture_manager.h"
#include "lodepng/lodepng.h"

TextureManager::TextureManager()
{  
}

Texture *TextureManager::Get(const std::string &name)
{
    auto it = mTextures.find(name);
    if (it == mTextures.end())
    {
        std::cerr << "Cannot find texture: " << name << std::endl;
        return nullptr;
    }

    return it->second;
}

bool TextureManager::Add(const std::string &name, Texture *texture)
{
    auto ret = mTextures.insert(pair<string, Texture *>(name, texture));
    if (ret.second == false)
    {
        std::cerr << "Texture " << name << " already added" << std::endl;
        return false;
    }

    return true;
}

Texture *TextureManager::Add(const std::string &genericName, const std::string &path)
{
    uint width, height, err;
    std::vector<uint8_t> imageBuf;
    err = lodepng::decode(imageBuf, width, height, path);
    if (err)
    {
        std::cerr
            << "Cannot open " << path << ": (" << err << ")"
            << lodepng_error_text(err) << std::endl;
        return nullptr;
    }
    Texture *texture = new Texture(path, width, height, imageBuf);
    if (Add(genericName, texture)) 
        return texture;

    delete texture;

    return nullptr;
}

TextureManager::~TextureManager()
{
    for (auto i = mTextures.begin(); i != mTextures.end(); i++)
        delete i->second;
}
