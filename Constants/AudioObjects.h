#pragma once
#include <cstdint>

/*
 * Defines all Wwise IDs and their corresponding event names
 * Game Components can then register needed events
 * Processed at compile time
 */

struct AudioEvent
{
    AkAudioObjectID ID;
    std::string_view EventName;
};

inline constexpr AudioEvent EVT_PLAY_BG_MUSIC {1, "Play_Background_Music"};
inline constexpr AudioEvent EVT_CHANGE_TO_UP_BEAT {2, "Change_To_Upbeat"};
inline constexpr AudioEvent EVT_INTENSITY {3, "Intensity"};

// Not Currently Implemented
inline constexpr AudioEvent EVT_PAUSE_BG_MUSIC = {4, "Pause_Background_Music"};
inline constexpr AudioEvent EVT_RESUME_BG_MUSIC = {5, "Resume_Background_Music"};