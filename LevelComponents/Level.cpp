#include "Level.h"

// debug mode
//#include "../Constants/DebugMode.h"

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
    
#ifdef DEBUGMODE
    // testing obstacles
    
    scenery_config obstacle_config;
    obstacle_config.point_count = 4;
    obstacle_config.radius = 80;
    obstacle_config.origin = sf::Vector2f( obstacle_config.radius,  obstacle_config.radius);
    obstacle_config.rotation = 0;
    obstacle_config.position = spawn_zone_min_;
    obstacle_config.position = v->getCenter();
   // obstacle_config.velocity = sf::Vector2f(-object_speed_, 0);
    obstacle_config.color = sf::Color::Cyan;
    obstacles_.emplace_back(std::make_unique<Scenery>(win, v, obstacle_config));
    
#endif
    
#ifndef DEBUGMODE
    // spawner config
    // add these to constants (make unique pos for different spawners)
    sf::Vector2f plat_spawn_pos = sf::Vector2f(v->getCenter().x + v->getSize().x, v->getCenter().y);
    sf::Vector2f hazard_spawn_pos = sf::Vector2f(v->getCenter().x + v->getSize().x, v->getCenter().y + 50);
    sf::Vector2f despawn_pos = sf::Vector2f(v->getCenter().x - v->getSize().x, v->getCenter().y);
    
    scene_spawner_ = std::make_unique<object_spawner>(win, v, scenery);
    scene_spawner_->setPosition(plat_spawn_pos);
    scene_spawner_->set_spawn_rate(1.f);
    scene_spawner_->set_score_threshold(v->getCenter());
    scene_spawner_->set_despawn_threshold(despawn_pos);
    
    hazard_spawner_ = std::make_unique<object_spawner>(win, v, hazard);
    hazard_spawner_->setPosition(hazard_spawn_pos);
    hazard_spawner_->set_spawn_rate(0.5f);
    hazard_spawner_->set_score_threshold(v->getCenter());
    hazard_spawner_->set_despawn_threshold(despawn_pos);
#endif
    
    //timer attributes (debug only, comment out in release)
    timer_.get_text()->setString("Time: 0.00");
    timer_.get_text()->setOrigin(timer_.get_text()->getGlobalBounds().size / 2.0f);
    timer_.get_text()->setFillColor(sf::Color::White);
    timer_.get_text()->setOutlineThickness(1.0f);
    timer_.get_text()->setPosition(sf::Vector2(v->getCenter().x, v->getCenter().y - v->getSize().y / 2 + 20));
    
    // score
    // position sf::Vector2(v->getCenter().x - v->getSize().x /2 + 125, v->getCenter().y - v->getSize().y / 2 + 20));
    score_.get_text()->setOrigin(score_.get_text()->getGlobalBounds().size / 2.0f);
    score_.get_text()->setFillColor(sf::Color::White);
    score_.get_text()->setOutlineThickness(1.0f);
    score_.get_text()->setPosition(sf::Vector2(v->getCenter().x - v->getSize().x /2 + 50, v->getCenter().y - v->getSize().y / 2 + 10));
}

void level::handle_input(float dt)
{
    player_->handle_input(dt);
    
#ifdef  DEBUGMODE
    // testing
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) score_.add_to_score(1);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) score_.sub_from_score(1);
#endif
}

void level::update(float dt)
{
    // component updates
    player_->update(dt);
    ground_->update(dt);
    // need to account for multiple spawners (make function)
    score_.add_to_score(scene_spawner_->get_objects_scored());
    // adjusting object speeds based on score (make function)
    scene_spawner_->update_object_speed( score_.get_score(),timer_.get_time().asSeconds());
    scene_spawner_->update_spawn_rate( score_.get_score(), timer_.get_time().asSeconds());
    
#ifndef DEBUGMODE
    scene_spawner_->update(dt);
    hazard_spawner_->update(dt);
#endif
    
#ifdef DEBUGMODE
    
    for (const auto& obstacle : obstacles_)
    {
        obstacle->update(dt);
    }
    
    // Testing collision with object
    for (const auto& obstacle : obstacles_)
    {
        // change to reset player 
        if (obstacle->get_object_type() != scenery) continue;
        
        sf::Vector2f mtv2;
        if (sat_detection::sat_collision(*player_, *obstacle, mtv2))
        {
            player_->collision_response(obstacle.get(), mtv2);
        }
    }
#endif
    
#ifndef DEBUGMODE
    // spawned obstacle collision
    scene_spawner_->detect_collision(player_);
    hazard_spawner_->detect_collision(player_);
#endif
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
#ifndef DEBUGMODE
        scene_spawner_->render_objects();
        hazard_spawner_->render_objects();
#endif
        score_.render_score(*window);
#ifdef DEBUGMODE
        for (const auto& obstacle : obstacles_) window->draw(*obstacle);
#endif
        window->draw(*player_);
        window->draw(*ground_);
        
        window.reset();
    }
    
   
}
