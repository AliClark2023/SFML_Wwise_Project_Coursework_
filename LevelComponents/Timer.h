#pragma once
#include <SFML/Graphics/Font.hpp>
#include "../Utilities/FileLoading.h"

/*
 * Class that creates timer and contains functions to display and adjust time
 * (used in conjunction with Menu UI)
 */
class timer
{
public:
    timer();
    timer(sf::Font& font);
    void render_timer(sf::RenderWindow& window) const;
    sf::Text* get_text() const;
    sf::Time get_time() const;
    
    sf::Clock& get_clock() {return timer_;}
    void set_pos(const sf::Vector2f& pos) const { timer_text_->setPosition(pos);}
    
    void pause_timer() { timer_.stop(); }
    void resume_timer() { timer_.start(); }
    void reset_timer() { timer_.reset(); }
private:
    // time variables
    sf::Clock timer_;
    
    // text variables
    sf::Font timer_font_;
    std::unique_ptr<sf::Text> timer_text_;
};
