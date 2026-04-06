#ifndef ECS_SYSTEMS_H
#define ECS_SYSTEMS_H

/**
 * @file
 * ECS system iteration helpers.
 *
 * Provides efficient iteration over entities with specific component
 * combinations using bitmask operations and GCC intrinsics.
 */

#include "bn_keypad.h"
#include "ecs/ecs_types.h"
#include "ecs_world.h"
#include <algorithm>

namespace ecs {

/**
 * Iterate over all alive entities and call the provided function.
 *
 * @param world The ECS world.
 * @param func Function to call with each entity ID.
 *
 * Example:
 *   for_each_entity(world, [](EntityId id) {
 *       // Process entity
 *   });
 */
template <typename Func>
inline void for_each_entity(World &world, Func &&func) {
  uint32_t mask = world.alive_mask();
  while (mask != 0) {
    int id = __builtin_ctz(mask); // Find next set bit
    func(static_cast<EntityId>(id));
    mask &= mask - 1; // Clear lowest set bit
  }
}

/**
 * Iterate over entities that have ALL of the specified components.
 *
 * @param world The ECS world.
 * @param required Component flags that entities must have.
 * @param func Function to call with each matching entity ID.
 *
 * Example:
 *   for_each_with(world, ComponentFlag::Position | ComponentFlag::Velocity,
 *       [&world](EntityId id) {
 *           auto& pos = world.get_component<Position>(id);
 *           auto& vel = world.get_component<Velocity>(id);
 *           pos.x += vel.vx;
 *           pos.y += vel.vy;
 *       });
 */
template <typename Func>
inline void for_each_with(World &world, ComponentFlag required, Func &&func) {
  uint32_t mask = world.alive_mask();
  while (mask != 0) {
    int id = __builtin_ctz(mask);
    EntityId eid = static_cast<EntityId>(id);

    if (world.has_components(eid, required)) {
      func(eid);
    }

    mask &= mask - 1;
  }
}

/**
 * Iterate over entities with specific components (const version).
 */
template <typename Func>
inline void for_each_with(const World &world, ComponentFlag required,
                          Func &&func) {
  uint32_t mask = world.alive_mask();
  while (mask != 0) {
    int id = __builtin_ctz(mask);
    EntityId eid = static_cast<EntityId>(id);

    if (world.has_components(eid, required)) {
      func(eid);
    }

    mask &= mask - 1;
  }
}

/**
 * Count entities with the specified components.
 */
inline int count_with(const World &world, ComponentFlag required) {
  int count = 0;
  for_each_with(world, required, [&count](EntityId) { ++count; });
  return count;
}

/**
 * Find the first entity with the specified components.
 *
 * @return Entity ID or INVALID_ENTITY if none found.
 */
inline EntityId find_first_with(const World &world, ComponentFlag required) {
  uint32_t mask = world.alive_mask();
  while (mask != 0) {
    int id = __builtin_ctz(mask);
    EntityId eid = static_cast<EntityId>(id);

    if (world.has_components(eid, required)) {
      return eid;
    }

    mask &= mask - 1;
  }
  return INVALID_ENTITY;
}

// =============================================================================
// Common System Patterns
// =============================================================================

inline void system_controllable(World &world) {
  for_each_with(world, ComponentFlag::Position | ComponentFlag::Velocity,
                [&world](EntityId id) {
                  auto &vel = world.get_component<Velocity>(id);
                  if (bn ::keypad::l_pressed() || bn::keypad::l_held()) {
                    vel.vx -= 1;
                    if (vel.vx < 0) {
                      vel.vx = 0;
                    }
                  }
                  if (bn::keypad::r_pressed() || bn::keypad::r_held()) {
                    vel.vx += 1;
                    if (vel.vx > 10) {
                      vel.vx = 10;
                    }
                  }
                  if (bn::keypad::up_pressed() || bn::keypad::up_held()) {
                    vel.vy += 1;
                    if (vel.vy > 10) {
                      vel.vy = 10;
                    }
                  }
                  if (bn::keypad::down_pressed() || bn::keypad::down_held()) {
                    vel.vy -= 1;
                    if (vel.vy < 0) {
                      vel.vy = 0;
                    }
                  }
                });
}

/**
 * Movement system - applies velocity to position.
 */
inline void system_movement(World &world) {
  for_each_with(world, ComponentFlag::Position | ComponentFlag::Velocity,
                [&world](EntityId id) {
                  auto &pos = world.get_component<Position>(id);
                  const auto &vel = world.get_component<Velocity>(id);
                  pos.x += vel.vx;
                  pos.y += vel.vy;
                });
}

/**
 * Time drain system - decrements player time each frame.
 * @param drain_rate Frames to drain per frame (default 1).
 */
inline void system_time_drain(World &world) {
  int drain_rate = 1;
  for_each_with(world, ComponentFlag::Player | ComponentFlag::Time,
                [&world, drain_rate](EntityId id) {
                  auto &time = world.get_component<Time>(id);
                  if (time.current > drain_rate) {
                    time.current -= drain_rate;
                  } else {
                    time.current = 0;
                  }
                });
}

/**
 * Sprite sync system - updates sprite positions from Position components.
 */
inline void system_sprite_sync(World &world) {
  for_each_with(world, ComponentFlag::Position | ComponentFlag::Sprite,
                [&world](EntityId id) {
                  const auto &pos = world.get_component<Position>(id);
                  auto &sprite = world.get_component<Sprite>(id);
                  if (sprite.ptr.has_value()) {
                    sprite.ptr->set_position(pos.x, pos.y);
                  }
                });
}

/**
 * Animation system - updates animation frames.
 */
inline void system_animation(World &world) {
  for_each_with(
      world, ComponentFlag::Animation | ComponentFlag::Sprite,
      [&world](EntityId id) {
        auto &anim = world.get_component<Animation>(id);

        ++anim.tick_counter;
        if (anim.tick_counter >= anim.ticks_per_frame) {
          anim.tick_counter = 0;

          if (anim.looping) {
            anim.current_frame = (anim.current_frame + 1) % anim.frame_count;
          } else if (anim.current_frame < anim.frame_count - 1) {
            ++anim.current_frame;
          }

          // Update sprite graphics index when frame changes
          auto &sprite = world.get_component<Sprite>(id);
          if (sprite.ptr.has_value()) {
            // Sprite tile update would go here once sprite items are set up
            // Example: sprite.ptr->set_tiles(tiles_item, anim.base_tile_index +
            // anim.current_frame);
            (void)
                sprite; // Suppress unused warning until sprites are implemented
          }
        }
      });
}

/**
 * Update all systems
 */
inline void system_update_all(World &world) {
  system_movement(world);
  system_time_drain(world);
  system_sprite_sync(world);
  system_controllable(world);
  system_animation(world);
}

} // namespace ecs

#endif // ECS_SYSTEMS_H
