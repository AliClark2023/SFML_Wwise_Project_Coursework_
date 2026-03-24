#include "Level.h"

// debug mode
//#include "../Constants/DebugMode.h"

level::level( sf::RenderWindow& win,  sf::View& v) : window_ref_(win), view_ref_(v)
{
    //window_ref_ = win;
    //view_ref_ = v;
    
    player_ = std::make_unique<Player>(win, v);
    ground_ = std::make_unique<Scenery>(win, v);
    
    //initializing spawn zone
    spawn_zone_min_ = view_ref_.getCenter();
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
    // spawner config (create function)
    setup_spawners();
    /*
    // add these to constants (make unique pos for different spawners)
    sf::Vector2f plat_spawn_pos = sf::Vector2f(view_ref_.getCenter().x + view_ref_.getSize().x, view_ref_.getCenter().y);
    sf::Vector2f hazard_spawn_pos = sf::Vector2f(view_ref_.getCenter().x + view_ref_.getSize().x, view_ref_.getCenter().y + 50);
    sf::Vector2f despawn_pos = sf::Vector2f(view_ref_.getCenter().x - view_ref_.getSize().x, view_ref_.getCenter().y);
    
    scene_spawner_ = std::make_unique<object_spawner>(window_ref_, view_ref_, scenery);
    scene_spawner_->setPosition(plat_spawn_pos);
    scene_spawner_->set_spawn_rate(START_SPWN_RATE);
    scene_spawner_->set_object_speed(START_OBJ_SPEED);
    scene_spawner_->set_score_threshold(view_ref_.getCenter());
    scene_spawner_->set_despawn_threshold(despawn_pos);
    
    hazard_spawner_ = std::make_unique<object_spawner>(win, v, hazard);
    hazard_spawner_->setPosition(hazard_spawn_pos);
    hazard_spawner_->set_spawn_rate(START_SPWN_RATE);
    hazard_spawner_->set_object_speed(START_OBJ_SPEED);
    hazard_spawner_->set_score_threshold(view_ref_.getCenter());
    hazard_spawner_->set_despawn_threshold(despawn_pos);
    */
#endif
    
    //timer attributes (debug only, comment out in release)
    timer_.get_text()->setString("Time: 0.00");
    timer_.get_text()->setOrigin(timer_.get_text()->getGlobalBounds().size / 2.0f);
    timer_.get_text()->setFillColor(sf::Color::White);
    timer_.get_text()->setOutlineThickness(1.0f);
    timer_.get_text()->setPosition(sf::Vector2(view_ref_.getCenter().x, view_ref_.getCenter().y - view_ref_.getSize().y / 2 + 20));
    
    // score
    // position sf::Vector2(v->getCenter().x - v->getSize().x /2 + 125, v->getCenter().y - v->getSize().y / 2 + 20));
    score_.get_text()->setOrigin(score_.get_text()->getGlobalBounds().size / 2.0f);
    score_.get_text()->setFillColor(sf::Color::White);
    score_.get_text()->setOutlineThickness(1.0f);
    score_.get_text()->setPosition(sf::Vector2(view_ref_.getCenter().x - view_ref_.getSize().x /2 + 50, view_ref_.getCenter().y - view_ref_.getSize().y / 2 + 10));
}

void level::handle_input(float dt)
{
    player_->handle_input(dt);
    
#ifdef  DEBUGMODE
    // testing
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) score_.add_to_score(1);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) score_.sub_from_score(1);
    bool updatedAudio = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
    {
        if (!updatedAudio)
        {
            if (AK::SoundEngine::SetRTPCValue("Intensity", 50.f))
            {
                std::cout << "Could not initialise Intensity value." << '\n';
            }
            updatedAudio = true;
        }
        
    }
    
#endif
}

void level::update(float dt)
{
    // component updates
    player_->update(dt);
    ground_->update(dt);
    // make function/calculation to account for multiple spawners
    score_.add_to_score(update_score());
    
    /*
    const float score = static_cast<float>(score_.get_score());
    // CLAMP THESE
    const float new_obj_speed = std::clamp((score * OBJ_SPEED_MULTIPLIER) + START_OBJ_SPEED, START_OBJ_SPEED, MAX_OBJ_SPEED);
    const float new_obj_spwn_rate = std::clamp( (-score * OBJ_SPAWN_MULTIPLIER) + START_SPWN_RATE, MAX_SPWN_RATE, START_SPWN_RATE);
    
    const float new_audio_intensity = std::clamp(score * AUDIO_INTENSITY_MULTIPLIER, START_AUDIO_INTENSITY, MAX_AUDIO_INTENSITY);
    
    // adjusting object speeds based on score (make function)
    scene_spawner_->set_object_speed(new_obj_speed);
    scene_spawner_->set_spawn_rate(new_obj_spwn_rate);
    //scene_spawner_->update_object_speed( score_.get_score(),timer_.get_time().asSeconds());
    //scene_spawner_->update_spawn_rate( score_.get_score(), timer_.get_time().asSeconds());
    
    // audio hook testing
    if ( !(prev_intensity <= new_audio_intensity && AK::SoundEngine::SetRTPCValue("Intensity", new_audio_intensity)))
    {
        std::cout << "Could not initialise Intensity value." << '\n';
    }
    prev_intensity = new_audio_intensity;
    
    */
    
    update_audio();
    
#ifndef DEBUGMODE
    update_spawners(dt);
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
    spawner_collisions();
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
    /*
    if (!window_ref_.expired())
    {
        std::shared_ptr<sf::RenderWindow> window = window_ref_.lock();
        
        
    }
    */
    
    // UI elements first
    timer_.render_timer(window_ref_);
#ifndef DEBUGMODE
    render_spawners();
#endif
    score_.render_score(window_ref_);
#ifdef DEBUGMODE
    for (const auto& obstacle : obstacles_) window->draw(*obstacle);
#endif
    window_ref_.draw(*player_);
    window_ref_.draw(*ground_);
    
}

