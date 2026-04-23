#pragma once
#include <cstdint>

/*
 * Will contain all string definitions for wwise triggers
 */

inline constexpr std::string_view play_music_event{"Play_Background_Music" };
inline constexpr std::string_view stop_music_event{"Stop_Background_Music" };
inline constexpr std::string_view pause_music_event{"Stop_Background_Music" };
inline constexpr std::string_view resume_music_event{"Resume_Background_Music" };
inline constexpr std::string_view plat_event{"Platform_Landing" };
inline constexpr std::string_view hazard_event{"Destroy_Hazzard" };
inline constexpr std::string_view upbeat_event{"Change_To_Upbeat" };
inline constexpr std::string_view intensity_event{"Intensity" };


/*
 * Defines all Wwise IDs and their corresponding event names
 * Game Components can then register needed events
 * Processed at compile time
 */

// Set game object with specified ID and name of event that will trigger it
/*
struct AudioObject
{
    AkAudioObjectID ID;
    std::string_view EventName;
};


// Event used by wwise that contains ID of affecting audio and name of event
struct AudioTrigger
{
    AkAudioObjectID Associated_ID;
    std::string_view EventName;
};

// require registering as objects
inline constexpr AudioObject EVT_PLAY_BG_MUSIC {1, "Play_Background_Music"};


//  must refer to the registered objects
inline constexpr AudioTrigger EVT_PLAT_LANDING {1, "Platform_Landing"};
inline constexpr AudioTrigger EVT_DESTROY_HAZARD {1, "Destroy_Hazzard"};
inline constexpr AudioTrigger EVT_CHANGE_TO_UP_BEAT {1, "Change_To_Upbeat"};
inline constexpr AudioTrigger EVT_INTENSITY {1, "Intensity"};
inline constexpr AudioTrigger EVT_PAUSE_BG_MUSIC = {1, "Pause_Background_Music"};
inline constexpr AudioTrigger EVT_RESUME_BG_MUSIC = {1, "Resume_Background_Music"};
inline constexpr AudioTrigger EVT_STOP_BG_MUSIC = {1, "Stop_Background_Music"};

// only need in ID on Audio playing objects (classed as Game Object)
// events/triggers should refer to these ID's
*/