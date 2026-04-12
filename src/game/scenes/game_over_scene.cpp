#include "game/scenes/game_over_scene.h"

namespace game {

GameOverScene::GameOverScene(const GameState& state)
    : _state(state)
{
}

void GameOverScene::enter() {
    // TODO: show game over screen, display score
}

void GameOverScene::exit() {
    // TODO: release assets
}

bn::optional<engine::SceneId> GameOverScene::update() {
    // TODO: on press, return SceneId::Title
    return bn::nullopt;
}

} // namespace game
