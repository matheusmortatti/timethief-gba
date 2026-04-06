#ifndef ECS_WORLD_H
#define ECS_WORLD_H

/**
 * @file
 * ECS World class - manages all entities and component arrays.
 *
 * Memory layout:
 * - IWRAM: Position, Velocity, Collision, Time, Combat, Facing, Player (~1.2
 * KB)
 * - EWRAM: Sprite, Animation, AI (~0.9 KB)
 */

#include "ecs_types.h"

namespace ecs {

// Forward declarations for component type traits
template <typename T> struct ComponentTraits;

/**
 * World manages all entities and their components.
 *
 * Entities are just indices into parallel component arrays.
 * A 32-bit bitmask tracks which entities are alive.
 * Each entity has a ComponentFlag bitmask tracking which components it has.
 */
class World {
public:
  // =========================================================================
  // Entity Management
  // =========================================================================

  /**
   * Create a new entity.
   * @return Entity ID (0-31) or INVALID_ENTITY if no slots available.
   */
  EntityId create_entity() {
    // Find first free slot using GCC intrinsic
    uint32_t free_mask = ~alive_mask_;
    if (free_mask == 0) {
      return INVALID_ENTITY; // All slots used
    }

    int slot = __builtin_ctz(free_mask); // Count trailing zeros
    alive_mask_ |= (1u << slot);
    component_masks_[slot] = ComponentFlag::None;
    ++entity_count_;

    return static_cast<EntityId>(slot);
  }

  /**
   * Destroy an entity and clear all its components.
   * @param id Entity to destroy.
   */
  void destroy_entity(EntityId id) {
    if (id < 0 || id >= MAX_ENTITIES)
      return;
    if (!is_alive(id))
      return;

    // Clear sprite component if present (needs explicit reset)
    if (has_flag(component_masks_[id], ComponentFlag::Sprite)) {
      sprites_[id].ptr.reset();
    }

    alive_mask_ &= ~(1u << id);
    component_masks_[id] = ComponentFlag::None;
    --entity_count_;
  }

  /**
   * Check if an entity is alive.
   */
  [[nodiscard]] bool is_alive(EntityId id) const {
    if (id < 0 || id >= MAX_ENTITIES)
      return false;
    return (alive_mask_ & (1u << id)) != 0;
  }

  /**
   * Get the number of alive entities.
   */
  [[nodiscard]] int entity_count() const { return entity_count_; }

  /**
   * Get the alive bitmask for iteration.
   */
  [[nodiscard]] uint32_t alive_mask() const { return alive_mask_; }

  // =========================================================================
  // Component Management - Add (overloaded by type)
  // =========================================================================

  void add_component(EntityId id, const Position &component) {
    if (!is_alive(id))
      return;
    positions_[id] = component;
    component_masks_[id] = component_masks_[id] | ComponentFlag::Position;
  }

  void add_component(EntityId id, const Velocity &component) {
    if (!is_alive(id))
      return;
    velocities_[id] = component;
    component_masks_[id] = component_masks_[id] | ComponentFlag::Velocity;
  }

  void add_component(EntityId id, const Collision &component) {
    if (!is_alive(id))
      return;
    collisions_[id] = component;
    component_masks_[id] = component_masks_[id] | ComponentFlag::Collision;
  }

  void add_component(EntityId id, const Time &component) {
    if (!is_alive(id))
      return;
    times_[id] = component;
    component_masks_[id] = component_masks_[id] | ComponentFlag::Time;
  }

  void add_component(EntityId id, const Combat &component) {
    if (!is_alive(id))
      return;
    combats_[id] = component;
    component_masks_[id] = component_masks_[id] | ComponentFlag::Combat;
  }

  void add_component(EntityId id, const Facing &component) {
    if (!is_alive(id))
      return;
    facings_[id] = component;
    component_masks_[id] = component_masks_[id] | ComponentFlag::Facing;
  }

  void add_component(EntityId id, const Player &component) {
    if (!is_alive(id))
      return;
    players_[id] = component;
    component_masks_[id] = component_masks_[id] | ComponentFlag::Player;
  }

  void add_component(EntityId id, const Sprite &component) {
    if (!is_alive(id))
      return;
    sprites_[id] = component;
    component_masks_[id] = component_masks_[id] | ComponentFlag::Sprite;
  }

  void add_component(EntityId id, const Animation &component) {
    if (!is_alive(id))
      return;
    animations_[id] = component;
    component_masks_[id] = component_masks_[id] | ComponentFlag::Animation;
  }

  void add_component(EntityId id, const AI &component) {
    if (!is_alive(id))
      return;
    ais_[id] = component;
    component_masks_[id] = component_masks_[id] | ComponentFlag::AI;
  }

