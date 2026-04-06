#ifndef ECS_TYPES_H
#define ECS_TYPES_H

/**
 * @file
 * ECS component definitions and type constants for Time Thief GBA.
 *
 * Design: Structure of Arrays (SOA) with bitmask-based component tracking.
 * - Entities are indices (0-31) into parallel component arrays
 * - 32-bit bitmasks track entity state and component presence
 * - Static allocation only - all memory determined at compile time
 */

#include "bn_fixed.h"
#include "bn_optional.h"
#include "bn_sprite_ptr.h"

namespace ecs {

// =============================================================================
// Constants
// =============================================================================

constexpr int MAX_ENTITIES = 32;
using EntityId = int8_t;
constexpr EntityId INVALID_ENTITY = -1;

// =============================================================================
// Component Flags (bitmask for component presence)
// =============================================================================

enum class ComponentFlag : uint16_t {
    None       = 0,
    Position   = 1 << 0,
    Velocity   = 1 << 1,
    Collision  = 1 << 2,
    Time       = 1 << 3,
    Combat     = 1 << 4,
    Facing     = 1 << 5,
    Player     = 1 << 6,
    Sprite     = 1 << 7,
    Animation  = 1 << 8,
    AI         = 1 << 9,
};

// Bitwise operators for ComponentFlag
inline constexpr ComponentFlag operator|(ComponentFlag a, ComponentFlag b) {
    return static_cast<ComponentFlag>(
        static_cast<uint16_t>(a) | static_cast<uint16_t>(b)
    );
}

inline constexpr ComponentFlag operator&(ComponentFlag a, ComponentFlag b) {
    return static_cast<ComponentFlag>(
        static_cast<uint16_t>(a) & static_cast<uint16_t>(b)
    );
}

inline constexpr ComponentFlag operator~(ComponentFlag a) {
    return static_cast<ComponentFlag>(~static_cast<uint16_t>(a));
}

inline constexpr bool has_flag(ComponentFlag flags, ComponentFlag test) {
    return (static_cast<uint16_t>(flags) & static_cast<uint16_t>(test)) != 0;
}

// =============================================================================
// Direction Enum (for Facing component)
// =============================================================================

enum class Direction : uint8_t {
    Down  = 0,
    Up    = 1,
    Left  = 2,
    Right = 3,
};

// =============================================================================
// Collision Layers
// =============================================================================

enum class CollisionLayer : uint8_t {
    None    = 0,
    Player  = 1 << 0,
    Enemy   = 1 << 1,
    Weapon  = 1 << 2,
    Wall    = 1 << 3,
};

inline constexpr CollisionLayer operator|(CollisionLayer a, CollisionLayer b) {
    return static_cast<CollisionLayer>(
        static_cast<uint8_t>(a) | static_cast<uint8_t>(b)
    );
}

inline constexpr CollisionLayer operator&(CollisionLayer a, CollisionLayer b) {
    return static_cast<CollisionLayer>(
        static_cast<uint8_t>(a) & static_cast<uint8_t>(b)
    );
}

inline constexpr bool has_layer(CollisionLayer layers, CollisionLayer test) {
    return (static_cast<uint8_t>(layers) & static_cast<uint8_t>(test)) != 0;
}

// =============================================================================
// AI Behavior Types
// =============================================================================

enum class AIBehavior : uint8_t {
    None     = 0,
    Idle     = 1,
    Wander   = 2,
    Chase    = 3,
    Flee     = 4,
};

// =============================================================================
// Attack States
// =============================================================================

enum class AttackState : uint8_t {
    None     = 0,
    Windup   = 1,
    Active   = 2,
    Recovery = 3,
};

// =============================================================================
// IWRAM Components (~1.2 KB total in IWRAM)
// =============================================================================

/**
 * Position component - 8 bytes
 * World position using Butano's fixed-point math.
 */
struct Position {
    bn::fixed x = 0;
    bn::fixed y = 0;
};

/**
 * Velocity component - 8 bytes
 * Movement speed per frame.
 */
struct Velocity {
    bn::fixed vx = 0;
    bn::fixed vy = 0;
};

/**
 * Collision component - 8 bytes
 * AABB hitbox with layer info.
 */
struct Collision {
    int8_t half_width  = 8;   // Half-extents for AABB
    int8_t half_height = 8;
    CollisionLayer layer = CollisionLayer::None;  // What layer this entity is on
    CollisionLayer mask  = CollisionLayer::None;  // What layers this entity collides with
    uint8_t _padding[4] = {};
};

/**
 * Time component - 4 bytes
 * Health IS time in this game. Current and max stored as frame counts.
 */
struct Time {
    uint16_t current = 0;  // Current time (frames remaining)
    uint16_t max     = 0;  // Maximum time
};

/**
 * Combat component - 6 bytes
 * Attack state machine and damage values.
 */
struct Combat {
    AttackState state     = AttackState::None;
    uint8_t     cooldown  = 0;   // Frames until next attack allowed
    uint8_t     timer     = 0;   // Current state timer
    uint8_t     damage    = 30;  // Time stolen on hit (frames)
    uint8_t     _padding[2] = {};
};

/**
 * Facing component - 1 byte
 * Direction the entity is facing.
 */
struct Facing {
    Direction dir = Direction::Down;
};

/**
 * Player component - 2 bytes
 * Marker component for player entity with input state.
 */
struct Player {
    uint8_t input_flags = 0;  // Current frame input state
    uint8_t _padding = 0;
};

// =============================================================================
// EWRAM Components (~0.9 KB total in EWRAM)
// =============================================================================

/**
 * Sprite component - 16 bytes (bn::optional<bn::sprite_ptr>)
 * Wraps Butano's sprite smart pointer.
 */
struct Sprite {
    bn::optional<bn::sprite_ptr> ptr;
};

/**
 * Animation component - 8 bytes
 * Animation state for sprite frame updates.
 */
struct Animation {
    uint8_t  current_frame   = 0;
    uint8_t  frame_count     = 1;
    uint8_t  ticks_per_frame = 6;  // ~10 fps at 60 fps game
    uint8_t  tick_counter    = 0;
    uint8_t  base_tile_index = 0;  // Starting tile for this animation
    bool     looping         = true;
    uint8_t  _padding[2] = {};
};

/**
 * AI component - 4 bytes
 * Simple AI state machine.
 */
struct AI {
    AIBehavior behavior    = AIBehavior::Idle;
    uint8_t    state       = 0;     // Behavior-specific state
    uint8_t    timer       = 0;     // State timer
    uint8_t    _padding    = 0;
};

} // namespace ecs

#endif // ECS_TYPES_H
