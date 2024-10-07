#include <iostream>             // for cin, cout, cerr
#include <memory>               // for shared_ptr, make_shared (easily manages memory allocations and deallocations)
#include <fstream>              // for ifstream (to read files)

#include <SFML/Graphics.hpp>    // for sfml drawing stuff 
#include "imgui.h"              // main imgui header
#include "imgui-SFML.h"         // for sfml and imgui compatibility

#include "vec2.h"               // 2D mathematical vector

struct Transform
{
    vec2 pos      = {0, 0};
    vec2 velocity = {0, 0};
};

struct Circle
{
    sf::CircleShape shape;
    float original_radius;
};

struct Rectangle
{
    sf::RectangleShape shape;
    vec2 original_size;
};

struct Properties
{
    std::string name;
    sf::Text text;
    sf::Color color;
    bool drawable = true;
};

struct Entity
{
    std::shared_ptr<Transform>  transform;
    std::shared_ptr<Circle>     circle;
    std::shared_ptr<Rectangle>  rect;
    std::shared_ptr<Properties> properties;
};

void clamp_color(int& c)
{
    if (c < 0) c = 0; 
    if (c > 255) c = 255;
}

// read the config file
void read_config(const std::string& filename, int& window_width, int& window_height, sf::Font& font, sf::Color& font_color, unsigned int& font_size, std::vector<Entity>& entities)
{
    std::ifstream stream(filename);
    if (!stream.is_open())
    {
        std::cerr << "Could not open config file: " << filename << '\n';
        exit(-1);
    }

    std::string type;
    // read from the file stream based on `type`
    while (stream >> type)
    {
        if (type == "Window")
        {
            int w, h;

            stream >> w >> h;

            window_width = (w <= 0) ? 1280 : w;
            window_height = (h <= 0) ? 720 : h;
        }
        else if (type == "Font")
        {
            int size, r, g, b;
            std::string path;

            stream >> path >> size >> r >> g >> b;

            clamp_color(r); clamp_color(g); clamp_color(b);

            if (!font.loadFromFile(path))
            {
                std::cerr << "Could not load font!\n";
                exit(-1);
            }
            font_color = {r, g, b};
            font_size = size;
        }
        else if (type == "Rectangle")
        {
            float x, y, sx, sy, w, h;
            int r, g, b;
            std::string name;

            stream >> name >> x >> y >> sx >> sy >> r >> g >> b >> w >> h;

            x = (x < 0) ? 10.0f : x;
            y = (y < 0) ? 10.0f : y;
            w = (w <= 0) ? 10.0f : w;
            h = (h <= 0) ? 10.0f : h;
            clamp_color(r); clamp_color(g); clamp_color(b);

            auto t = std::make_shared<Transform>();
            t->pos = {x, y};
            t->velocity = {sx, sy};
            
            auto rec = std::make_shared<Rectangle>();
            rec->shape.setSize(vec2(w, h));
            rec->shape.setFillColor(sf::Color(r, g, b));
            rec->original_size = vec2(w, h);

            auto props = std::make_shared<Properties>();
            props->color = {r, g, b};
            props->name = name;

            Entity e = {t, nullptr, rec, props};
            entities.push_back(e);
        }
        else if (type == "Circle")
        {
            float x, y, sx, sy, radius;
            int r, g, b;
            std::string name;

            stream >> name >> x >> y >> sx >> sy >> r >> g >> b >> radius;

            x = (x < 0) ? 10.0f : x;
            y = (y < 0) ? 10.0f : y;
            radius = (radius <= 0) ? 10.0f : radius;
            clamp_color(r); clamp_color(g); clamp_color(b);

            auto t = std::make_shared<Transform>();
            t->pos = {x, y};
            t->velocity = {sx, sy};
            
            auto circ = std::make_shared<Circle>();
            circ->shape.setRadius(radius);
            circ->shape.setFillColor(sf::Color(r, g, b));
            circ->original_radius = radius;

            auto props = std::make_shared<Properties>();
            props->color = {r, g, b};
            props->name = name;

            Entity e = {t, circ, nullptr, props};
            entities.push_back(e);
        }
    }
}

// update entities
void update_entities(std::vector<Entity>& entities, const int window_width, const int window_height)
{
    for (auto& e : entities)
    {
        // update velocity and position for transform, shape, and text
        // text is offseted, so translate it to the center using vector math
        e.transform->pos += e.transform->velocity;
        vec2 text_pos;
        if (e.circle)
        {
            e.circle->shape.setPosition(e.transform->pos);
            text_pos = e.transform->pos + e.circle->shape.getRadius();
        }
        if (e.rect)
        {
            e.rect->shape.setPosition(e.transform->pos);
            text_pos = e.transform->pos + vec2((e.rect->shape.getSize() * 0.5f));
        }
        sf::FloatRect text_bounds = e.properties->text.getLocalBounds();
        vec2 translation_v = vec2(text_bounds.getSize()) - vec2(0, -(float)(e.properties->text.getCharacterSize()/2));
        e.properties->text.setPosition(text_pos - (translation_v * 0.5f));

        // collision detection with window
        if (e.transform->pos.x < 0
            || e.transform->pos.x > window_width - ((e.circle)? 2 * e.circle->shape.getRadius() : e.rect->shape.getSize().x))
            e.transform->velocity.x *= -1;
        
        if (e.transform->pos.y < 0
            || e.transform->pos.y > window_height - ((e.circle)? 2 * e.circle->shape.getRadius() : e.rect->shape.getSize().y))
            e.transform->velocity.y *= -1;
    }
}

