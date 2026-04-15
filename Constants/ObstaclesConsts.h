#pragma once
// holds variables denoting parameters for obstacles being spawned into a level

// obstacle default speed
inline constexpr float MIN_OBJ_SPEED = 300.0f;
inline constexpr float MIN_SPWN_RATE = 0.5f;

// obstacle limits
inline constexpr float MAX_OBJ_SPEED = 1000.0f;
inline constexpr float MAX_SPWN_RATE = 0.2f;

// spawner constants
inline constexpr float OBJ_SPEED_MULTIPLIER = 0.2f;
inline constexpr float OBJ_SPAWN_MULTIPLIER = 0.01f;

// move to audio specific header?
inline constexpr float AUDIO_INTENSITY_MULTIPLIER = 0.5f;
inline constexpr float START_AUDIO_INTENSITY = 0.0f;
inline constexpr float MAX_AUDIO_INTENSITY = 100.0f;

// specific level state header?
// determines threshold used by level to change state (score based)
inline constexpr int HIGH_STATE_SCORE_THRESHOLD = 200;