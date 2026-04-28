#pragma once
#include "SFML\Graphics.hpp"
#include <memory>

/*
 *  Names determining which type of object
 *  Ideally should be handled by derived classes but is fine for this small scale
 *  
 */
enum ObjectType
{
    player_controlled,
    spawner,
    scenery,
    hazard,
};

/* Base Class for all in game objects
 * Uses circle class as base shape, to create other shape types from this class (triangles, squares etc)
*/

class GameObject : public sf::CircleShape
{
public:
    GameObject( sf::RenderWindow& win,  sf::View& v);
    ~GameObject() override {}
    
    virtual void handle_input(float dt) = 0;
    virtual void update(float dt) = 0;
    
    void set_velocity(const sf::Vector2f& vel) {velocity_ = vel;}
    void set_velocity(const float& vx, const float& vy){velocity_.x = vx;velocity_.y = vy;}
    void set_speed(float speed) { speed_ = speed; }
    void set_alive(const bool b) { alive_ = b; }
    
    sf::Vector2f get_velocity() const {return velocity_;}
    float get_Speed() const {return speed_;}
    bool is_alive() const { return alive_; }
    
    // collision is handled by SAT utility suite and requires this functions to trigger responses & effects
    virtual void collision_response(GameObject* collider, const sf::Vector2f& mtv) = 0;
    
    ObjectType get_object_type() const { return object_type_; }
protected:
    // properties
    sf::Vector2f velocity_{0.0f,0.0f };
    float speed_{0.0f};
    
    // flags to determine state of object
    bool alive_ = true;
    ObjectType object_type_ = scenery;
    
    // Rendering references
    sf::RenderWindow& window_ref_;
    sf::View& view_ref_;
};
