#include "game/objects/enemy_base.h"

namespace game {

EnemyBase::EnemyBase(bn::fixed_point position, int time_budget)
    : GameObject(position, 8, 8,
                 engine::CollisionLayer::Enemy,
                 engine::CollisionLayer::Player | engine::CollisionLayer::Wall),
      _time_budget(time_budget)
{
}

void EnemyBase::update() {
    // TODO: base AI tick (timer decrement, state transitions)
}

void EnemyBase::on_collision([[maybe_unused]] engine::GameObject& other) {
    // TODO: handle player contact
}

void EnemyBase::steal_time(int amount) {
    _time_budget -= amount;
    if (_time_budget <= 0) {
        destroy();
    }
}

} // namespace game
