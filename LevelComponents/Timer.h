#pragma once
#include <SFML/Graphics/Font.hpp>
#include "../Utilities/FileLoading.h"

/*
 * Class that creates timer and contains functions to display and adjust time
 */
class timer
{
public:
    timer();
    void render_timer(sf::RenderWindow& window) const;
    sf::Text* get_text() const;
    sf::Time get_time() const;
    
    void pause_timer() { timer_.stop(); }
    void resume_timer() { timer_.start(); }
private:
    // time variables
    sf::Clock timer_;
    
    // text variables
    sf::Font timer_font_;
    std::unique_ptr<sf::Text> timer_text_;
};
