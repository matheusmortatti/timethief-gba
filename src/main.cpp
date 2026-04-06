#include "bn_bg_palettes.h"
#include "bn_color.h"
#include "bn_core.h"
#include "bn_dp_direct_bitmap_bg_painter.h"
#include "bn_dp_direct_bitmap_bg_ptr.h"
#include "bn_log.h"

#include "ecs/ecs_systems.h"
#include "ecs/ecs_types.h"
#include "ecs/ecs_world.h"

namespace {

/**
 * Demo: Create a player entity with Position, Velocity, and Time components.
 * This verifies the ECS architecture is working correctly.
 */
void demo_ecs() {
  ecs::World world;

  // Create player entity
  ecs::EntityId player = world.create_entity();
  BN_LOG("Created player entity: ", static_cast<int>(player));

  // Add components (uses Butano's bn::fixed for positions)
  world.add_component(player, ecs::Position{.x = 0, .y = 0});
  world.add_component(player, ecs::Velocity{.vx = bn::fixed(0.5), .vy = 0});
  world.add_component(
      player, ecs::Time{.current = 1800, .max = 1800}); // 30 seconds at 60fps
  world.add_component(player, ecs::Player{});
  world.add_component(player, ecs::Facing{.dir = ecs::Direction::Right});

  // Verify components
  BN_LOG("Player has Position: ", world.has_component<ecs::Position>(player));
  BN_LOG("Player has Velocity: ", world.has_component<ecs::Velocity>(player));
  BN_LOG("Player has Time: ", world.has_component<ecs::Time>(player));
  BN_LOG("Player has Player: ", world.has_component<ecs::Player>(player));
  BN_LOG("Player has Sprite: ",
         world.has_component<ecs::Sprite>(player)); // Should be false

  // Create a test enemy
  ecs::EntityId enemy = world.create_entity();
  world.add_component(enemy, ecs::Position{.x = 50, .y = 30});
  world.add_component(enemy,
                      ecs::Time{.current = 300, .max = 300}); // 5 seconds
  world.add_component(enemy, ecs::AI{.behavior = ecs::AIBehavior::Wander});

  BN_LOG("Created enemy entity: ", static_cast<int>(enemy));
  BN_LOG("Entity count: ", world.entity_count());

  // Test iteration
  int movable_count = ecs::count_with(world, ecs::ComponentFlag::Position |
                                                 ecs::ComponentFlag::Velocity);
  BN_LOG("Entities with Position+Velocity: ", movable_count);

  // Test find_first_with
  ecs::EntityId found_player =
      ecs::find_first_with(world, ecs::ComponentFlag::Player);
  BN_LOG("Found player entity: ", static_cast<int>(found_player));

  // Simulate a few frames
  for (int frame = 0; frame < 10; ++frame) {
    // Run movement system
    ecs::system_movement(world);

    // Run time drain system
    ecs::system_time_drain(world);
  }

  // Check position after movement
  const auto &pos = world.get_component<ecs::Position>(player);
  BN_LOG("Player position after 10 frames - x: ", pos.x.integer(),
         " y: ", pos.y.integer());

  // Check time after drain
  const auto &time = world.get_component<ecs::Time>(player);
  BN_LOG("Player time after 10 frames: ", time.current);

  // Test component removal
  world.remove_component<ecs::Velocity>(player);
  BN_LOG("After removal - Player has Velocity: ",
         world.has_component<ecs::Velocity>(player));

  // Test entity destruction
  world.destroy_entity(enemy);
  BN_LOG("After destroy - Enemy alive: ", world.is_alive(enemy));
  BN_LOG("Entity count after destroy: ", world.entity_count());

  BN_LOG("ECS demo complete!");
}

} // anonymous namespace

int main() {
  bn::core::init();

  // Set a visible backdrop color (blue) so we know it's running
  bn::bg_palettes::set_transparent_color(bn::color(0, 0, 31));

  ecs::World world;
  ecs::EntityId player = world.create_entity();
  world.add_component(player, ecs::Position{.x = 50, .y = 50});
  world.add_component(player, ecs::Velocity{.vx = 0, .vy = 0});
  auto bg = bn::dp_direct_bitmap_bg_ptr::create();
  bn::dp_direct_bitmap_bg_painter painter(bg);

  while (true) {
    ecs::system_update_all(world);
    auto const &pos = world.get_component<ecs::Position>(player);
    auto const &vel = world.get_component<ecs::Velocity>(player);
    painter.fill(bn::color(0, 0, 0));

    painter.rectangle(pos.x.integer(), pos.y.integer(), pos.x.integer() + 10,
                      pos.y.integer() + 10, bn::color(31, 0, 0));
    painter.flip_page_later();
    bn::core::update();
  }
}
