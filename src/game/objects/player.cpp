#include "game/objects/player.h"
#include "game/constants.h"
#include "bn_keypad.h"

namespace game {

Player::Player()
    : GameObject({0, 0}, 8, 8,
                 engine::CollisionLayer::Player,
                 engine::CollisionLayer::Enemy | engine::CollisionLayer::Wall),
      _time_remaining(constants::PLAYER_START_TIME)
{
}

void Player::update() {
    // TODO: read input, set velocity, apply movement, drain time
}

void Player::on_collision([[maybe_unused]] engine::GameObject& other) {
    // TODO: handle enemy contact, wall pushback
}

} // namespace game
