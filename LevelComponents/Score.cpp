#include "Score.h"

// initialises score with default specified font, requires score text to be formatted in whatever class uses it
score::score()
{
    // replace with desired font (note: the directory is vital to loading correctly)
    const std::string font_name = "Assets/Fonts/Montserrat-Regular.ttf";
    FileLoading::load_font(score_font_, font_name);
    score_text_ = std::make_unique<sf::Text>(score_font_);
}

// used by menu_ui to pass through font type and set up score parameters
score::score(sf::Font& font)
{
    score_text_ = std::make_unique<sf::Text>(font);
    score_text_->setOrigin(score_text_->getGlobalBounds().size / 2.0f);
    score_text_->setFillColor(sf::Color::White);
    score_text_->setOutlineThickness(1.0f);
}

// returns text variable for formatting/adjusting
sf::Text* score::get_text() const
{
    return score_text_.get();
}

// formats score variable into UI to be displayed
void score::render_score(sf::RenderWindow& window) const
{
    std::string score_ui = "Score: " + std::to_string(score_);
    score_text_->setString(score_ui);
    window.draw(*score_text_);
}

void score::add_to_score(const int& val_to_add)
{
    score_ += val_to_add;
}

void score::sub_from_score(const int& val_to_sub)
{
    score_ -= val_to_sub;
}

