#pragma once

#include "engine/game_object.h"

namespace game {

class Player : public engine::GameObject {
public:
    Player();

    void update() override;
    void on_collision(engine::GameObject& other) override;

    [[nodiscard]] int time_remaining() const { return _time_remaining; }

private:
    int _time_remaining;
};

} // namespace game