// initialises all spawners and moves to container for easy alterations
void level::setup_spawners()
{
    // spawner config (create function)
    // add these to constants (make unique pos for different spawners)
    sf::Vector2f plat_spawn_pos = sf::Vector2f(view_ref_.getCenter().x + view_ref_.getSize().x, view_ref_.getCenter().y - 50);
    sf::Vector2f hazard_spawn_pos = sf::Vector2f(view_ref_.getCenter().x + view_ref_.getSize().x, view_ref_.getCenter().y );
    sf::Vector2f plat2_spawn_pos = sf::Vector2f(view_ref_.getCenter().x + view_ref_.getSize().x, view_ref_.getCenter().y + 50);
    sf::Vector2f despawn_pos = sf::Vector2f(view_ref_.getCenter().x - view_ref_.getSize().x, view_ref_.getCenter().y);
    
    std::unique_ptr<object_spawner> spawner;
    // initial scenery spawner # 1
    spawner = std::make_unique<object_spawner>(window_ref_, view_ref_, scenery);
    spawner->setPosition(plat_spawn_pos);
    spawner->set_start_spawn_rate(MIN_SPWN_RATE * 1.5);
    spawner->set_start_speed(MIN_OBJ_SPEED);
    spawner->set_score_threshold(view_ref_.getCenter());
    spawner->set_despawn_threshold(despawn_pos);
    spawners_.push_back(std::move(spawner));
    
    // initial hazard spawner
    spawner = std::make_unique<object_spawner>(window_ref_, view_ref_, hazard);
    spawner->setPosition(hazard_spawn_pos);
    spawner->set_start_spawn_rate(MIN_SPWN_RATE  * 2);
    //spawner->set_spawn_rate(MIN_SPWN_RATE  * 2);
    //spawner->set_object_speed(MIN_OBJ_SPEED);
    spawner->set_start_speed(MIN_OBJ_SPEED);
    spawner->set_score_threshold(view_ref_.getCenter());
    spawner->set_despawn_threshold(despawn_pos);
    spawners_.push_back(std::move(spawner));
    
    
    // initial scenery spawner # 2
    spawner = std::make_unique<object_spawner>(window_ref_, view_ref_, scenery);
    spawner->setPosition(plat2_spawn_pos);
    spawner->set_start_spawn_rate(MIN_SPWN_RATE * 0.75);
    spawner->set_start_speed(MIN_OBJ_SPEED);
    spawner->set_score_threshold(view_ref_.getCenter());
    spawner->set_despawn_threshold(despawn_pos);
    spawners_.push_back(std::move(spawner));
    
    
}

int level::update_score() const
{
    int score = 0;
    for (const auto& spawner : spawners_)
    {
        score += spawner->get_objects_scored();
    }
    return score;
}

void level::update_spawners(const float& dt)
{
    const float score = static_cast<float>(score_.get_score());
    // CLAMP THESE
    float new_obj_speed;
    float new_obj_spwn_rate;
 
    for (const auto& spawner : spawners_)
    {
        
        // seperate out speed & rate for scenery & hazards
        if (spawner->get_object_type() == scenery)
        {
            new_obj_speed = std::clamp((score * OBJ_SPEED_MULTIPLIER) + spawner->get_start_speed(), spawner->get_start_speed(), MAX_OBJ_SPEED);
            new_obj_spwn_rate = std::clamp( (-score * OBJ_SPAWN_MULTIPLIER) + spawner->get_start_spawn_rate(), MAX_SPWN_RATE, spawner->get_start_spawn_rate());
            
            spawner->set_object_speed(new_obj_speed);
            spawner->set_spawn_rate(new_obj_spwn_rate);
        }
        
        
        spawner->update(dt);
    }
}

void level::update_audio()
{
    const float score = static_cast<float>(score_.get_score());
    // CLAMP THESE
    const float new_audio_intensity = std::clamp(score * AUDIO_INTENSITY_MULTIPLIER, START_AUDIO_INTENSITY, MAX_AUDIO_INTENSITY);
    // audio hook testing
    if ( !(prev_intensity <= new_audio_intensity && AK::SoundEngine::SetRTPCValue("Intensity", new_audio_intensity)))
    {
        std::cout << "Could not initialise Intensity value." << '\n';
    }
    prev_intensity = new_audio_intensity;
}

void level::render_spawners()
{
    for (const auto& spawner : spawners_)
    {
        spawner->render_objects();
    }
}

void level::spawner_collisions()
{
    for (const auto& spawner : spawners_)
    {
        spawner->detect_collision(player_);
    }
}
