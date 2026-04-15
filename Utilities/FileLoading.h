#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class FileLoading
{
public:
    static void load_font(sf::Font& font, const std::string& font_name);
};
