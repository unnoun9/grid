#include "util.h"
#include "int.h"
#include "Filters.h"
#include "Undo_Redo.h"

// ..................................................................................................
Filters::Filters(Canvas *canv, Undo_redo *ur)
    : canv(canv), ur(ur)
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

    // undo-redo-related
    ur->redostack.clear();
    Edit::Type edit_type = Edit::NONE;
    if (filter == "FlipX") edit_type = Edit::FLIPX;
    else if (filter == "FlipY") edit_type = Edit::FLIPY;
    else if (filter == "Rotate" || filter == "RotateCW" || filter == "RotateCCW") edit_type = Edit::ROTATE;
    else if (filter == "Brightness") edit_type = Edit::BRIGHTNESS;
    else if (filter == "Contrast") edit_type = Edit::CONTRAST;
    else if (filter == "GrayScale") edit_type = Edit::GRAYSCALE;
    else if (filter == "Invert") edit_type = Edit::INVERT;
    else if (filter == "Sepia") edit_type = Edit::SEPIA;
    else if (filter == "Pixelate") edit_type = Edit::PIXELATE;
    else if (filter == "BoxBlur") edit_type = Edit::BOX_BLUR;
    else if (filter == "GaussianBlur") edit_type = Edit::GAUSSIAN_BLUR;
    else if (filter == "EdgeDetection") edit_type = Edit::EDGE_DETECT;
    Edit e(edit_type, canv->current_layer_index);

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

        e.tex = std::make_shared<sf::Texture>(raster->texture);
        ur->undostack.push_back(e);
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

        e.tex = std::make_shared<sf::Texture>(raster->texture);
        ur->undostack.push_back(e);
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

        e.tex = std::make_shared<sf::Texture>(raster->texture);
        ur->undostack.push_back(e);
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

        ur->undostack.push_back(e);
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

        ur->undostack.push_back(e);
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

        e.tex = std::make_shared<sf::Texture>(raster->texture);   
        ur->undostack.push_back(e);
        raster->texture = target.getTexture();
        raster->sprite.setTexture(raster->texture);
        return;
    }

    filter_shader.setUniform("texture", raster->texture);

    target.clear(sf::Color::Transparent);
    target.draw(raster->sprite, &filter_shader);
    target.display();

    if (e.type != Edit::INVERT)
        e.tex = std::make_shared<sf::Texture>(raster->texture);   
    ur->undostack.push_back(e);
    raster->texture = target.getTexture();
    raster->sprite.setTexture(raster->texture);
}