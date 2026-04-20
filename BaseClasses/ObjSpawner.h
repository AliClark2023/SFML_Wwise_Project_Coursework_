#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "GameObject.h"
#include "../Utilities/SATDetection.h"
#include "../Player/Player.h"
#include "../Obstacles/Scenery.h"
#include "../Constants/AudioObjects.h"
/*
 * Base Class that spawns specified object type at a specified rate & speed
 */
class object_spawner : public GameObject
{
public:
    object_spawner(sf::RenderWindow& win, sf::View& v,
        const ObjectType& object_to_spawn, const AudioTrigger& event);
    
    void spawn_object();
    // directly set spawn & speed
    void set_spawn_rate(const float rate){spawn_rate_ = rate;}
    //called when initialising spawner
    void set_start_spawn_rate(const float rate){start_spawn_rate_ = rate; set_spawn_rate(start_spawn_rate_);}
    void set_start_speed(const float speed){ start_speed_ = speed; set_object_speed(start_speed_); }
    
    float get_start_spawn_rate() const {return start_spawn_rate_;}
    void set_object_speed(const float speed){ object_speed_ = speed; }
    
    float get_start_speed() const { return start_speed_; }
    // calculation function to set spawn rate & speed (add parameters for timer)
    void update_object_speed(const int& score, const float& time);
    void update_spawn_rate(const int& score, const float& time);
    
    void render_objects() const;
    void detect_collision(const std::unique_ptr<Player>& player);
    void set_score_threshold(const sf::Vector2f& threshold_pos){ score_threshold_pos_ = threshold_pos; }
    void set_despawn_threshold(const sf::Vector2f& despawn_pos){ despawn_threshold = despawn_pos; }
    int get_objects_scored();
    // revert/remove
    void handle_input(float dt) override;
    void update(float dt) override;
    void collision_response(GameObject* collider, const sf::Vector2f& mtv) override;
private:
    //ObjectType type_to_spawn_;
    float elapsed_time_ = 0.f;
    float spawn_rate_ = 0.5f;
    float start_spawn_rate_ = 0.f;
    float object_speed_ = 0.f;
    float start_speed_ = 0.f;
    
    // change to x pos only ?
    sf::Vector2f score_threshold_pos_;
    sf::Vector2f despawn_threshold;
    
    int objects_scored_ = 0;
    // change to obstacle/scenery class
    std::vector<std::unique_ptr<Scenery>> objects_;
    
    // flags for determining if variable has been altered
    bool increased_speed_ = false;
    bool increases_spawn_rate_ = false;
    //Object triggered SFX Event & ID
    AudioTrigger object_triggered_sfx_;
};
