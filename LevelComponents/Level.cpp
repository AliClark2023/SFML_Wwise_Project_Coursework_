#include "Level.h"

level::level(const std::shared_ptr<sf::RenderWindow>& win, const std::shared_ptr<sf::View>& v)
{
    window_ref_ = win;
    view_ref_ = v;
    player_ = std::make_unique<Player>(win, v);
    ground_ = std::make_unique<Scenery>(win, v);
    
    //initializing spawn zone
    spawn_zone_min_ = v->getCenter();
    spawn_zone_min_.x = spawn_zone_min_.x + 500.f;
    spawn_zone_min_.y = spawn_zone_min_.y + 100.f;
    spawn_zone_max_ = sf::Vector2f(0.0f, 0.0f);
    
    // testing velocity works
    //ground_->set_velocity(-50.0f, 0.0f);

    // (need to update origin after altering radius)
    obstacles_.emplace_back(std::make_unique<Scenery>(win, v));
    obstacles_[0]->setPosition(sf::Vector2f(spawn_zone_min_));
    obstacles_[0]->setRadius(20);
    obstacles_[0]->setOrigin(sf::Vector2f( obstacles_[0]->getRadius(),  obstacles_[0]->getRadius()));
    obstacles_[0]->setFillColor(sf::Color::Cyan);
    obstacles_[0]->set_velocity(-object_speed_, 0);
    //obstacles_[0]->set_velocity(-object_speed_,0);
}

void level::handle_input(float dt)
{
    player_->handle_input(dt);
}

void level::update(float dt)
{
    // object updates
    player_->update(dt);
    ground_->update(dt);
    for (const auto& obstacle : obstacles_)
    {
        obstacle->update(dt);
    }
    
    // collision tests
    // need pointer check
    for (const auto& obstacle : obstacles_)
    {
        if (obstacle->get_object_type() != scenery) continue;
        
        sf::Vector2f mtv;
        if (sat_detection::sat_collision(*player_, *obstacle, mtv))
        {
            player_->collision_response(obstacle.get(), mtv);
        }
    }
    
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
        
        for (const auto& obstacle : obstacles_) window->draw(*obstacle);
        
        window->draw(*ground_);
        
        window.reset();
    }
    
   
}
