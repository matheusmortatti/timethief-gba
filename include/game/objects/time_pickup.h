#pragma once

#include "engine/game_object.h"

namespace game {

class TimePickup : public engine::GameObject {
public:
    explicit TimePickup(bn::fixed_point position, int time_value);

    void update() override;
    void on_collision(engine::GameObject& other) override;

    [[nodiscard]] int time_value() const { return _time_value; }

private:
    int _time_value;
};

} // namespace game
