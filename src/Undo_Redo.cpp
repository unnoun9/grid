#include "Canvas.h"
#include "Undo_Redo.h"
#include "Filters.h"

//..................................................................................................
Edit::Edit(Type type, i32 layer)
    : type(type), layer(layer)
{
}

//..................................................................................................
void Undo_redo::undo()
{
    if (undostack.empty())
    {
        std::cerr << "Undo stack is empty\n";
        return;
    }

    Edit e = undostack.back();

    if (e.type == Edit::MOVE)
    {
        Layer* layer = &canv->layers[e.layer];
        if (!layer)
        {
            std::cerr << e.type << ": Move undo could not happen because the current layer is null\n";
            return;
        }

        std::swap(layer->pos, e.move_prev_pos);
    }
    else if (e.type == Edit::LAYER_ADD)
    {
        if (!e.layer)
        {
            std::cerr << e.type << ": Layer add undo could not happen because the layer index is null\n";
            return;
        }

        canv->layers.erase(canv->layers.begin() + e.layer);
    }
    else if (e.type == Edit::LAYER_REMOVE)
    {
        if (!e.removed_layer)
        {
            std::cerr << e.type << ": Layer remove undo could not happen because the removed layer is null\n";
            return;
        }

        canv->layers.insert(canv->layers.begin() + e.layer, *e.removed_layer);
    }
    else if (e.type == Edit::FILL || e.type == Edit::BRUSH || e.type == Edit::ERASE || e.type == Edit::BRIGHTNESS || e.type == Edit::CONTRAST || e.type == Edit::GRAYSCALE || e.type == Edit::SEPIA || e.type == Edit::PIXELATE || e.type == Edit::BOX_BLUR || e.type == Edit::GAUSSIAN_BLUR || e.type == Edit::SHARPEN || e.type == Edit::EDGE_DETECT || e.type == Edit::ROTATE)
    {
        Layer* layer = &canv->layers[e.layer];
        if (!layer || layer->type != Layer::RASTER)
        {
            std::cerr << e.type << ": Texture undo could not happen because the current layer is either null or not a raster layer\n";
            return;
        }
        Raster* raster = (Raster*)layer->graphic;
        if (!raster)
        {
            std::cerr << e.type << ": Texture undo could not happen because the current layer's graphic is null\n";
            return;
        }
        
        std::swap(*e.tex, raster->texture);
        raster->sprite.setTexture(raster->texture);
    }
    else if (e.type == Edit::INVERT)
    {
        Layer* layer = &canv->layers[e.layer];
        if (!layer || layer->type != Layer::RASTER)
        {
            std::cerr << e.type << ": Invert undo could not happen because the current layer is either null or not a raster layer\n";
            return;
        }
        Raster* raster = (Raster*)layer->graphic;
        if (!raster)
        {
            std::cerr << e.type << ": Invert undo could not happen because the current layer's graphic is null\n";
            return;
        }

        sf::Shader& invert_shader = canv->assets->get_shader("Invert");
        invert_shader.setUniform("texture", raster->texture);

        target.create(raster->texture.getSize().x, raster->texture.getSize().y);
        target.clear(sf::Color::Transparent);
        target.draw(raster->sprite, &invert_shader);
        target.display();

        raster->texture = target.getTexture();
        raster->sprite.setTexture(raster->texture);

    }
    else if (e.type == Edit::FLIPX)
    {
        Layer* layer = &canv->layers[e.layer];
        if (!layer || layer->type != Layer::RASTER)
        {
            std::cerr << e.type << ": Flipx undo could not happen because the current layer is either null or not a raster layer\n";
            return;
        }
        Raster* raster = (Raster*)layer->graphic;
        if (!raster)
        {
            std::cerr << e.type << ": Flipx undo could not happen because the current layer's graphic is null\n";
            return;
        }

        sf::Shader& flipx_shader = canv->assets->get_shader("FlipX");
        flipx_shader.setUniform("texture", raster->texture);
        
        target.create(raster->texture.getSize().x, raster->texture.getSize().y);
        target.clear(sf::Color::Transparent);
        target.draw(raster->sprite, &flipx_shader);
        target.display();

        raster->texture = target.getTexture();
        raster->sprite.setTexture(raster->texture);
    }
    else if (e.type == Edit::FLIPY)
    {
        Layer* layer = &canv->layers[e.layer];
        if (!layer || layer->type != Layer::RASTER)
        {
            std::cerr << e.type << ": Flipy undo could not happen because the current layer is either null or not a raster layer\n";
            return;
        }
        Raster* raster = (Raster*)layer->graphic;
        if (!raster)
        {
            std::cerr << e.type << ": Flipy undo could not happen because the current layer's graphic is null\n";
            return;
        }

        sf::Shader& flipy_shader = canv->assets->get_shader("FlipY");
        flipy_shader.setUniform("texture", raster->texture);
        
        target.create(raster->texture.getSize().x, raster->texture.getSize().y);
        target.clear(sf::Color::Transparent);
        target.draw(raster->sprite, &flipy_shader);
        target.display();

        raster->texture = target.getTexture();
        raster->sprite.setTexture(raster->texture);
    }

    redostack.push_back(e);
    undostack.pop_back();
}

