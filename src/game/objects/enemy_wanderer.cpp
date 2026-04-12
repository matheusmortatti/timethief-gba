#include "game/objects/enemy_wanderer.h"

namespace game {

EnemyWanderer::EnemyWanderer()
    : EnemyBase({0, 0}, 0)
{
    _alive = false; // inactive until reset() is called
}

void EnemyWanderer::update() {
    // TODO: wander/chase behaviour
}

void EnemyWanderer::reset(bn::fixed_point position, int time_budget) {
    _position    = position;
    _time_budget = time_budget;
    _ai_timer    = 0;
    _behavior    = AIBehavior::Wander;
    _alive       = true;
}

} // namespace game
