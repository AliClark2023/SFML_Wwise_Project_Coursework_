#include "Player.h"

#include <algorithm>

Player::Player(sf::RenderWindow& win, sf::View& v):  GameObject(win, v)
{

    setPointCount(4);
    setRadius(25.f);
    setOrigin(sf::Vector2f(getRadius(), getRadius()));
    setRotation(sf::degrees(45));
    spawn_point = sf::Vector2f(win.getView().getCenter().x, win.getView().getCenter().y - 200.f);
    setPosition(spawn_point);
    setFillColor(sf::Color::Yellow);
    
    //physics Initialisation
    //gravity
    gravity_ = sf::Vector2f(0, 9.8f) * PLAYER_V_SCALE;
    //jumping velocity
    jump_vector_ = sf::Vector2f(0, -4.0f) * PLAYER_V_SCALE;
    is_jumping_ = false;
    //movement
    velocity_ = sf::Vector2f(2.f, 0) * PLAYER_V_SCALE;
}

Player::~Player()
{
}

void Player::handle_input(float dt)
{
    
    if(window_ref_.hasFocus())
    {
        //jumping
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !is_key_held_) {
            
            // disabled to allow multiple jumps in air
            if (!is_jumping_) {
                /*
                y_velocity_ = jump_vector_;
                is_jumping_ = true;
                is_key_held_ = true;
                //jump sound
                */
            }
            
            y_velocity_ = jump_vector_;
            is_jumping_ = true;
            is_key_held_ = true;
            is_on_ground_ = false;
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && is_key_held_) {
            is_key_held_ = false;
        }
        // side ways movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            velocity_.x = 2 * PLAYER_V_SCALE;
            move(-velocity_ * dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            velocity_.x = 2 * PLAYER_V_SCALE;
            move(velocity_ * dt);
        }
    }
}

void Player::update(float dt)
{
    //Updating gravity
    
    if (is_on_ground_) y_velocity_ = sf::Vector2f(0, 0);
    sf::Vector2f pos = y_velocity_ * dt + 0.5f * gravity_ * dt * dt;
    y_velocity_ += gravity_ * dt;
    setPosition(getPosition() + pos);
    //required to constantly apply gravity when a collision is not occuring
    if (is_on_ground_) is_on_ground_ = false;
    
    float window_width = window_ref_.getView().getSize().x;
    float window_height = window_ref_.getView().getSize().y;
    sf::Vector2f window_centre = window_ref_.getView().getCenter();
    
    /*
     * Window/view boundaries
     */
    // left border
    if (getPosition().x < window_centre.x - (window_width / 2))
    {
        setPosition(sf::Vector2f(window_centre.x -(window_width / 2), getPosition().y));
    }
    // right border
    if (getPosition().x > window_ref_.getView().getCenter().x + (window_width / 2))
    {
        setPosition(sf::Vector2f(window_centre.x  + (window_width / 2), getPosition().y));
    }
    // top border
    if (getPosition().y < window_centre.y - (window_height / 2))
    {
        setPosition(sf::Vector2f(getPosition().x, window_centre.y - (window_height / 2)));
    }
}

// will handle collision responses according to their type
void Player::collision_response(GameObject* collider, const sf::Vector2f& mtv)
{
    //used as validation
    if (!collider) return;
    if (collider->get_object_type() == scenery)
    {
        // moves player out of collision
        setPosition(getPosition() + mtv);
        
        // adjusts velocity depending on collision side
        float align = mtv.x * UP.x + mtv.y * UP.y;
        //landing from above
        if (align > 0.5f)
        {
            y_velocity_.y = 0;
            is_on_ground_ = true;
        }// underside of object
        else if (align < -0.5f)
        {
            is_on_ground_ = false;
            y_velocity_.y = std::max(y_velocity_.y, 0.f);
        }else
        {
            is_on_ground_ = false;
            velocity_.x = 0.f;
        }
    }else if (collider->get_object_type() == hazard)
    {
        setPosition(spawn_point);
    }
}
