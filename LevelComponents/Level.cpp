#include "Level.h"

level::level( sf::RenderWindow& win,  sf::View& v, menu_ui& menu) : window_ref_(win), view_ref_(v), menu_ref_(menu)
{
    player_ = std::make_unique<Player>(win, v);
    ground_ = std::make_unique<Scenery>(win, v);
 
    // level specific audio events, acquiring IDs of events
    play_bg = AudioManager::instance().get_registered_object(play_music_event.data());
    plat_land = AudioManager::instance().register_object(plat_event.data(), play_music_event.data());
    hazard_hit = AudioManager::instance().register_object(hazard_event.data(), play_music_event.data());
    upbeat_change =AudioManager::instance().register_object(upbeat_event.data(), play_music_event.data());
    intensity_obj =AudioManager::instance().register_object(intensity_event.data(), play_music_event.data());
    
    // starting music
    AK::SoundEngine::PostEvent(play_bg.Name.data(), play_bg.ID);
    
    //initializing spawn zone
    spawn_zone_min_ = view_ref_.getCenter();
    spawn_zone_min_.x = spawn_zone_min_.x + 500.f;
    spawn_zone_min_.y = spawn_zone_min_.y - 50.f;
    spawn_zone_max_ = sf::Vector2f(0.0f, 0.0f);
    
    setup_spawners();
}

// handling player and UI input
void level::handle_input(float dt)
{
    if (!menu_ref_.is_pause_active()) player_->handle_input(dt);
    
    menu_ref_.handle_input(dt);
}

void level::update(float dt)
{
    if (menu_ref_.is_pause_active()) return;
    
    // component updates
    player_->update(dt);
    ground_->update(dt);
    menu_ref_.add_to_score(update_score());
    
    update_audio();
    update_spawners(dt);
    
    // spawned obstacle collision
    spawner_collisions();

    // ground collision
    sf::Vector2f mtv;
    if (sat_detection::sat_collision(*player_, *ground_, mtv))
    {
        player_->collision_response(ground_.get(), mtv);
    }
    
}

void level::render()
{
    render_spawners();
    window_ref_.draw(*player_);
    window_ref_.draw(*ground_);
}

