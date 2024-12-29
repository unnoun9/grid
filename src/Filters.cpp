#include "util.h"
#include "int.h"
#include "Filters.h"
#include "Undo_Redo.h"

extern Undo_Redo undo_redo;

// ..................................................................................................
Filters::Filters(Canvas *canv)
    : canv(canv)
{
}

// ..................................................................................................
void Filters::apply_filter(const std::string& filter)
{
    Layer* layer = canv->current_layer();
    if (!layer || layer->type != Layer::RASTER)
        return;

    Raster* raster = (Raster*)layer->graphic;
    vec2 layer_size = raster->texture.getSize();

    sf::Shader* shader = nullptr;
    if (filter != "BoxBlur" && filter != "GaussianBlur" && filter != "EdgeDetection" && filter != "FlipX" && filter != "FlipY" && filter != "Rotate" && filter != "RotateCW" && filter != "RotateCCW")
        shader = &canv->assets->get_shader(filter);
    sf::Shader& filter_shader = *shader;

    target.create(layer_size.x, layer_size.y);

    if (filter == "Brightness")
        filter_shader.setUniform("brightness", brightness_strength / 255.f);
    else if (filter == "Contrast")
        filter_shader.setUniform("contrast", (float)contrast_strength);
    else if (filter == "Pixelate")
        filter_shader.setUniform("pixelate_size", (float)pixelate_size);
    else if (filter == "BoxBlur")
    {
        sf::Shader& horizontal_shader = canv->assets->get_shader("BoxBlurHorizontal");
        sf::Shader& vertical_shader = canv->assets->get_shader("BoxBlurVertical");

        sf::RenderTexture intermediate_target;
        intermediate_target.create(layer_size.x, layer_size.y);

        // horizontal pass
        horizontal_shader.setUniform("texture", *raster->sprite.getTexture());
        horizontal_shader.setUniform("blur_size", (float)box_blur_strength);
        horizontal_shader.setUniform("texel_size", sf::Glsl::Vec2(1.f / layer_size.x, 1.f / layer_size.y));

        intermediate_target.clear(sf::Color::Transparent);
        intermediate_target.draw(raster->sprite, &horizontal_shader);
        intermediate_target.display();

        // vertical pass
        sf::Sprite intermediate_sprite(intermediate_target.getTexture());
        vertical_shader.setUniform("texture", intermediate_target.getTexture());
        vertical_shader.setUniform("blur_size", (float)box_blur_strength);
        vertical_shader.setUniform("texel_size", sf::Glsl::Vec2(1.f / layer_size.x, 1.f / layer_size.y));

        target.clear(sf::Color::Transparent);
        target.draw(intermediate_sprite, &vertical_shader);
        target.display();

        undo_redo.undostack.push_back({ Edit::Type::BOX_BLUR, canv->current_layer_index, new sf::Texture(raster->texture) });
        undo_redo.redostack.clear();
        raster->texture = target.getTexture();
        raster->sprite.setTexture(raster->texture);
        return;
    }
    else if (filter == "GaussianBlur")
    {
        sf::Shader& horizontal_shader = canv->assets->get_shader("GaussianBlurHorizontal");
        sf::Shader& vertical_shader = canv->assets->get_shader("GaussianBlurVertical");

        sf::RenderTexture intermediate_target;
        intermediate_target.create(layer_size.x, layer_size.y);

        // horizontal pass
        horizontal_shader.setUniform("texture", *raster->sprite.getTexture());
        horizontal_shader.setUniform("blur_size", (float)gauss_blur_strength);
        horizontal_shader.setUniform("texel_size", sf::Glsl::Vec2(1.f / layer_size.x, 1.f / layer_size.y));

        intermediate_target.clear(sf::Color::Transparent);
        intermediate_target.draw(raster->sprite, &horizontal_shader);
        intermediate_target.display();

        // vertical pass
        sf::Sprite intermediate_sprite(intermediate_target.getTexture());
        vertical_shader.setUniform("texture", intermediate_target.getTexture());
        vertical_shader.setUniform("blur_size", (float)gauss_blur_strength);
        vertical_shader.setUniform("texel_size", sf::Glsl::Vec2(1.f / layer_size.x, 1.f / layer_size.y));

        target.clear(sf::Color::Transparent);
        target.draw(intermediate_sprite, &vertical_shader);
        target.display();

        undo_redo.undostack.push_back({ Edit::Type::GAUSSIAN_BLUR, canv->current_layer_index, new sf::Texture(raster->texture) });
        undo_redo.redostack.clear();
        raster->texture = target.getTexture();
        raster->sprite.setTexture(raster->texture);
        return;
    }
    else if (filter == "EdgeDetection")
    {
        sf::Shader& edge_shader = canv->assets->get_shader("EdgeDetection");
        edge_shader.setUniform("texture", raster->texture);

        target.clear(sf::Color::Transparent);
        target.draw(raster->sprite, &edge_shader);
        target.display();

        undo_redo.undostack.push_back({ Edit::Type::EDGE_DETECT, canv->current_layer_index, new sf::Texture(raster->texture) });
        undo_redo.redostack.clear();
        raster->texture = target.getTexture();
        raster->sprite.setTexture(raster->texture);
        return;
    }
    else if (filter == "FlipX")
    {
        sf::Shader& flipx_shader = canv->assets->get_shader("FlipX");
        flipx_shader.setUniform("texture", raster->texture);

        target.clear(sf::Color::Transparent);
        target.draw(raster->sprite, &flipx_shader);
        target.display();

        undo_redo.undostack.push_back({ Edit::Type::FLIPX, canv->current_layer_index, nullptr });
        undo_redo.redostack.clear();
        raster->texture = target.getTexture();
        raster->sprite.setTexture(raster->texture);
        return;
    }
    else if (filter == "FlipY")
    {
        sf::Shader& flipy_shader = canv->assets->get_shader("FlipY");
        flipy_shader.setUniform("texture", raster->texture);

        target.clear(sf::Color::Transparent);
        target.draw(raster->sprite, &flipy_shader);
        target.display();

        undo_redo.undostack.push_back({ Edit::Type::FLIPY, canv->current_layer_index, nullptr });
        undo_redo.redostack.clear();
        raster->texture = target.getTexture();
        raster->sprite.setTexture(raster->texture);
        return;
    }
    else if (filter == "Rotate" || filter == "RotateCW" || filter == "RotateCCW")
    {
        sf::Shader& rotate_shader = canv->assets->get_shader("Rotate");
        rotate_shader.setUniform("texture", raster->texture);

        float angle = rotate_angle;
        if (filter == "RotateCW")
            angle = 90.0f;
        else if (filter == "RotateCCW")
            angle = -90.0f;

        angle *= 3.14159265358979323846 / 180.0f;
        rotate_shader.setUniform("angle", angle);

        target.clear(sf::Color::Transparent);
        target.draw(raster->sprite, &rotate_shader);
        target.display();

        Edit::Type edit_type = Edit::Type::NONE;
        if (filter == "Rotate")
            edit_type = Edit::Type::ROTATE;
        else if (filter == "RotateCW")
            edit_type = Edit::Type::ROTATECW;
        else if (filter == "RotateCCW")
            edit_type = Edit::Type::ROTATECCW;
        undo_redo.undostack.push_back({ edit_type, canv->current_layer_index, new int(rotate_angle) });
        undo_redo.redostack.clear();
        raster->texture = target.getTexture();
        raster->sprite.setTexture(raster->texture);
        return;
    }

    filter_shader.setUniform("texture", raster->texture);

    target.clear(sf::Color::Transparent);
    target.draw(raster->sprite, &filter_shader);
    target.display();
    Edit::Type edit_type = Edit::Type::NONE;
    if (filter == "Brightness")
        edit_type = Edit::Type::BRIGHTNESS;
    else if (filter == "Contrast")
        edit_type = Edit::Type::CONTRAST;
    else if (filter == "Grayscale")
        edit_type = Edit::Type::GRAYSCALE;
    else if (filter == "Invert")
        edit_type = Edit::Type::INVERT;
    else if (filter == "Sepia")
        edit_type = Edit::Type::SEPIA;
    undo_redo.undostack.push_back({ edit_type, canv->current_layer_index, new sf::Texture(raster->texture) });
    undo_redo.redostack.clear();
    raster->texture = target.getTexture();
    raster->sprite.setTexture(raster->texture);
}