//..................................................................................................
void Undo_redo::redo()
{
    if (redostack.empty())
    {
        std::cerr << "Redo stack is empty\n";
        return;
    }

    Edit e = redostack.back();

    if (e.type == Edit::LAYER_ADD)
    {
        if (!e.layer)
        {
            std::cerr << e.type << ": Layer add redo could not happen because the layer index is null\n";
            return;
        }

        canv->layers.insert(canv->layers.begin() + e.layer, *e.removed_layer);
    }
    else if (e.type == Edit::LAYER_REMOVE)
    {
        if (!e.removed_layer)
        {
            std::cerr << e.type << ": Layer remove redo could not happen because the removed layer is null\n";
            return;
        }

        canv->layers.erase(canv->layers.begin() + e.layer);
    }
    else if (e.type == Edit::MOVE)
    {
        Layer* layer = &canv->layers[e.layer];
        if (!layer)
        {
            std::cerr << e.type << ": Move redo could not happen because the current layer is null\n";
            return;
        }

        std::swap(layer->pos, e.move_prev_pos);
    }
    else if (e.type == Edit::FILL || e.type == Edit::BRUSH || e.type == Edit::ERASE || e.type == Edit::BRIGHTNESS || e.type == Edit::CONTRAST || e.type == Edit::GRAYSCALE || e.type == Edit::SEPIA || e.type == Edit::PIXELATE || e.type == Edit::BOX_BLUR || e.type == Edit::GAUSSIAN_BLUR || e.type == Edit::SHARPEN || e.type == Edit::EDGE_DETECT || e.type == Edit::ROTATE)
    {
        Layer* layer = &canv->layers[e.layer];
        if (!layer || layer->type != Layer::RASTER)
        {
            std::cerr << e.type << ": Texture redo could not happen because the current layer is either null or not a raster layer\n";
            return;
        }
        Raster* raster = (Raster*)layer->graphic;
        if (!raster)
        {
            std::cerr << e.type << ": Texture redo could not happen because the current layer's graphic is null\n";
            return;
        }

        std::swap(*e.tex, raster->texture);
        raster->sprite.setTexture(raster->texture);
    }
    else if (e.type == Edit::INVERT)
    {
        Layer* layer = &canv->layers[e.layer];
        if (!layer || layer->type != Layer::RASTER)
        {
            std::cerr << e.type << ": Invert redo could not happen because the current layer is either null or not a raster layer\n";
            return;
        }
        Raster* raster = (Raster*)layer->graphic;
        if (!raster)
        {
            std::cerr << e.type << ": Invert redo could not happen because the current layer's graphic is null\n";
            return;
        }

        sf::Shader& invert_shader = canv->assets->get_shader("Invert");
        invert_shader.setUniform("texture", raster->texture);

        target.create(raster->texture.getSize().x, raster->texture.getSize().y);
        target.clear(sf::Color::Transparent);
        target.draw(raster->sprite, &invert_shader);
        target.display();

        raster->texture = target.getTexture();
        raster->sprite.setTexture(raster->texture);        
    }
    else if (e.type == Edit::FLIPX)
    {
        Layer* layer = &canv->layers[e.layer];
        if (!layer || layer->type != Layer::RASTER)
        {
            std::cerr << e.type << ": Flipx redo could not happen because the current layer is either null or not a raster layer\n";
            return;
        }
        Raster* raster = (Raster*)layer->graphic;
        if (!raster)
        {
            std::cerr << e.type << ": Flipx redo could not happen because the current layer's graphic is null\n";
            return;
        }

        sf::Shader& flipx_shader = canv->assets->get_shader("FlipX");
        flipx_shader.setUniform("texture", raster->texture);
        
        target.create(raster->texture.getSize().x, raster->texture.getSize().y);
        target.clear(sf::Color::Transparent);
        target.draw(raster->sprite, &flipx_shader);
        target.display();

        raster->texture = target.getTexture();
        raster->sprite.setTexture(raster->texture);
    }
    else if (e.type == Edit::FLIPY)
    {
        Layer* layer = &canv->layers[e.layer];
        if (!layer || layer->type != Layer::RASTER)
        {
            std::cerr << e.type << ": Flipy redo could not happen because the current layer is either null or not a raster layer\n";
            return;
        }
        Raster* raster = (Raster*)layer->graphic;
        if (!raster)
        {
            std::cerr << e.type << ": Flipy redo could not happen because the current layer's graphic is null\n";
            return;
        }

        sf::Shader& flipy_shader = canv->assets->get_shader("FlipY");
        flipy_shader.setUniform("texture", raster->texture);
        
        target.create(raster->texture.getSize().x, raster->texture.getSize().y);
        target.clear(sf::Color::Transparent);
        target.draw(raster->sprite, &flipy_shader);
        target.display();

        raster->texture = target.getTexture();
        raster->sprite.setTexture(raster->texture);
    }

    undostack.push_back(e);
    redostack.pop_back();
}