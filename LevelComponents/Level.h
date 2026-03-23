#pragma once
#include "Score.h"
#include "../Player/Player.h"
#include "../Obstacles/Scenery.h"
#include "../BaseClasses/ObjSpawner.h"
#include "../BaseClasses/GameObject.h"
#include "../LevelComponents/Timer.h"
#include "../WwiseWrapper.h"
#include "../Utilities/SATDetection.h"

class level
{
public:
    level(const std::shared_ptr<sf::RenderWindow>& win, const std::shared_ptr<sf::View>& v);
    void handle_input(float dt);
    void update(float dt);
    void render();
    
private:
    std::weak_ptr<sf::RenderWindow> window_ref_;
    std::weak_ptr<sf::View> view_ref_;
    
    std::unique_ptr<Player> player_;
    std::unique_ptr<Scenery> ground_;
   std::vector<std::unique_ptr<GameObject>> obstacles_;
    
    /*
     * spawners
     */
    std::unique_ptr<object_spawner> scene_spawner_;
    std::unique_ptr<object_spawner> hazard_spawner_;
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
};
