#include <fstream>
#include <cassert>
#include <iostream>
#include "Assets.h"

//..................................................................................................
Assets::Assets()
{}

//..................................................................................................
void Assets::loadfromfile(const std::string& path)
{
    std::ifstream file(path);
    std::string type;
    
    while (file.good())
    {
        file >> type;

        if (type == "Texture")
        {
            std::string name, path;
            file >> name >> path;
            texture_map[name] = sf::Texture();
            if (!texture_map[name].loadFromFile(path))
            {
                std::cerr << "Could not load texture file: " << path << std::endl;
                texture_map.erase(name);
            }
            else
            {
                texture_map[name].setSmooth(true);
                std::cout << "Loaded Texture: " << path << ", " << name << std::endl;
            }
        }
        else if (type == "Font")
        {
            std::string name, path;
            file >> name >> path;
            font_map[name] = sf::Font();
            if (!font_map[name].loadFromFile(path))
            {
                std::cerr << "Could not load font file: " << path << std::endl;
                font_map.erase(name);
            }
            else
            {
                std::cout << "Loaded Font: " << path << ", " << name << std::endl;
            }
        }
        else if (type == "FragShader")
        {
            std::string name, path;
            file >> name >> path;
            if (!shader_map[name].loadFromFile(path, sf::Shader::Fragment))
            {
                std::cerr << "Could not load shader file: " << path << std::endl;
                shader_map.erase(name);
            }
            else
            {
                std::cout << "Loaded Shader: " << path << ", " << name << std::endl;
            }
        }
        else
        {
            std::cerr << "Unknown Asset Type: " << type << std::endl;
        }
    }
}

//..................................................................................................
sf::Texture& Assets::get_texture(const std::string& name)
{
    bool assertion = texture_map.find(name) != texture_map.end();
    if (!assertion)
        std::cerr << "Failed at `get_texture`: " << name << std::endl;
    assert(assertion);
    return texture_map.at(name);
}

//..................................................................................................
sf::Font& Assets::get_font(const std::string& name)
{
    bool assertion = font_map.find(name) != font_map.end();
    if (!assertion)
        std::cerr << "Failed at `get_font`: " << name << std::endl;
    assert(assertion);
    return font_map.at(name);
}

//..................................................................................................
sf::Shader& Assets::get_shader(const std::string& name)
{
    bool assertion = shader_map.find(name) != shader_map.end();
    if (!assertion)
        std::cerr << "Failed at `get_shader`: " << name << std::endl;
    assert(assertion);
    return shader_map.at(name);
}

//..................................................................................................
const sf::Texture& Assets::get_texture(const std::string& name) const
{
    bool assertion = texture_map.find(name) != texture_map.end();
    if (!assertion)
        std::cerr << "Failed at `get_texture`: " << name << std::endl;
    assert(assertion);
    return texture_map.at(name);
}

//..................................................................................................
const sf::Font& Assets::get_font(const std::string& name) const
{
    bool assertion = font_map.find(name) != font_map.end();
    if (!assertion)
        std::cerr << "Failed at `get_font`: " << name << std::endl;
    assert(assertion);
    return font_map.at(name);
}

//..................................................................................................
const sf::Shader& Assets::get_shader(const std::string& name) const
{
    bool assertion = shader_map.find(name) != shader_map.end();
    if (!assertion)
        std::cerr << "Failed at `get_shader`: " << name << std::endl;
    assert(assertion);
    return shader_map.at(name);
}