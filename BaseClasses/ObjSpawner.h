#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "../Obstacles/Scenery.h"
/*
 * Base Class that spawns specified object type at a specified rate & speed
 */
class object_spawner : public GameObject
{
public:
    object_spawner(const std::shared_ptr<sf::RenderWindow>& win, const std::shared_ptr<sf::View>& v,
        const ObjectType& object_to_spawn);
    
    void spawn_object();
    void set_spawn_rate(float);
    void set_object_speed(float);
    void set_object_speed(sf::Vector2f vel);
    void render_objects() const;
    // revert/remove
    void handle_input(float dt) override;
    void update(float dt) override;
    void collision_response(GameObject* collider, const sf::Vector2f& mtv) override;
private:
    ObjectType type_to_spawn_;
    float elapsed_time_ = 0.f;
    float spawn_rate_ = 0.5f;
    float object_speed_ = 300.0f;
    sf::Vector2f object_position_;
    std::vector<std::unique_ptr<GameObject>> objects_;
};
