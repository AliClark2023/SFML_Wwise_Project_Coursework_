#pragma once
#include <map>
#include "../WwiseWrapper.h"

// Set game object with specified ID and name of event that will trigger it
struct AudioObject
{
    std::string Name;
    AkAudioObjectID ID;
};

// manages registering wwise objects and their associating IDs
// contains user input controls to adjust audio in game (volume currently implemented)
class AudioManager
{
public:
    static AudioManager& instance();
    AudioObject register_object(const std::string& obj_name, const std::string& obj_associated_name);
    AudioObject get_registed_object(const std::string& obj_name);
    void deregister_audio_object(std::string obj_name);
    void handle_input(float dt);
private:
    std::map<std::string, AkAudioObjectID> registered_objects_;
    int id_value_;
};
