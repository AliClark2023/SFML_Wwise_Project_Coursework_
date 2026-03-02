#include "Player.h"

Player::Player(const std::shared_ptr<sf::RenderWindow>& win, const std::shared_ptr<sf::View>& v)
{
    set_window(win);
    set_view(v);
    setPointCount(4);
    setRadius(25.f);
    setOrigin(sf::Vector2f(getRadius(), getRadius()));
    setRotation(sf::degrees(45));
    setPosition(win->getView().getCenter());
    setFillColor(sf::Color::Yellow);
    
    //physics Initialisation
    //gravity
    gravity_ = sf::Vector2f(0, 9.8f) * VELOCITY_SCALE;
    //jumping velocity
    jump_vector_ = sf::Vector2f(0, -4.0f) * VELOCITY_SCALE;
    is_jumping_ = false;
    //movement
    velocity_ = sf::Vector2f(2.f, 0) * VELOCITY_SCALE;
}

Player::~Player()
{
}

void Player::handle_input(float dt)
{
    if (!window_ref_.expired())
    {
        std::shared_ptr<sf::RenderWindow> window = window_ref_.lock();
        if(window->hasFocus())
        {
            //jumping
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !is_key_held_) {
                if (!is_jumping_) {
                    /*
                    y_velocity_ = jump_vector_;
                    is_jumping_ = true;
                    is_key_held_ = true;
                    //jump sound
                    */
                }
                
                // testing jumping 
                y_velocity_ = jump_vector_;
                is_jumping_ = true;
                is_key_held_ = true;
            }
            else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && is_key_held_) {
                is_key_held_ = false;
            }
        }
        
        window.reset();
    }
    
}

void Player::update(float dt)
{
    ////Updating gravity
    //s=ut + 1/2at^2
    sf::Vector2f pos = y_velocity_ * dt + 0.5f * gravity_ * dt * dt;
    // v = u +at note its += not =
    y_velocity_ += gravity_ * dt;
    setPosition(getPosition() + pos);
    
}

void Player::collision_response(GameObject* collider)
{
    
}
