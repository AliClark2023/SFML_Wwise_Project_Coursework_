#pragma once
#include <map>
#include "../WwiseWrapper.h"
#include <SFML/Graphics.hpp>
#include "../Constants/AudioObjects.h"

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
    
    static void initialize(sf::RenderWindow& window_ref, sf::View& view_ref);
    static AudioManager& instance();
    
    // disabling copy functionality to create singleton class
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    AudioObject register_object(const std::string& obj_name, const std::string& obj_associated_name);
    AudioObject get_registered_object(const std::string& obj_name);
    void deregister_audio_object(std::string obj_name);
    void handle_input(float dt);
private:
    AudioManager(sf::RenderWindow& window, sf::View& view);
    static AudioManager* instance_;
    
    sf::RenderWindow& window_ref_;
    sf::View& view_ref_;
    std::map<std::string, AkAudioObjectID> registered_objects_;
    int id_value_ = 1;
    
    float volume_level_ = 20;
    // units per second
    const float volume_rate_ = 30.f;
    
     AudioObject bg_music;
     AudioObject bg_volume;
};
