#pragma once
#include "../BaseClasses/GameObject.h"
#include "../Constants/PhysicsConsts.h"

// comment function usage and parameters

// simple square sprite that the user can control
class Player : public GameObject
{
public:
    Player(sf::RenderWindow& win, sf::View& v);
    ~Player();
    
    void handle_input(float dt) override;
    void update(float dt) override;
    void collision_response(GameObject* collider, const sf::Vector2f& mtv) override;
    
    void set_spawn_point(const sf::Vector2f& spawnPoint){ spawn_point = spawnPoint; }
    sf::Vector2f get_spawn_point() const { return spawn_point; }
private:
    //sf::RectangleShape render_shape_;
    sf::Vector2f gravity_;
    sf::Vector2f jump_vector_;
    sf::Vector2f y_velocity_;
    sf::Vector2f spawn_point;
    bool is_jumping_{ false };
    bool is_key_held_{ false };
    bool is_on_ground_{ false };
};
