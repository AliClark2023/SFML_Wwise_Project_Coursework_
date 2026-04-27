#include "AudioManager.h"

AudioManager* AudioManager::instance_ = nullptr;

void AudioManager::initialize(sf::RenderWindow& window_ref, sf::View& view_ref)
{
    if (!instance_)
    {
        instance_ = new AudioManager(window_ref, view_ref);
    }
}

AudioManager& AudioManager::instance()
{
    return *instance_;
}

AudioObject AudioManager::register_object(const std::string& obj_name, const std::string& obj_associated_name)
{
    AudioObject obj;
    
    // game object/event that doesn't rely on other registered audio objects
    if (obj_associated_name.empty())
    {
        //requires check if object has been registered before
        
        AK::SoundEngine::RegisterGameObj(id_value_);
        
        obj.Name = obj_name;
        obj.ID = id_value_;
        registered_objects_.insert(std::pair<std::string, AkAudioObjectID>(obj.Name, obj.ID));
        id_value_ ++;
    }
    else{
        // obj_name requires an existing object to work, doesn't require registering to sound engine, just needs ID of a registered object it will affect
        auto it = registered_objects_.find(obj_associated_name);
        if (it != registered_objects_.end())
        {
            std::pair<std::string, AkAudioObjectID> entry = *it;
            obj.Name = obj_name;
            obj.ID = entry.second;
        }
    }
    return obj;
}

AudioObject AudioManager::get_registered_object(const std::string& obj_name)
{
    AudioObject obj;
    auto it = registered_objects_.find(obj_name);
    if (it != registered_objects_.end())
    {
        std::pair<std::string, AkAudioObjectID> entry = *it;
        obj.Name = entry.first;
        obj.ID = entry.second;
    }
    return obj;
}

void AudioManager::deregister_audio_object(std::string obj_name)
{
}

// handles audio controls (volume implemented only)
void AudioManager::handle_input(float dt)
{
    // volume adjustment when player input is detected
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
    {
        volume_level_ += volume_rate_ * dt;
        volume_level_ = std::clamp(volume_level_, 0.f, 100.f);
        AK::SoundEngine::SetRTPCValue(volume_event.data(), volume_level_);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))
    {
        volume_level_ -= volume_rate_ * dt;
        volume_level_ = std::clamp(volume_level_, 0.f, 100.f);
        AK::SoundEngine::SetRTPCValue(volume_event.data(), volume_level_);
    }
    
    
}

// Creates default 
AudioManager::AudioManager(sf::RenderWindow& window, sf::View& view): window_ref_(window), view_ref_(view)
{
    bg_music = register_object(play_music_event.data(),"");
    bg_volume = register_object(volume_event.data(),play_music_event.data());
}
