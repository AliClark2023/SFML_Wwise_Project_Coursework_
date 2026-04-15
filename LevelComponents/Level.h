#pragma once
#include "Score.h"
#include "../Player/Player.h"
#include "../Obstacles/Scenery.h"
#include "../BaseClasses/ObjSpawner.h"
#include "../BaseClasses/GameObject.h"
#include "../LevelComponents/Timer.h"
#include "../Constants/ObstaclesConsts.h"
#include "../WwiseWrapper.h"
#include "../Utilities/SATDetection.h"

class level
{
public:
    level( sf::RenderWindow& win,  sf::View& v);
    void handle_input(float dt);
    void update(float dt);
    void render();

private:
    void setup_spawners();
    void update_spawners(const float& dt);
    void render_spawners();
    void spawner_collisions();
    
    int update_score() const;
    void update_audio();
    
    sf::RenderWindow& window_ref_;
    sf::View& view_ref_;
    
    std::unique_ptr<Player> player_;
    std::unique_ptr<Scenery> ground_;
    
    // remove after testing is done
    std::vector<std::unique_ptr<GameObject>> obstacles_;
    
    std::vector<std::unique_ptr<object_spawner>> spawners_;
    /*
     * spawned object parameters
     */
    // speed variable to adjust all spawned objects by
    float object_speed_ = 300.0f;
    sf::Vector2f spawn_zone_min_ = sf::Vector2f(0.0f, 0.0f);
    sf::Vector2f spawn_zone_max_ = sf::Vector2f(0.0f, 0.0f);
    int object_count_ = 0;
    
    // level UI components
    timer timer_;
    score score_;
    
    // testing
    float prev_intensity = 0.0f;
    
    enum class level_state { slow, high };
    level_state level_state_ = level_state::slow;
};
