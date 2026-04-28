#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

/*  Helper class to load in file assets, currently only loading font is implemented
 */
class FileLoading
{
public:
    static void load_font(sf::Font& font, const std::string& font_name);
};
