#include "ObjectSpawner.h"

object_spawner::object_spawner(const std::shared_ptr<sf::RenderWindow>& win, const std::shared_ptr<sf::View>& v,
        const ObjectType& object_to_spawn, const sf::Vector2f& object_position )
{
        set_window(win);
        set_view(v);
        type_to_spawn = object_to_spawn;
        
}