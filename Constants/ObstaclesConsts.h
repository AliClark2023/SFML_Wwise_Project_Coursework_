#pragma once
// holds variables denoting parameters for obstacles being spawned into a level

// obstacle default speed
inline constexpr float MIN_OBJ_SPEED = 300.0f;
inline constexpr float MAX_OBJ_SPEED = 800.0f;

// spawner constants
inline constexpr float MIN_SPAWN_RATE = 0.25f;
inline constexpr float MAX_SPAWN_RATE = 1.5f;

// Audio constants
inline constexpr float START_AUDIO_INTENSITY = 0.0f;
inline constexpr float MAX_AUDIO_INTENSITY = 100.0f;

// determines threshold used by level to change state (score based) and for adjusting spawn rates and speed
inline constexpr int HIGH_STATE_SCORE_THRESHOLD = 50; 
