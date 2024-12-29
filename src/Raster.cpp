#include "Canvas.h"
#include "Raster.h"

//..................................................................................................
Raster::Raster()
{
    texture.create(1, 1);
    sprite.setTexture(texture);
}

//..................................................................................................
bool Raster::loadfromfile(const std::string& path, Canvas* canv)
{
    sf::Image img;
    if (!img.loadFromFile(path))
    {
        std::cerr << "Failed to load image: " << path << "\n";
        return false;
    }
    
    sf::RenderTexture target;

    texture.loadFromImage(img);
    sprite.setTexture(texture, true);

    if (!canv || canv && !canv->initialized)
    {
        target.create(texture.getSize().x, texture.getSize().y);
    }
    else if (canv && canv->initialized)
    {
        sprite.setPosition((canv->size - (vec2)(texture.getSize())) / 2);
        target.create(canv->size.x, canv->size.y);
    }

    target.clear(sf::Color(255, 255, 255, 0));
    target.draw(sprite);
    target.display();

    texture = target.getTexture();
    sprite.setTexture(texture, true);

    return true;
}

//..................................................................................................
void Raster::create_blank(vec2 size, const sf::Color& color)
{
    texture.create(size.x, size.y);
    std::vector<ui8> pixels(size.x * size.y * 4, 0);

    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            int index = (y * size.x + x) * 4;
            pixels[index] = color.r;
            pixels[index + 1] = color.g;
            pixels[index + 2] = color.b;
            pixels[index + 3] = color.a;
        }
    }

    texture.update(pixels.data());
    sprite.setTexture(texture, true);
}
