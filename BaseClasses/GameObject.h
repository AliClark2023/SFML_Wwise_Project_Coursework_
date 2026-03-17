#pragma once
#include "SFML\Graphics.hpp"
#include <memory>

enum ObjectType
{
    player_controlled,
    scenery,
    hazard,
};

// uses circle class as base shape, can create other shape types from this class (triangles, squares etc)
class GameObject : public sf::CircleShape
{
public:
    GameObject();
    ~GameObject() override {}
    
    virtual void handle_input(float dt) = 0;
    virtual void update(float dt) = 0;
    
    // Control object speed and direction
    void set_velocity(const sf::Vector2f& vel) {velocity_ = vel;}
    void set_velocity(const float& vx, const float& vy){velocity_.x = vx;velocity_.y = vy;}
    sf::Vector2f get_velocity() const {return velocity_;}
    
    // Object states
    bool is_alive() const { return alive_; }
    void set_alive(const bool b) { alive_ = b; }
    /*
    // move to obstacle class
    bool has_been_counted() const { return counted_; }
    void set_counted(const bool b) { counted_ = b; }
    bool has_been_activated() const { return activated_; }
    void set_activated(const bool b) { activated_ = b; }
    */
    // For Object collision, set collider box, get collider box, and dedicated virtual function for any collision responses
    bool is_collider() const { return collider_; }
    void set_collider(bool b) { collider_ = b; }
    sf::FloatRect get_collision_box() const { return collision_box_; }
    void set_collision_box(float x, float y, float width, float height) { collision_box_ = sf::FloatRect({ x, y }, { width, height }); }
    void set_collision_box(const sf::FloatRect fr) { collision_box_ = fr; }
    virtual void collision_response(GameObject* collider, const sf::Vector2f& mtv) = 0;
    
    // Set game components 
    void set_window(const std::shared_ptr<sf::RenderWindow>& win) { window_ref_ = win; }
    void set_view(const std::shared_ptr<sf::View>& v) { view_ref_ = v; }
    ObjectType get_object_type() const { return object_type_; }
protected:
    // properties
    sf::Vector2f velocity_{0.0f,0.0f };
    
    // flags to determine state of object
    bool alive_ = true;
    /*
    // move to obstacle class
    bool counted_ = false;
    bool activated_ = false;
    */
    
    ObjectType object_type_;
    
    // collision variables
    // not currently used (but could be used to define smaller/larger collision area)
    sf::FloatRect collision_box_;
    bool collider_;
    
    std::weak_ptr<sf::RenderWindow> window_ref_;
    std::weak_ptr<sf::View> view_ref_;
};
