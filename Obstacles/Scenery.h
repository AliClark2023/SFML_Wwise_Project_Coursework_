#pragma once
#include "../BaseClasses/GameObject.h"
#include "../Constants/PhysicsConsts.h"

// make general to use between obstacle types
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
    ObjectType type = ObjectType::scenery;
};

// Handles platforms the player can land on top of
class Scenery : public GameObject
{
public:
    Scenery(const std::shared_ptr<sf::RenderWindow>& win, const std::shared_ptr<sf::View>& v);
    Scenery(const std::shared_ptr<sf::RenderWindow>& win, const std::shared_ptr<sf::View>& v, const scenery_config& config);
    virtual ~Scenery();
    
    // move to obstacle class
    bool has_been_counted() const { return counted_; }
    void set_counted(const bool b) { counted_ = b; }
    bool has_been_activated() const { return activated_; }
    void set_activated(const bool b) { activated_ = b; }
    
    void handle_input(float dt) override;
    void update(float dt) override;
    void collision_response(GameObject* collider, const sf::Vector2f& mtv) override;

private:
    // can add in obstacle only flags (counted, activated) from base game object
    bool counted_ = false;
    bool activated_ = false;
};
