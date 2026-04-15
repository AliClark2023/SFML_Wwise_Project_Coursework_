#include "Level.h"

// debug mode
//#include "../Constants/DebugMode.h"

level::level( sf::RenderWindow& win,  sf::View& v) : window_ref_(win), view_ref_(v)
{
    player_ = std::make_unique<Player>(win, v);
    ground_ = std::make_unique<Scenery>(win, v);
    
    // Audio initialisation (create function)
    // registering objects used within level
    AK::SoundEngine::RegisterGameObj(EVT_PLAY_BG_MUSIC.ID);
    AK::SoundEngine::RegisterGameObj(EVT_CHANGE_TO_UP_BEAT.ID);
    AK::SoundEngine::RegisterGameObj(EVT_INTENSITY.ID);
    
    // starting music
    AK::SoundEngine::PostEvent(EVT_PLAY_BG_MUSIC.EventName.data(), EVT_PLAY_BG_MUSIC.ID);
    
    
    //initializing spawn zone
    spawn_zone_min_ = view_ref_.getCenter();
    spawn_zone_min_.x = spawn_zone_min_.x + 500.f;
    spawn_zone_min_.y = spawn_zone_min_.y - 50.f;
    spawn_zone_max_ = sf::Vector2f(0.0f, 0.0f);
    
    setup_spawners();
    
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
}

void level::update(float dt)
{
    // component updates
    player_->update(dt);
    ground_->update(dt);
    // make function/calculation to account for multiple spawners
    score_.add_to_score(update_score());
    
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
    // UI elements first
    timer_.render_timer(window_ref_);
    render_spawners();
    score_.render_score(window_ref_);
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

// handles music triggers within the level
void level::update_audio()
{
    const float score = static_cast<float>(score_.get_score());
    float new_audio_intensity = 0.f;
    
    // change state (apply to whole level)
    if (level_state_ != level_state::high && score >= HIGH_STATE_SCORE_THRESHOLD)
    {
        level_state_ = level_state::high;
        //need to reset after state switching
        prev_intensity = 0;
        /*
        // testing
        // coallate and create game objects in audio header file
        const uint64_t gameObjectId = 2;
        AK::SoundEngine::RegisterGameObj(gameObjectId);
        */
        
        //AK::SoundEngine::PostEvent(AKTEXT("Change_To_Upbeat"), gameObjectId);
        AK::SoundEngine::PostEvent(EVT_CHANGE_TO_UP_BEAT.EventName.data(), EVT_CHANGE_TO_UP_BEAT.ID);
    }
    
    // handles how audio intensity is handled depending on level state
    switch (level_state_)
    {
        case level_state::slow:
            new_audio_intensity = std::clamp(score * AUDIO_INTENSITY_MULTIPLIER, START_AUDIO_INTENSITY, MAX_AUDIO_INTENSITY);
            break;
        // high state
        default:
            new_audio_intensity = std::clamp((score - HIGH_STATE_SCORE_THRESHOLD) * AUDIO_INTENSITY_MULTIPLIER, START_AUDIO_INTENSITY, MAX_AUDIO_INTENSITY);
            break;
    }
    
    //if ( !(prev_intensity <= new_audio_intensity && AK::SoundEngine::SetRTPCValue("Intensity", new_audio_intensity)))
    if ( prev_intensity <= new_audio_intensity)
    {
        const AKRESULT result = AK::SoundEngine::SetRTPCValue(EVT_INTENSITY.EventName.data(), new_audio_intensity);
        
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
