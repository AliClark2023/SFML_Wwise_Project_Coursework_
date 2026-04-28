#pragma once

#include "../WwiseWrapper.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include "../Utilities/FileLoading.h"

/*  Pause functionality component (used in conjunction with Menu UI)
 *  Handles Input and text display
 */
class pause_menu
{
public:
    pause_menu( sf::RenderWindow& win,  sf::View& v, const sf::Font& font);
    void handle_input(float dt);
    void render() const;
    
    bool is_active() const {return active_;}
    bool get_reset_level() const {return reset_level_;}
    void set_reset_level(const bool reset){ reset_level_ = reset; }
    void set_active(const bool active){ active_ = active; }
private:
    sf::RenderWindow& window_ref_;
    sf::View& view_ref_;
    
    // menu status
    bool active_ = false;
    //used as flag to reset current level, should change to enum to denote which level to reset
    bool reset_level_ = false;
    
    // text variables
    sf::Font font_;
    const unsigned int text_size_ = 20;
    std::unique_ptr<sf::Text> paused_text_;
    std::unique_ptr<sf::Text> reset_text_;
    std::unique_ptr<sf::Text> resume_text_;
    std::unique_ptr<sf::Text> quit_text_;
};
