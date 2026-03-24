#include "Scenery.h"

Scenery::Scenery(sf::RenderWindow& win, sf::View& v) : GameObject(win, v)
{
    setPointCount(4);
    setRadius(view_ref_.getSize().x);
    setOrigin(sf::Vector2f(getRadius(), getRadius()));
    setRotation(sf::degrees(45));
    setPosition(sf::Vector2f(window_ref_.getView().getCenter().x, window_ref_.getView().getCenter().y + (getRadius()/1.2)));
    setFillColor(sf::Color::Red);
    object_type_ = scenery;
}

Scenery::Scenery(sf::RenderWindow& win, sf::View& v,
    const scenery_config& config) : GameObject(win, v)
{
    setPointCount(config.point_count);
    setRadius(config.radius);
    setOrigin(config.origin);
    setRotation(sf::degrees(config.rotation));
    setPosition(config.position);
    set_velocity(config.velocity);
    setFillColor(config.color);
    object_type_ = config.type;
}

Scenery::~Scenery()
{
}

void Scenery::handle_input(float dt)
{
}

void Scenery::update(float dt)
{
    // update position based on velocity
    move(velocity_ * dt);
}

// needs to indicate whether player has collided with object
void Scenery::collision_response(GameObject* collider, const sf::Vector2f& mtv)
{
    float collision_align;
    switch (object_type_)
    {
        case scenery:
            // alter so that only an upward collision will change colour/gain points
            collision_align = mtv.x * UP.x + mtv.y * UP.y;
            //landing from above
            if (collision_align > 0.5f)
            {
                if (!has_been_activated())
                {
                    set_activated(true);
                    setFillColor(sf::Color::Green);
                }
            }
        break;
        case hazard:
            set_alive(false);
        break;
        default:
        break;
    }
}
