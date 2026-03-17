#include "Timer.h"
#include <sstream>

// Constructs timer with specified font & starts timer
Timer::Timer()
{
    // replace with desired font (note: the directory is vital to loading correctly)
    std::string font_name = "Assets/Fonts/Montserrat-Regular.ttf";
    asset_utilities::LoadFont(timer_font_, font_name);
    timer_text_ = std::make_unique<sf::Text>(timer_font_);
    timer_.restart();
}
// converts clock timer to MM::SS and renders to window
void Timer::render_timer( sf::RenderWindow& window) const
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

sf::Text* Timer::get_text() const
{
    return timer_text_.get();
}
