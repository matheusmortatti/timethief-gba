#pragma once

#include "game/objects/enemy_base.h"

namespace game {

class EnemyWanderer : public EnemyBase {
public:
    EnemyWanderer();

    void update() override;
    void reset(bn::fixed_point position, int time_budget);
};

} // namespace game
