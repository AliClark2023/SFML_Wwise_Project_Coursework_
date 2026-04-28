#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "GameObject.h"
#include "../Utilities/SATDetection.h"
#include "../Player/Player.h"
#include "../Obstacles/Scenery.h"
#include "../Constants/AudioObjects.h"
#include "../LevelComponents/AudioManager.h"
/*
 * Spawner that instantiates specified object type at a specified rate & speed
 * Can update spawned objects characteristics 
 */
class object_spawner : public GameObject
{
public:
    object_spawner(sf::RenderWindow& win, sf::View& v);
    
    void spawn_object();
    
    /* Setting spawner properties
     */
    void set_spawn_rate(const float rate){spawn_rate_ = rate;}
 
    /* Spawned object properties
     */
    void set_start_speed(const float speed){ start_speed_ = speed; set_object_speed(start_speed_); }
    void set_hazard_chance(const int chance){hazard_chance = chance;}
    void set_object_speed(const float speed){ object_speed_ = speed; }
    // setting audio events
    void set_hazard_sfx (const AudioObject& event) { hazard_triggered_sfx = event; }
    void set_plat_sfx (const AudioObject& event) { plat_triggered_sfx_ = event; }
    float get_start_speed() const { return start_speed_; }
    // handles were to despawn, instantiated objects
    void set_despawn_threshold(const sf::Vector2f& despawn_pos){ despawn_threshold = despawn_pos; }
    
    int get_objects_scored();
    
    // requires use of SAT utility suite
    void detect_collision(const std::unique_ptr<Player>& player);
    void update(float dt) override;
    void render_objects() const;
    
    
    // not in use but required to be defined
    void handle_input(float dt) override;
    void collision_response(GameObject* collider, const sf::Vector2f& mtv) override;
    
private:
    float elapsed_time_ = 0.f;
    float spawn_rate_ = 0.5f;
    float object_speed_ = 0.f;
    float start_speed_ = 0.f;
    
    // chance of making object a hazard
    int hazard_chance = 0;
    
    sf::Vector2f score_threshold_pos_;
    sf::Vector2f despawn_threshold;
    
    int objects_scored_ = 0;
    std::vector<std::unique_ptr<Scenery>> objects_;
    
    // flags for determining if variable has been altered
    bool increased_speed_ = false;
    bool increases_spawn_rate_ = false;
    //Object triggered SFX Event & ID
    AudioObject hazard_triggered_sfx;
    AudioObject plat_triggered_sfx_;
};
