#include "Raster.h"

Raster::Raster()
{
    texture.create(1, 1);
    sprite.setTexture(texture);
}

bool Raster::update_texture()
{
    if (texture.getSize() != data.getSize() && !texture.create(data.getSize().x, data.getSize().y))
    {
        std::cerr << "Failed to create texture of size: " << (vec2)data.getSize() << " while updating\n";
        return false;
    }
    texture.loadFromImage(data);
    sprite.setTexture(texture, true);
    return true;
}

bool Raster::loadfromfile(const std::string& path)
{
    if (!data.loadFromFile(path))
    {
        std::cerr << "Failed to load image: " << path << "\n";
        return false;
    }
    update_texture();
    return true;
}