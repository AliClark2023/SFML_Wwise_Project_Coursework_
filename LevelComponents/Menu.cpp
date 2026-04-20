#include "Menu.h"

menu_UI::menu_UI( sf::RenderWindow& win,  sf::View& v) : window_ref_(win), view_ref_(v)
{
    // replace with desired font (note: the directory is vital to loading correctly)
    const std::string font_name = "Assets/Fonts/Montserrat-Regular.ttf";
    FileLoading::load_font(font_, font_name);
    
    // text pos offsets
    sf::Vector2f pause_pos {view_ref_.getCenter().x -100,view_ref_.getCenter().y -100};
    sf::Vector2f reset_pos {view_ref_.getCenter().x -300,view_ref_.getCenter().y};
    sf::Vector2f resume_pos {view_ref_.getCenter().x -100,view_ref_.getCenter().y};
    sf::Vector2f quit_pos {view_ref_.getCenter().x + 150,view_ref_.getCenter().y};
    
    // Text initialisation
    paused_text_ = std::make_unique<sf::Text>(font_);
    paused_text_->setString("Level Paused ");
    paused_text_->setPosition(pause_pos);
    
    reset_text_ = std::make_unique<sf::Text>(font_);
    reset_text_->setString("Reset ( R ) ");
    reset_text_->setPosition(reset_pos);
    
    resume_text_ = std::make_unique<sf::Text>(font_);
    resume_text_->setString("Resume ( P ) ");
    resume_text_->setPosition(resume_pos);
    
    quit_text_ = std::make_unique<sf::Text>(font_);
    quit_text_->setString("Quit ( Q ) ");
    quit_text_->setPosition(quit_pos);
}

void menu_UI::handle_input(float dt)
{
    while (const std::optional event = window_ref_.pollEvent())
    {
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            // pause detection
            if (!active_)
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::P)
                {
                    active_ = true;
                }
            }// pause handling
            else
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::P)
                {
                    // return to level
                    active_ = false;
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::R)
                {
                    //reset level
                    reset_level_ = true;
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::Q)
                {
                    //quit game
                    window_ref_.close();
                }
            }
        }
    }
    
}

void menu_UI::render() const
{
    if (!active_) return;
    
    window_ref_.draw(*paused_text_);
    window_ref_.draw(*reset_text_);
    window_ref_.draw(*resume_text_);
    window_ref_.draw(*quit_text_);
}
