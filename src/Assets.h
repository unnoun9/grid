#pragma once
#include <map>
#include <SFML/Graphics.hpp>

// stores assets in memory so that they are easily accessible
struct Assets
{
    std::map<std::string, sf::Texture>  texture_map;
    std::map<std::string, sf::Font>     font_map;
    std::map<std::string, sf::Shader>   shader_map;
    
    Assets();
    void loadfromfile(const std::string& path);
    sf::Texture& get_texture(const std::string& name);
    sf::Font& get_font(const std::string& name);
    sf::Shader& get_shader(const std::string& name);
    const sf::Texture& get_texture(const std::string& name) const;
    const sf::Font& get_font(const std::string& name) const;
    const sf::Shader& get_shader(const std::string& name) const;
};