  // =========================================================================
  // Component Management - Has (template with tag dispatch)
  // =========================================================================

  template <typename T> [[nodiscard]] bool has_component(EntityId id) const {
    return is_alive(id) &&
           has_flag(component_masks_[id], ComponentTraits<T>::flag);
  }

  // =========================================================================
  // Component Management - Get (overloaded by return type pointer)
  // =========================================================================

  // Non-const getters
  Position &get_position(EntityId id) { return positions_[id]; }
  Velocity &get_velocity(EntityId id) { return velocities_[id]; }
  Collision &get_collision(EntityId id) { return collisions_[id]; }
  Time &get_time(EntityId id) { return times_[id]; }
  Combat &get_combat(EntityId id) { return combats_[id]; }
  Facing &get_facing(EntityId id) { return facings_[id]; }
  Player &get_player(EntityId id) { return players_[id]; }
  Sprite &get_sprite(EntityId id) { return sprites_[id]; }
  Animation &get_animation(EntityId id) { return animations_[id]; }
  AI &get_ai(EntityId id) { return ais_[id]; }

  // Const getters
  [[nodiscard]] const Position &get_position(EntityId id) const {
    return positions_[id];
  }
  [[nodiscard]] const Velocity &get_velocity(EntityId id) const {
    return velocities_[id];
  }
  [[nodiscard]] const Collision &get_collision(EntityId id) const {
    return collisions_[id];
  }
  [[nodiscard]] const Time &get_time(EntityId id) const { return times_[id]; }
  [[nodiscard]] const Combat &get_combat(EntityId id) const {
    return combats_[id];
  }
  [[nodiscard]] const Facing &get_facing(EntityId id) const {
    return facings_[id];
  }
  [[nodiscard]] const Player &get_player(EntityId id) const {
    return players_[id];
  }
  [[nodiscard]] const Sprite &get_sprite(EntityId id) const {
    return sprites_[id];
  }
  [[nodiscard]] const Animation &get_animation(EntityId id) const {
    return animations_[id];
  }
  [[nodiscard]] const AI &get_ai(EntityId id) const { return ais_[id]; }

  // Template get_component using tag dispatch (for generic code)
  template <typename T> T &get_component(EntityId id);

  template <typename T> const T &get_component(EntityId id) const;

  // =========================================================================
  // Component Management - Remove (overloaded by type tag)
  // =========================================================================

  template <typename T> void remove_component(EntityId id);

  // =========================================================================
  // Component Flag Access
  // =========================================================================

  /**
   * Get the component mask for an entity.
   */
  [[nodiscard]] ComponentFlag get_component_mask(EntityId id) const {
    if (id < 0 || id >= MAX_ENTITIES)
      return ComponentFlag::None;
    return component_masks_[id];
  }

  /**
   * Check if an entity has all the specified components.
   */
  [[nodiscard]] bool has_components(EntityId id, ComponentFlag required) const {
    if (!is_alive(id))
      return false;
    return (component_masks_[id] & required) == required;
  }

private:
  // Entity tracking
  uint32_t alive_mask_ = 0;
  int entity_count_ = 0;
  ComponentFlag component_masks_[MAX_ENTITIES] = {};

  // IWRAM component arrays (hot data, frequently accessed)
  Position positions_[MAX_ENTITIES] = {};
  Velocity velocities_[MAX_ENTITIES] = {};
  Collision collisions_[MAX_ENTITIES] = {};
  Time times_[MAX_ENTITIES] = {};
  Combat combats_[MAX_ENTITIES] = {};
  Facing facings_[MAX_ENTITIES] = {};
  Player players_[MAX_ENTITIES] = {};

