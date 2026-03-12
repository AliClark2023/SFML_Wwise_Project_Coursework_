#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
/*
 * holds all inititalised assets used for game
 */
class FileAssets
{
    public:
    static const sf::Font& get_ui_font();
    
    private:
    static sf::Font ui_font_;
    
};