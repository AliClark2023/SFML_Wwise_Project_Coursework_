#pragma once
#include "../BaseClasses/GameObject.h"


struct scenery_config
{   // Needs to be in this precise order for collision to be accurate
    size_t point_count = 4;
    float radius = 1.0f;
    sf::Vector2f origin = sf::Vector2f(0.0f, 0.0f);
    // must be 0->360
    float rotation = 0.0f;
    sf::Vector2f position = sf::Vector2f(0.0f, 0.0f);
    sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f);
    sf::Color color = sf::Color::Black;
};

// Handles platforms the player can land on top of
class Scenery : public GameObject
{
public:
    Scenery(const std::shared_ptr<sf::RenderWindow>& win, const std::shared_ptr<sf::View>& v);
    Scenery(const std::shared_ptr<sf::RenderWindow>& win, const std::shared_ptr<sf::View>& v, const scenery_config& config);
    virtual ~Scenery();
    
    void handle_input(float dt) override;
    void update(float dt) override;
    void collision_response(GameObject* collider, const sf::Vector2f& mtv) override;

private:
    //sf::Vector2f y_velocity_;
};
