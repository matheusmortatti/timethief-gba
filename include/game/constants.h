#pragma once

#include <cstdint>

namespace game::constants {

constexpr int MAX_ENEMIES  = 16;
constexpr int MAX_OBJECTS  = 32;

// Z-order layers
constexpr int Z_BACKGROUND = 3;
constexpr int Z_OBJECTS    = 2;
constexpr int Z_PLAYER     = 1;
constexpr int Z_UI         = 0;

// Time mechanic (frames)
constexpr int PLAYER_START_TIME  = 3600; // 60 seconds at 60fps
constexpr int TIME_DRAIN_PER_FRAME = 1;

} // namespace game::constants