// initialises all spawners and moves to container for easy alterations
void level::setup_spawners()
{
    // spawner config 
    sf::Vector2f plat_spawn_pos = sf::Vector2f(view_ref_.getCenter().x + view_ref_.getSize().x, view_ref_.getCenter().y - 50);
    sf::Vector2f hazard_spawn_pos = sf::Vector2f(view_ref_.getCenter().x + view_ref_.getSize().x, view_ref_.getCenter().y );
    sf::Vector2f plat2_spawn_pos = sf::Vector2f(view_ref_.getCenter().x + view_ref_.getSize().x, view_ref_.getCenter().y + 50);
    sf::Vector2f despawn_pos = sf::Vector2f(view_ref_.getCenter().x - view_ref_.getSize().x, view_ref_.getCenter().y);
    
    std::unique_ptr<object_spawner> spawner;
    
    // initial scenery spawner #1
    spawner = std::make_unique<object_spawner>(window_ref_, view_ref_);
    spawner->setPosition(plat_spawn_pos);
    spawner->set_start_speed(MIN_OBJ_SPEED);
    spawner->set_despawn_threshold(despawn_pos);
    spawner->set_hazard_chance(10);
    spawner->set_hazard_sfx(hazard_hit);
    spawner->set_plat_sfx(plat_land);
    spawners_.push_back(std::move(spawner));
    
    // initial hazard spawner (higher hazard chance)
    spawner = std::make_unique<object_spawner>(window_ref_, view_ref_);
    spawner->setPosition(hazard_spawn_pos);
    spawner->set_start_speed(MIN_OBJ_SPEED);
    spawner->set_despawn_threshold(despawn_pos);
    spawner->set_hazard_chance(50);
    spawner->set_hazard_sfx(hazard_hit);
    spawner->set_plat_sfx(plat_land);
    spawners_.push_back(std::move(spawner));
    
    
    // initial scenery spawner #2
    spawner = std::make_unique<object_spawner>(window_ref_, view_ref_);
    spawner->setPosition(plat2_spawn_pos);
    spawner->set_start_speed(MIN_OBJ_SPEED);
    spawner->set_despawn_threshold(despawn_pos);
    spawner->set_hazard_chance(5);
    spawner->set_hazard_sfx(hazard_hit);
    spawner->set_plat_sfx(plat_land);
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
    const float score = static_cast<float>(menu_ref_.get_score());
    // CLAMP THESE, now handled by curve calculation and min/max values in ObstaclesConst.h
    float new_obj_speed;
    float new_obj_spwn_rate;
    
    // will activate current spawner when reaching spawn rate threshold
    // spawning according to spawn rate
    spawn_elapsed_time_ += dt;
    // spawn rate calculations, similar to intensity calculation, uses slow -> fast curve (t2)
    float spawn_t = std::clamp(score / (HIGH_STATE_SCORE_THRESHOLD * 2), 0.f, 1.0f);
    new_obj_spwn_rate = MAX_SPAWN_RATE + (MIN_SPAWN_RATE - MAX_SPAWN_RATE) * (spawn_t * spawn_t);
    
    spawn_rate_= new_obj_spwn_rate;
    int max_num_spawners = static_cast<int>(spawners_.size()) - 1;
    
    if (spawn_elapsed_time_ >= spawn_rate_)
    {
        // new speed calculation, similar to intensity & spawn rate (slow -> fast curve)
        float  new_obj_speed_t = std::clamp(score / (HIGH_STATE_SCORE_THRESHOLD * 2), 0.f, 1.0f);
        new_obj_speed = MIN_OBJ_SPEED + (MAX_OBJ_SPEED - MIN_OBJ_SPEED) * (new_obj_speed_t * new_obj_speed_t);
        spawners_[spawner_selection]->set_object_speed(new_obj_speed);
        
        spawners_[spawner_selection]->spawn_object();
        
        // progresses through spawners ascending -> descending before reversing
        if (ascending)
        {
            spawner_selection ++;
            if (spawner_selection >= max_num_spawners) ascending = false;
        }else
        {
            spawner_selection --;
            if (spawner_selection <= 0) ascending = true;
        }
        spawner_selection = std::clamp( spawner_selection, 0 , max_num_spawners );
        
        
        spawn_elapsed_time_ = 0.0f;
    }
    
    // updating all spawners (and spawned objects)
    for (const auto& spawner : spawners_)
    {
        spawner->update(dt);
    }
}

// handles music triggers within the level
void level::update_audio()
{
    const float score = static_cast<float>(menu_ref_.get_score());
    float new_audio_intensity = 0.f;
    float intensity_t = 0;
    
    // change state (apply to whole level)
    if (level_state_ != level_state::high && score >= HIGH_STATE_SCORE_THRESHOLD)
    {
        level_state_ = level_state::high;
        //need to reset after state switching
        prev_intensity = 0;
        AK::SoundEngine::PostEvent(upbeat_change.Name.data(), upbeat_change.ID);
    }
    
    // handles how audio intensity is handled depending on level state
    switch (level_state_)
    {
        case level_state::slow:
            // using same smooth curve formula as spawn rate & speed
            intensity_t = std::clamp(score / HIGH_STATE_SCORE_THRESHOLD, 0.f, 1.0f);
            new_audio_intensity = START_AUDIO_INTENSITY + (MAX_AUDIO_INTENSITY - START_AUDIO_INTENSITY) * intensity_t;
            break;
        // high state
        default:
            intensity_t = std::clamp((score - HIGH_STATE_SCORE_THRESHOLD) / HIGH_STATE_SCORE_THRESHOLD, 0.f, 1.0f);
            new_audio_intensity = START_AUDIO_INTENSITY + (MAX_AUDIO_INTENSITY - START_AUDIO_INTENSITY) * intensity_t;
            break;
    }
    
    if ( prev_intensity <= new_audio_intensity)
    {

        const AKRESULT result = AK::SoundEngine::SetRTPCValue(intensity_obj.Name.data(), new_audio_intensity);
        
        if (result != AK_Success)
        {
            std::cout << "Could not initialise Intensity value." << '\n';
        }else
        {
            prev_intensity = new_audio_intensity;
        }
    }
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
