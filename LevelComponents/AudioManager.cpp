#include "AudioManager.h"

AudioManager& AudioManager::instance()
{
    static AudioManager instance;
    return instance;
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
        // registered objects requires an existing object to work
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

AudioObject AudioManager::get_registed_object(const std::string& obj_name)
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

void AudioManager::handle_input(float dt)
{
}
