#pragma once
#include <cstdint>

/*
 * Contains all string definitions for wwise triggers
 */

inline constexpr std::string_view play_music_event{"Play_Background_Music" };
inline constexpr std::string_view stop_music_event{"Stop_Background_Music" };
inline constexpr std::string_view pause_music_event{"Pause_Background_Music" };
inline constexpr std::string_view resume_music_event{"Resume_Background_Music" };
inline constexpr std::string_view plat_event{"Platform_Landing" };
inline constexpr std::string_view hazard_event{"Destroy_Hazzard" };
inline constexpr std::string_view upbeat_event{"Change_To_Upbeat" };
inline constexpr std::string_view intensity_event{"Intensity" };
inline constexpr std::string_view volume_event{"Volume" };
