#include "Level.h"

level::level(const std::shared_ptr<sf::RenderWindow>& win, const std::shared_ptr<sf::View>& v)
{
    window_ref_ = win;
    view_ref_ = v;
    player_ = std::make_unique<Player>(win, v);
    ground_ = std::make_unique<Scenery>(win, v);
    
    // testing velocity works
    //ground_->set_velocity(-50.0f, 0.0f);
}

void level::handle_input(float dt)
{
    player_->handle_input(dt);
}

void level::update(float dt)
{
    player_->update(dt);
    //ground_->update(dt);
    
    // collision tests
    sf::Vector2f mtv;
    if (sat_detection::sat_collision(*player_, *ground_, mtv))
    {
        player_->collision_response(ground_.get(), mtv);
    }
}

void level::render()
{
    if (!window_ref_.expired())
    {
        std::shared_ptr<sf::RenderWindow> window = window_ref_.lock();
        window->draw(*player_);
        window->draw(*ground_);
        
        window.reset();
    }
    
   
}