  // EWRAM component arrays (slower access, larger data)
  Sprite sprites_[MAX_ENTITIES] = {};
  Animation animations_[MAX_ENTITIES] = {};
  AI ais_[MAX_ENTITIES] = {};
};

// =============================================================================
// Component Traits - maps types to flags
// =============================================================================

template <> struct ComponentTraits<Position> {
  static constexpr ComponentFlag flag = ComponentFlag::Position;
};
template <> struct ComponentTraits<Velocity> {
  static constexpr ComponentFlag flag = ComponentFlag::Velocity;
};
template <> struct ComponentTraits<Collision> {
  static constexpr ComponentFlag flag = ComponentFlag::Collision;
};
template <> struct ComponentTraits<Time> {
  static constexpr ComponentFlag flag = ComponentFlag::Time;
};
template <> struct ComponentTraits<Combat> {
  static constexpr ComponentFlag flag = ComponentFlag::Combat;
};
template <> struct ComponentTraits<Facing> {
  static constexpr ComponentFlag flag = ComponentFlag::Facing;
};
template <> struct ComponentTraits<Player> {
  static constexpr ComponentFlag flag = ComponentFlag::Player;
};
template <> struct ComponentTraits<Sprite> {
  static constexpr ComponentFlag flag = ComponentFlag::Sprite;
};
template <> struct ComponentTraits<Animation> {
  static constexpr ComponentFlag flag = ComponentFlag::Animation;
};
template <> struct ComponentTraits<AI> {
  static constexpr ComponentFlag flag = ComponentFlag::AI;
};

// =============================================================================
// Template specializations for get_component (outside class)
// =============================================================================

template <> inline Position &World::get_component<Position>(EntityId id) {
  return positions_[id];
}
template <> inline Velocity &World::get_component<Velocity>(EntityId id) {
  return velocities_[id];
}
template <> inline Collision &World::get_component<Collision>(EntityId id) {
  return collisions_[id];
}
template <> inline Time &World::get_component<Time>(EntityId id) {
  return times_[id];
}
template <> inline Combat &World::get_component<Combat>(EntityId id) {
  return combats_[id];
}
template <> inline Facing &World::get_component<Facing>(EntityId id) {
  return facings_[id];
}
template <> inline Player &World::get_component<Player>(EntityId id) {
  return players_[id];
}
template <> inline Sprite &World::get_component<Sprite>(EntityId id) {
  return sprites_[id];
}
template <> inline Animation &World::get_component<Animation>(EntityId id) {
  return animations_[id];
}
template <> inline AI &World::get_component<AI>(EntityId id) {
  return ais_[id];
}

template <>
inline const Position &World::get_component<Position>(EntityId id) const {
  return positions_[id];
}
template <>
inline const Velocity &World::get_component<Velocity>(EntityId id) const {
  return velocities_[id];
}
template <>
inline const Collision &World::get_component<Collision>(EntityId id) const {
  return collisions_[id];
}
template <> inline const Time &World::get_component<Time>(EntityId id) const {
  return times_[id];
}
template <>
inline const Combat &World::get_component<Combat>(EntityId id) const {
  return combats_[id];
}
template <>
inline const Facing &World::get_component<Facing>(EntityId id) const {
  return facings_[id];
}
template <>
inline const Player &World::get_component<Player>(EntityId id) const {
  return players_[id];
}
template <>
inline const Sprite &World::get_component<Sprite>(EntityId id) const {
  return sprites_[id];
}
template <>
inline const Animation &World::get_component<Animation>(EntityId id) const {
  return animations_[id];
}
template <> inline const AI &World::get_component<AI>(EntityId id) const {
  return ais_[id];
}

// =============================================================================
// Template specializations for remove_component (outside class)
// =============================================================================

template <> inline void World::remove_component<Position>(EntityId id) {
  if (!is_alive(id))
    return;
  component_masks_[id] = component_masks_[id] & ~ComponentFlag::Position;
}

template <> inline void World::remove_component<Velocity>(EntityId id) {
  if (!is_alive(id))
    return;
  component_masks_[id] = component_masks_[id] & ~ComponentFlag::Velocity;
}

template <> inline void World::remove_component<Collision>(EntityId id) {
  if (!is_alive(id))
    return;
  component_masks_[id] = component_masks_[id] & ~ComponentFlag::Collision;
}

template <> inline void World::remove_component<Time>(EntityId id) {
  if (!is_alive(id))
    return;
  component_masks_[id] = component_masks_[id] & ~ComponentFlag::Time;
}

template <> inline void World::remove_component<Combat>(EntityId id) {
  if (!is_alive(id))
    return;
  component_masks_[id] = component_masks_[id] & ~ComponentFlag::Combat;
}

template <> inline void World::remove_component<Facing>(EntityId id) {
  if (!is_alive(id))
    return;
  component_masks_[id] = component_masks_[id] & ~ComponentFlag::Facing;
}

template <> inline void World::remove_component<Player>(EntityId id) {
  if (!is_alive(id))
    return;
  component_masks_[id] = component_masks_[id] & ~ComponentFlag::Player;
}

template <> inline void World::remove_component<Sprite>(EntityId id) {
  if (!is_alive(id))
    return;
  sprites_[id].ptr.reset();
  component_masks_[id] = component_masks_[id] & ~ComponentFlag::Sprite;
}

template <> inline void World::remove_component<Animation>(EntityId id) {
  if (!is_alive(id))
    return;
  component_masks_[id] = component_masks_[id] & ~ComponentFlag::Animation;
}

template <> inline void World::remove_component<AI>(EntityId id) {
  if (!is_alive(id))
    return;
  component_masks_[id] = component_masks_[id] & ~ComponentFlag::AI;
}

} // namespace ecs

#endif // ECS_WORLD_H
