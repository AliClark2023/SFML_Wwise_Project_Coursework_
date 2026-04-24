#include "ObjSpawner.h"

object_spawner::object_spawner(sf::RenderWindow& win, sf::View& v,
        const ObjectType& object_to_spawn, const AudioObject& event) : GameObject(win, v)
{
        //type_to_spawn_ = object_to_spawn;
        object_type_ = object_to_spawn;
        object_triggered_sfx_ = event;
}

void object_spawner::spawn_object()
{
        /*
        std::shared_ptr<sf::RenderWindow> window ;
        std::shared_ptr<sf::View> view ;
        scenery_config scene_config;
        if (!window_ref_.expired()) window = window_ref_.lock();
        if (!view_ref_.expired()) view = view_ref_.lock();
        
        if (!window_ref_.expired() && !view_ref_.expired())
        {
                
        }
        
        window.reset();
        view.reset();
        */
        scenery_config scene_config;
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
                scene_config.type = ObjectType::scenery;
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
                scene_config.color = sf::Color::Yellow;
                scene_config.type = ObjectType::hazard;
                scene_config.audio_event_sfx = object_triggered_sfx_;
                objects_.emplace_back(std::make_unique<Scenery>(window_ref_, view_ref_, scene_config));
                objects_.back()->set_alive(true);
                break;
        default:
                break;
        }
       
}
//need to clamp min/max values (remove?)
void object_spawner::update_object_speed(const int& score, const float& time)
{
        
        // timer increase calculation
        const int total_seconds = static_cast<int>(time);
        const int minutes = total_seconds / 60;
        const int seconds = total_seconds % 60;
        
        if (minutes == 0 && seconds == 0) return;
        //testing (every 10s increase speed)(remove magic numbers and make variable/calculation)
        if (seconds % 10 == 0 && !increased_speed_)
        {
                // change to add
                object_speed_ += static_cast<float>(seconds) / 10;
                increased_speed_ = true;
        }else if (seconds % 10 >= 1 && increased_speed_)
        {
                increased_speed_ = false;
        }
        
        //score increase calculation(remove magic numbers and make variable/calculation)
        if (score == 0) return;
        if (score % 10 == 0 && !increased_speed_)
        {
                // change to add
                object_speed_ += static_cast<float>(score) / 10;
                increased_speed_ = true;
                // can change type to spawn
                // type_to_spawn_ = hazard;
        }else if (score % 10 >= 1 && increased_speed_)
        {
                increased_speed_ = false;
        }
        
       
}

// need to clamp min/max values (remove?)
void object_spawner::update_spawn_rate(const int& score, const float& time)
{
        // timer increase calculation
        const int total_seconds = static_cast<int>(time);
        const int minutes = total_seconds / 60;
        const int seconds = total_seconds % 60;
        
        if (minutes == 0 && seconds == 0) return;
        //testing (every 10s increase speed)
        if (seconds % 10 == 0 && !increases_spawn_rate_)
        {
                // change to add
                spawn_rate_ *= 0.5f;
                increases_spawn_rate_ = true;
        }else if (seconds % 10 >= 1 && increases_spawn_rate_)
        {
                increases_spawn_rate_ = false;
        }
        
        //score increase calculation
        if (score == 0) return;
        if (score % 10 == 0 && !increases_spawn_rate_)
        {
                // change to add
                spawn_rate_ *= 0.5f;
                increases_spawn_rate_ = true;
        }else if (score % 10 >= 1 && increases_spawn_rate_)
        {
                increases_spawn_rate_ = false;
        }
}

void object_spawner::render_objects() const
{
        /*
        if (!window_ref_.expired())
        {
                std::shared_ptr<sf::RenderWindow> window = window_ref_.lock();
                
                window.reset();
        }
        */
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
                        //if (object->get_object_type() != scenery) return;
                        sf::Vector2f mtv2;
                        if (sat_detection::sat_collision(*player, *object, mtv2))
                        {
                                player->collision_response(object.get(), mtv2);
                                object->collision_response(player.get(), mtv2);
                        }
                        // object v score threshold position or
                        // object v activation = score increase
                        if (!object->has_been_counted())
                        {
                                /* score based on passing threshold
                                if (object->getPosition().x < score_threshold_pos_.x)
                                {
                                        object->set_counted(true);
                                        object_count++;
                                }
                                */
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

        //updating or removing dead objects from vector
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

void object_spawner::collision_response(GameObject* collider, const sf::Vector2f& mtv)
{
}
