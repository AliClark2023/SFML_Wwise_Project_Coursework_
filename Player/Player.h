#pragma once
#include "../BaseClasses/GameObject.h"
#include "../Constants/PhysicsConsts.h"

// simple square sprite that the user can control
class Player : public GameObject
{
public:
    Player(const std::shared_ptr<sf::RenderWindow>& win, const std::shared_ptr<sf::View>& v);
    ~Player();
    
    void handle_input(float dt) override;
    void update(float dt) override;
    void collision_response(GameObject* collider) override;
private:
    //sf::RectangleShape render_shape_;
    sf::Vector2f gravity_;
    sf::Vector2f jump_vector_;
    sf::Vector2f y_velocity_;
    bool is_jumping_{ false };
    bool is_key_held_{ false };
    bool is_on_ground_{ false };
};
