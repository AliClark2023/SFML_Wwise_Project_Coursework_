#include "Menu.h"

menu_ui::menu_ui( sf::RenderWindow& win,  sf::View& v) : window_ref_(win), view_ref_(v)
{
    // replace with desired font (note: the directory is vital to loading correctly)
    const std::string font_name = "Assets/Fonts/Montserrat-Regular.ttf";
    FileLoading::load_font(font_, font_name);
    
    // UI positions
    const sf::Vector2f score_pos { view_ref_.getCenter().x - view_ref_.getSize().x /2 + 50, view_ref_.getCenter().y - view_ref_.getSize().y / 2 + 10};
    const sf::Vector2f timer_pos { view_ref_.getCenter().x, view_ref_.getCenter().y - view_ref_.getSize().y / 2 + 20};

    score_ui_ = std::make_unique<score>(font_);
    score_ui_->set_pos(score_pos);
    timer_ui_ = std::make_unique<timer>(font_);
    timer_ui_->set_pos(timer_pos);
    pause_ui_ = std::make_unique<pause_menu>(window_ref_, view_ref_, font_);
}

void menu_ui::handle_input(float dt)
{
    pause_ui_->handle_input(dt);
   
    // pause/resume timer
    if (pause_ui_->is_active() && timer_ui_->get_clock().isRunning()) timer_ui_->pause_timer();
    if (!pause_ui_->is_active() && !timer_ui_->get_clock().isRunning()) timer_ui_->resume_timer();
}

void menu_ui::render() const
{
    score_ui_->render_score(window_ref_);
    timer_ui_->render_timer(window_ref_);
    
    
    if (!pause_ui_->is_active()) return;
    
    pause_ui_->render();
    
    /*
    window_ref_.draw(*paused_text_);
    window_ref_.draw(*reset_text_);
    window_ref_.draw(*resume_text_);
    window_ref_.draw(*quit_text_);
    */
    
    
}
