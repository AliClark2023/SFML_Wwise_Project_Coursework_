#include "ObjSpawner.h"

object_spawner::object_spawner(const std::shared_ptr<sf::RenderWindow>& win, const std::shared_ptr<sf::View>& v,
        const ObjectType& object_to_spawn)
{
        set_window(win);
        set_view(v);
        type_to_spawn_ = object_to_spawn;
        
        
}

void object_spawner::spawn_object()
{
        std::shared_ptr<sf::RenderWindow> window ;
        std::shared_ptr<sf::View> view ;
        scenery_config scene_config;
        if (!window_ref_.expired()) window = window_ref_.lock();
        if (!view_ref_.expired()) view = view_ref_.lock();
        
        if (!window_ref_.expired() && !view_ref_.expired())
        {
                switch (type_to_spawn_)
                {
                case scenery:
                        scene_config.point_count = 4;
                        scene_config.radius = 20;
                        scene_config.origin = sf::Vector2f( scene_config.radius,  scene_config.radius);
                        scene_config.rotation = 0;
                        scene_config.position = getPosition();
                       // scene_config.position = view->getCenter();
                        scene_config.velocity = sf::Vector2f(-object_speed_, 0);
                        scene_config.color = sf::Color::Cyan;
                        objects_.emplace_back(std::make_unique<Scenery>(window, view, scene_config));
                        objects_.back()->set_alive(true);
                        break;
                case hazard:
                        break;
                default:
                        break;
                }
        }
        
        window.reset();
        view.reset();
       
}

void object_spawner::set_spawn_rate(float)
{
}

void object_spawner::set_object_speed(float)
{
}

void object_spawner::set_object_speed(sf::Vector2f vel)
{
}

void object_spawner::render_objects() const
{
        if (!window_ref_.expired())
        {
                std::shared_ptr<sf::RenderWindow> window = window_ref_.lock();
                for (const auto& object : objects_)
                {
                        if (object->is_alive())
                        {
                                window->draw(*object);
                        }
                }
                window.reset();
        }
}

void object_spawner::handle_input(float dt)
{
}

void object_spawner::update(float dt)
{
        // spawning according to spawn rate
        elapsed_time_ += dt;
        if (elapsed_time_ >= spawn_rate_)
        {
                spawn_object();
                elapsed_time_ = 0.0f;
        }
        // updating or removing objects from vector
        for (const auto& object : objects_)
        {
                if (object->is_alive())
                {
                        object->update(dt);
                }else
                {
                        objects_.erase(objects_.begin());
                }
        } 
        
}

void object_spawner::collision_response(GameObject* collider, const sf::Vector2f& mtv)
{
}
