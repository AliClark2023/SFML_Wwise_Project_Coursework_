#include "Scenery.h"

Scenery::Scenery(const std::shared_ptr<sf::RenderWindow>& win, const std::shared_ptr<sf::View>& v)
{
    set_window(win);
    set_view(v);
    setPointCount(4);
    setRadius(v->getSize().x);
    setOrigin(sf::Vector2f(getRadius(), getRadius()));
    setRotation(sf::degrees(45));
    setPosition(sf::Vector2f(win->getView().getCenter().x, win->getView().getCenter().y + (getRadius()/1.2)));
    setFillColor(sf::Color::Red);
    object_type_ = scenery;
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

void Scenery::collision_response(GameObject* collider, const sf::Vector2f& mtv)
{
}
