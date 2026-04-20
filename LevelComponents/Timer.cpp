#include "Timer.h"
#include <sstream>

// Constructs timer with specified font & starts timer
timer::timer()
{
    // replace with desired font (note: the directory is vital to loading correctly)
    const std::string font_name = "Assets/Fonts/Montserrat-Regular.ttf";
    FileLoading::load_font(timer_font_, font_name);
    timer_text_ = std::make_unique<sf::Text>(timer_font_);
    timer_.restart();
}

timer::timer(sf::Font& font)
{
    timer_text_ = std::make_unique<sf::Text>(font);
    timer_text_->setString("Time: 0.00");
    timer_text_->setOrigin(timer_text_->getGlobalBounds().size / 2.0f);
    timer_text_->setFillColor(sf::Color::White);
    timer_text_->setOutlineThickness(1.0f);
}

// converts clock timer to MM::SS and renders to window
void timer::render_timer(sf::RenderWindow& window) const
{
    
    const sf::Time elapsed = timer_.getElapsedTime();
    const int total_seconds = static_cast<int>(elapsed.asSeconds());
    const int minutes = total_seconds / 60;
    const int seconds = total_seconds % 60;
    
    std::stringstream ss;
    ss << "Time: "
       << std::setw(2) << std::setfill('0') << minutes
       << ":"
       << std::setw(2) << std::setfill('0') << seconds;
    
    timer_text_->setString(ss.str());
    window.draw(*timer_text_);
}
// returns timer text for formatting/adjusting
sf::Text* timer::get_text() const
{
    return timer_text_.get();
}
// returns current value of time
sf::Time timer::get_time() const
{
    return timer_.getElapsedTime();
}
