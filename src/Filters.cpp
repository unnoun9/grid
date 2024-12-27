#include "util.h"
#include "int.h"
#include "Filters.h"

// ..................................................................................................
Filters::Filters(Canvas *canv)
    : canv(canv)
{
}

void Filters::apply_filter(const std::string& filter)
{
    Layer* layer = canv->current_layer();
    if (!layer || layer->type != Layer::RASTER)
        return;

    Raster* raster = (Raster*)layer->graphic;
    vec2 layer_size = raster->texture.getSize();

    sf::Shader* shader = nullptr;
    if (filter != "BoxBlur" && filter != "GaussianBlur")
        shader = &canv->assets->get_shader(filter);
    sf::Shader& filter_shader = *shader;

    target.create(layer_size.x, layer_size.y);

    if (filter == "Brightness")
        filter_shader.setUniform("brightness", brightness_strength / 255.f);
    else if (filter == "Contrast")
        filter_shader.setUniform("contrast", (float)contrast_strength);
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
        horizontal_shader.setUniform("layer_pos", sf::Glsl::Vec2(layer->pos.x, layer->pos.y));
        horizontal_shader.setUniform("canvas_size", sf::Glsl::Vec2(canv->size.x, canv->size.y));

        intermediate_target.clear(sf::Color::Transparent);
        intermediate_target.draw(raster->sprite, &horizontal_shader);
        intermediate_target.display();

        // vertical pass
        sf::Sprite intermediate_sprite(intermediate_target.getTexture());
        vertical_shader.setUniform("texture", intermediate_target.getTexture());
        vertical_shader.setUniform("blur_size", (float)box_blur_strength);
        vertical_shader.setUniform("texel_size", sf::Glsl::Vec2(1.f / layer_size.x, 1.f / layer_size.y));
        vertical_shader.setUniform("layer_pos", sf::Glsl::Vec2(layer->pos.x, layer->pos.y));
        vertical_shader.setUniform("canvas_size", sf::Glsl::Vec2(canv->size.x, canv->size.y));

        target.clear(sf::Color::Transparent);
        target.draw(intermediate_sprite, &vertical_shader);
        target.display();

        raster->texture = target.getTexture();
        raster->sprite.setTexture(raster->texture);
        raster->data = raster->texture.copyToImage();
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
        horizontal_shader.setUniform("layer_pos", sf::Glsl::Vec2(layer->pos.x, layer->pos.y));
        horizontal_shader.setUniform("canvas_size", sf::Glsl::Vec2(canv->size.x, canv->size.y));

        intermediate_target.clear(sf::Color::Transparent);
        intermediate_target.draw(raster->sprite, &horizontal_shader);
        intermediate_target.display();

        // vertical pass
        sf::Sprite intermediate_sprite(intermediate_target.getTexture());
        vertical_shader.setUniform("texture", intermediate_target.getTexture());
        vertical_shader.setUniform("blur_size", (float)gauss_blur_strength);
        vertical_shader.setUniform("texel_size", sf::Glsl::Vec2(1.f / layer_size.x, 1.f / layer_size.y));
        vertical_shader.setUniform("layer_pos", sf::Glsl::Vec2(layer->pos.x, layer->pos.y));
        vertical_shader.setUniform("canvas_size", sf::Glsl::Vec2(canv->size.x, canv->size.y));

        target.clear(sf::Color::Transparent);
        target.draw(intermediate_sprite, &vertical_shader);
        target.display();

        raster->texture = target.getTexture();
        raster->sprite.setTexture(raster->texture);
        raster->data = raster->texture.copyToImage();
        return;
    }

    filter_shader.setUniform("texture", raster->texture);
    vec2 pos = layer->pos - canv->start_pos;
    filter_shader.setUniform("layer_pos", sf::Glsl::Vec2(pos.x, pos.y));
    filter_shader.setUniform("canvas_size", sf::Glsl::Vec2(canv->size.x, canv->size.y));

    target.draw(raster->sprite, &filter_shader);
    target.display();
    raster->texture = target.getTexture();
    raster->sprite.setTexture(raster->texture);
    raster->data = raster->texture.copyToImage();
}

// ..................................................................................................
void cpu_filters::gray_scale(Raster &img)
{
    sf::Image& data = img.data;
    for (int y = 0; y < data.getSize().y; y++)
    {
        for (int x = 0; x < data.getSize().x; x++)
        {
            sf::Color pix = data.getPixel(x, y);
            i32 gray_scale_value = 0.299 * pix.r + 0.587 * pix.g + 0.144 * pix.b;
            util::clamp(gray_scale_value, 0, 255);
            data.setPixel(x, y, 
                sf::Color((ui8)gray_scale_value, (ui8)gray_scale_value, (ui8)gray_scale_value, pix.a));
        }
    }
    img.update_texture();
} 

// ..................................................................................................
void cpu_filters::color_mask(Raster& img, float r, float g, float b)
{
    sf::Image& data = img.data;
    for (int y = 0; y < data.getSize().y; y++)
    {
        for (int x = 0; x < data.getSize().x; x++)
        {
            sf::Color pix = data.getPixel(x, y);
            i32 new_r = r * pix.r;
            i32 new_g = g * pix.g;
            i32 new_b = b * pix.b;
            util::clamp(new_r, 0, 255);
            util::clamp(new_g, 0, 255);
            util::clamp(new_b, 0, 255);
            data.setPixel(x, y, sf::Color((ui8)new_r, (ui8)new_g, (ui8)new_b, pix.a));
        }
    }
    img.update_texture();
}

// ..................................................................................................
void cpu_filters::flip_horizontally(Raster& img)
{
    int width = img.data.getSize().x;
    int height = img.data.getSize().y;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width / 2; ++x)
        {
            sf::Color left_pixel = img.data.getPixel(x, y);
            sf::Color right_pixel = img.data.getPixel(width - x - 1, y);

            img.data.setPixel(x, y, right_pixel);
            img.data.setPixel(width - x - 1, y, left_pixel);
        }
    }
    img.update_texture();
}

// ..................................................................................................
void cpu_filters::flip_vertically(Raster& img)
{
    int width = img.data.getSize().x;
    int height = img.data.getSize().y;

    for (int y = 0; y < height / 2; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            sf::Color top_pixel = img.data.getPixel(x, y);
            sf::Color bottom_pixel = img.data.getPixel(x, height - y - 1);

            img.data.setPixel(x, y, bottom_pixel);
            img.data.setPixel(x, height - y - 1, top_pixel);
        }
    }
    img.update_texture();
}