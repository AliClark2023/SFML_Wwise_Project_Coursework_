#pragma once
#include "../BaseClasses/GameObject.h"

// handles platforms the player can land on top of

class Scenery : public GameObject
{
public:
    Scenery(const std::shared_ptr<sf::RenderWindow>& win, const std::shared_ptr<sf::View>& v);
    virtual ~Scenery();
    
    void handle_input(float dt) override;
    void update(float dt) override;
    void collision_response(GameObject* collider, const sf::Vector2f& mtv) override;

private:
    //sf::Vector2f y_velocity_;
};
