#include "game/objects/time_pickup.h"

namespace game {

TimePickup::TimePickup(bn::fixed_point position, int time_value)
    : GameObject(position, 6, 6,
                 engine::CollisionLayer::None,
                 engine::CollisionLayer::None),
      _time_value(time_value)
{
}

void TimePickup::update() {
    // TODO: idle animation, bob effect
}

void TimePickup::on_collision([[maybe_unused]] engine::GameObject& other) {
    // TODO: if other is Player, grant time and destroy self
}

} // namespace game
