#include "Player.h"

Player::Player(const std::shared_ptr<sf::RenderWindow>& win, const std::shared_ptr<sf::View>& v)
{
    set_window(win);
    set_view(v);
    setPointCount(4);
    setRadius(25.f);
    setOrigin(sf::Vector2f(getRadius(), getRadius()));
    setRotation(sf::degrees(45));
    //spawn_point = win->getView().getCenter();
    spawn_point = sf::Vector2f(win->getView().getCenter().x, win->getView().getCenter().y - 500.f);
    setPosition(spawn_point);
    setFillColor(sf::Color::Yellow);
    
    //physics Initialisation
    //gravity
    gravity_ = sf::Vector2f(0, 9.8f) * VELOCITY_SCALE;
    //jumping velocity
    jump_vector_ = sf::Vector2f(0, -4.0f) * VELOCITY_SCALE;
    is_jumping_ = false;
    //movement
    velocity_ = sf::Vector2f(2.f, 0) * VELOCITY_SCALE;
    object_type_ = player_controlled;
    
    // collision setup
    collision_box_ = getGlobalBounds();
}

Player::~Player()
{
}

void Player::handle_input(float dt)
{
    if (!window_ref_.expired())
    {
        // debug code
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
                is_on_ground_ = false;
            }
            else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && is_key_held_) {
                is_key_held_ = false;
            }
            // debug functions or keep ?
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                move(-velocity_ * dt);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                move(velocity_ * dt);
            }
        }
        window.reset();
    }
}

void Player::update(float dt)
{
    ////Updating gravity
    //s=ut + 1/2at^2
    
    if (is_on_ground_) y_velocity_ = sf::Vector2f(0, 0);
    sf::Vector2f pos = y_velocity_ * dt + 0.5f * gravity_ * dt * dt;
    // v = u +at note its += not =
    y_velocity_ += gravity_ * dt;
    setPosition(getPosition() + pos);
    
}

// will handle collision responses according to their type
void Player::collision_response(GameObject* collider, const sf::Vector2f& mtv)
{
    
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
            if (y_velocity_.y < 0.f) y_velocity_.y = 0.f;
        }else
        {
            
        }
    }
}
