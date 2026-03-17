#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
/*
 * Contains functions to load specified asset files (textures, fonts etc)
 */

class asset_utilities
{
    public:
    static void LoadFont(sf::Font& font, const std::string& font_name);
};