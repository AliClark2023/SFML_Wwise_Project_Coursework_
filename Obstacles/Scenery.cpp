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
    set_speed(config.move_speed);
    set_velocity(config.velocity);
    setFillColor(config.color);
    object_type_ = config.type;
    audio_sfx_event = config.audio_event_sfx;
}

Scenery::~Scenery()
{
}

void Scenery::handle_input(float dt)
{
}

void Scenery::update(float dt)
{
    // update position based on velocity (assumes vel is just a direction vector)
    move(velocity_ * speed_ * dt);
}

// needs to indicate whether player has collided with object
void Scenery::collision_response(GameObject* collider, const sf::Vector2f& mtv)
{
    float collision_align;
    switch (object_type_)
    {
        case scenery:
            collision_align = mtv.x * UP.x + mtv.y * UP.y;
            //landing from above
            if (collision_align > 0.5f)
            {
                if (!has_been_activated())
                {
                    AK::SoundEngine::PostEvent(audio_sfx_event.Name.data(), audio_sfx_event.ID, AK_EndOfEvent, AudioEventCallback, this);
                }
            }
        break;
        case hazard:
            AK::SoundEngine::PostEvent(audio_sfx_event.Name.data(), audio_sfx_event.ID, AK_EndOfEvent, AudioEventCallback, this);
        break;
        default:
        break;
    }
}
// attempted to sync object changing colour with audio trigget
void Scenery::AudioEventCallback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
{
    if (in_eType & AK_EndOfEvent)
    {
        if (auto* owner = static_cast<Scenery*>(in_pCallbackInfo->pCookie))
        {
            if (owner->object_type_ == scenery)
            {
                owner->set_activated(true);
                owner->setFillColor(sf::Color::Green);
            }else
            {
                owner->set_alive(false);
            }
        }
    }
}
