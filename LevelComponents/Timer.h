#pragma once
#include <SFML/Graphics/Font.hpp>
#include "../Utilities/AssetUtilities.h"

class Timer
{
public:
    Timer();
    //void start_timer();
    void render_timer(sf::RenderWindow& window) const;
    sf::Text* get_text() const;
    sf::Time get_time();
private:
    // time variables
    sf::Clock timer_;
    bool started_ = false;
    float elapsed_time_ = 0.0f;
    
    // text variables
    sf::Font timer_font_;
    std::unique_ptr<sf::Text> timer_text_;
};