// draw entities
void draw_entities(std::vector<Entity>& entities, sf::RenderWindow& window)
{
    for (auto& e : entities)
    {
        if (e.circle && e.properties->drawable) window.draw(e.circle->shape);
        if (e.rect && e.properties->drawable) window.draw(e.rect->shape);
        if ((e.circle || e.rect) && e.properties->drawable) window.draw(e.properties->text);
    }
}

// main
int main(int argc, char * argv[])
{
    // needed variables
    int window_width = 1280, window_height = 720;
    sf::RenderWindow window;
    std::vector<Entity> entities;
    sf::Color font_color;
    unsigned int font_size;
    sf::Font font;

    // read the config file and fill the variables
    read_config("config.txt", window_width, window_height, font, font_color, font_size, entities);

    // create window
    window.create(sf::VideoMode(window_width, window_height), "A1");
    window.setFramerateLimit(60);

    // set the entities' text up after reading config because "Font" in config can be in any line
    for (auto& e : entities)
    {
        e.properties->text.setFont(font);
        e.properties->text.setCharacterSize(font_size);
        e.properties->text.setFillColor(font_color);
        e.properties->text.setString(e.properties->name);
        e.properties->text.setPosition(e.transform->pos);
    }

    // setup ImGui stuff
    ImGui::SFML::Init(window);
    sf::Clock clock;
    ImGui::GetStyle().ScaleAllSizes(1.0f);

    // variables useful for updating shapes with ImGui
    bool is_changed = false;
    size_t esize = entities.size();
    char* shape_names[esize];
    for (size_t i = 0; i < esize; i++)
        shape_names[i] = (char*) entities[i].properties->name.c_str();
    int selected_shape = 0;
    bool drawable = entities[0].properties->drawable;
    float scale_factor = 1.0f;
    float vel[2] = {entities[0].transform->velocity.x, entities[0].transform->velocity.y};
    sf::Color& ec = entities[0].properties->color;
    float c[3];
    c[0] = ec.r / 255.0f;
    c[1] = ec.g / 255.0f;
    c[2] = ec.b / 255.0f;
    char* display_string = shape_names[0];
 
    // main loop
    while (window.isOpen())
    {
        // event handling
        sf::Event event;
        while (window.pollEvent(event))
        {
            // pass the event to imgui to be parsed
            ImGui::SFML::ProcessEvent(window, event);

            // handle events
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // update imgui for this frame with the time that the last frame took
        ImGui::SFML::Update(window, clock.restart());

        ImGui::Begin("Shape Properties");
        // selecting a shape
        is_changed = ImGui::Combo("Shape", &selected_shape, shape_names, IM_ARRAYSIZE(shape_names));
        auto& e = entities[selected_shape];
        if (is_changed)
        {
            drawable = e.properties->drawable;
            vel[0] = e.transform->velocity.x;
            vel[1] = e.transform->velocity.y;
            sf::Color& ec = e.properties->color;
            c[0] = ec.r / 255.0f;
            c[1] = ec.g / 255.0f;
            c[2] = ec.b / 255.0f;
            if (e.circle) scale_factor = e.circle->shape.getRadius() / e.circle->original_radius;
            else if (e.rect) scale_factor = e.rect->shape.getSize().x / e.rect->original_size.x;
            display_string = shape_names[selected_shape];
        } 
        // toggling visibility
        if(ImGui::Checkbox("Draw Shape", &drawable))
        {
            e.properties->drawable = drawable;
        }
        // scaling size
        if (ImGui::SliderFloat("Scale", &scale_factor, 0, 4))
        {
            if (e.circle) e.circle->shape.setRadius(e.circle->original_radius * scale_factor);
            else if (e.rect) e.rect->shape.setSize(e.rect->original_size * scale_factor);
        }
        // changing velocity
        if (ImGui::SliderFloat2("Velocity", vel, -8, 8))
        {
            e.transform->velocity.x = vel[0];
            e.transform->velocity.y = vel[1];
        }
        // changing color
        if(ImGui::ColorEdit3("Color", c))
        {
            e.properties->color = sf::Color(c[0]*255, c[1]*255, c[2]*255);
            if (e.circle) e.circle->shape.setFillColor(e.properties->color);
            else if (e.rect) e.rect->shape.setFillColor(e.properties->color);
        }
        // changing name
        if(ImGui::InputText("Name", display_string, IM_ARRAYSIZE(display_string)))
        {
            e.properties->name.assign(display_string);
            e.properties->text.setString(e.properties->name);
        }
        ImGui::End();
        
        // update entities
        update_entities(entities, window_width, window_height);

        // render / draw everything
        window.clear();
        draw_entities(entities, window);
        ImGui::SFML::Render(window);
        window.display();
    }

    return 0;
}