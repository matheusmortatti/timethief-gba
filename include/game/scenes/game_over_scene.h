#pragma once

#include "engine/scene.h"
#include "game/game_state.h"

namespace game {

class GameOverScene : public engine::Scene {
public:
    explicit GameOverScene(const GameState& state);

    void enter() override;
    void exit()  override;
    [[nodiscard]] bn::optional<engine::SceneId> update() override;

private:
    const GameState& _state;
};

} // namespace game
