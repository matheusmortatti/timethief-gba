#pragma once

#include "engine/game_object.h"

namespace game {

enum class AIBehavior : uint8_t {
    Idle,
    Wander,
    Chase,
    Flee,
};

class EnemyBase : public engine::GameObject {
public:
    void update() override;
    void on_collision(engine::GameObject& other) override;

    [[nodiscard]] int time_budget() const { return _time_budget; }
    void steal_time(int amount);

protected:
    EnemyBase(bn::fixed_point position, int time_budget);

    AIBehavior _behavior     = AIBehavior::Wander;
    int        _time_budget  = 0;
    int        _ai_timer     = 0;
};

} // namespace game
