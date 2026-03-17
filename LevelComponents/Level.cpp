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
    spawn_zone_min_.y = spawn_zone_min_.y - 50.f;
    spawn_zone_max_ = sf::Vector2f(0.0f, 0.0f);
    
    // testing obstacles
    scenery_config obstacle_config;
    obstacle_config.point_count = 4;
    obstacle_config.radius = 20;
    obstacle_config.origin = sf::Vector2f( obstacle_config.radius,  obstacle_config.radius);
    obstacle_config.rotation = 0;
    obstacle_config.position = spawn_zone_min_;
    obstacle_config.position = v->getCenter();
   // obstacle_config.velocity = sf::Vector2f(-object_speed_, 0);
    obstacle_config.color = sf::Color::Cyan;
    obstacles_.emplace_back(std::make_unique<Scenery>(win, v, obstacle_config));
    
    // spawner config
    scene_spawner_ = std::make_unique<object_spawner>(win, v, scenery);
    scene_spawner_->setPosition( v->getCenter());
    scene_spawner_->set_spawn_rate(1.f);
    
    //timer attributes (debug only, comment out in release)
    timer_.get_text()->setString("Time: 0.00");
    timer_.get_text()->setOrigin(timer_.get_text()->getGlobalBounds().size / 2.0f);
    timer_.get_text()->setFillColor(sf::Color::White);
    timer_.get_text()->setOutlineThickness(1.0f);
    timer_.get_text()->setPosition(sf::Vector2(v->getCenter().x, v->getCenter().y - v->getSize().y / 2 + 20));
    
    // score
    // position sf::Vector2(v->getCenter().x - v->getSize().x /2 + 125, v->getCenter().y - v->getSize().y / 2 + 20));
}

void level::handle_input(float dt)
{
    player_->handle_input(dt);
}

void level::update(float dt)
{
    //elapsed_time_ = clock_.getElapsedTime();
    // object updates
    player_->update(dt);
    ground_->update(dt);
    scene_spawner_->update(dt);
    for (const auto& obstacle : obstacles_)
    {
        obstacle->update(dt);
    }
    
    // collision tests
    // need pointer check
    for (const auto& obstacle : obstacles_)
    {
        if (obstacle->get_object_type() != scenery) continue;
        
        sf::Vector2f mtv2;
        if (sat_detection::sat_collision(*player_, *obstacle, mtv2))
        {
            player_->collision_response(obstacle.get(), mtv2);
        }
    }
    
    // spawned obstacle collision
    scene_spawner_->detect_collision(player_);
    
    // ground collision
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
        // UI elements first
        timer_.render_timer(*window);
        window->draw(*player_);
        scene_spawner_->render_objects();
        for (const auto& obstacle : obstacles_) window->draw(*obstacle);
        
        window->draw(*ground_);
        
        window.reset();
    }
    
   
}
