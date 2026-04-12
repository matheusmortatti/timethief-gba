#pragma once

#include "engine/scene.h"
#include "engine/object_manager.h"
#include "game/game_state.h"

namespace game {

class GameplayScene : public engine::Scene {
public:
    explicit GameplayScene(GameState& state);

    void enter() override;
    void exit()  override;
    [[nodiscard]] bn::optional<engine::SceneId> update() override;

private:
    GameState&           _state;
    engine::ObjectManager _objects;
};

} // namespace game
