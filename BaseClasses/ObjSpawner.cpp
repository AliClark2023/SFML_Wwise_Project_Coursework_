#include "ObjSpawner.h"

object_spawner::object_spawner(sf::RenderWindow& win, sf::View& v) : GameObject(win, v)
{
        // initialising srand for random change calculation for spawning hazards
        srand(time(nullptr));
}

void object_spawner::spawn_object()
{
        scenery_config scene_config;
        AudioObject object_triggered_sfx_;
        
        // determining if object will become a hazard or platform (requires chance to be set after initialisation)
        int chance = rand() % 100;
        if (chance < hazard_chance)
        {
                scene_config.type = ObjectType::hazard;
                object_type_ = hazard;
                // set sfx
                object_triggered_sfx_ = hazard_triggered_sfx;
        }else
        {
                scene_config.type = ObjectType::scenery;
                object_type_ = scenery;
                //set sfx
                object_triggered_sfx_ = plat_triggered_sfx_;
        }
 
        // spawning objects, adding reference to container
        switch (object_type_)
        {
        case scenery:
                scene_config.point_count = 4;
                scene_config.radius = 20;
                scene_config.origin = sf::Vector2f( scene_config.radius,  scene_config.radius);
                scene_config.rotation = 45;
                scene_config.position = getPosition();
                scene_config.velocity = sf::Vector2f(-1, 0);
                scene_config.move_speed = object_speed_;
                scene_config.color = sf::Color::Cyan;
                scene_config.audio_event_sfx = object_triggered_sfx_;
                objects_.emplace_back(std::make_unique<Scenery>(window_ref_, view_ref_, scene_config));
                objects_.back()->set_alive(true);
                break;
        case hazard:
                scene_config.point_count = 4;
                scene_config.radius = 20;
                scene_config.origin = sf::Vector2f( scene_config.radius,  scene_config.radius);
                scene_config.rotation = 0;
                scene_config.position = getPosition();
                scene_config.velocity = sf::Vector2f(-1, 0);
                scene_config.move_speed = object_speed_;
                scene_config.color = sf::Color::Magenta;
                scene_config.audio_event_sfx = object_triggered_sfx_;
                objects_.emplace_back(std::make_unique<Scenery>(window_ref_, view_ref_, scene_config));
                objects_.back()->set_alive(true);
                break;
        default:
                break;
        }
       
}

void object_spawner::render_objects() const
{

        for (const auto& object : objects_)
        {
                if (object->is_alive())
                {
                        window_ref_.draw(*object);
                }
        }
}

void object_spawner::detect_collision(const std::unique_ptr<Player>& player)
{
        int object_count = 0;
        for (const auto& object : objects_)
        {
                if (object->is_alive())
                {
                        // player v object collisions
                        sf::Vector2f mtv2;
                        if (sat_detection::sat_collision(*player, *object, mtv2))
                        {
                                player->collision_response(object.get(), mtv2);
                                object->collision_response(player.get(), mtv2);
                        }
                        // only counts non counted objects
                        if (!object->has_been_counted())
                        {
                                // score based on activating platform
                                if (object->has_been_activated())
                                {
                                        object->set_counted(true);
                                        object_count++;
                                }
                        }
                        // object passing by render view (set flag for deletion)
                        if (object->getPosition().x < despawn_threshold.x)
                        {
                                object->set_alive(false);
                        }
                }
        }
        objects_scored_ = object_count;
}
// returns number of objects that have passed score threshold and resets count
int object_spawner::get_objects_scored()
{
        int scored_objects = objects_scored_;
        objects_scored_ = 0;
        return scored_objects;
}

void object_spawner::update(float dt)
{
        // updating or removing dead objects from vector
        // no need to increment iterator in loop condition as it gets updated within the loop
        for (auto it = objects_.begin(); it != objects_.end();)
        {
                if (*it && !(*it)->is_alive())
                {
                        it = objects_.erase(it);
                }else
                {
                        if (*it)
                        {
                                // update object with new speed if not already at it
                                float it_speed = (*it)->get_Speed();
                                // would only work assuming velocity direction, better to have a speed variable within to compare to
                                if (it_speed < object_speed_)
                                {
                                        (*it)->set_speed(object_speed_);
                                }
                                (*it)->update(dt);
                                ++it;
                        }
                }
        }
        
}

// not in use but required to be defined
void object_spawner::handle_input(float dt)
{
}

void object_spawner::collision_response(GameObject* collider, const sf::Vector2f& mtv)
{
}
