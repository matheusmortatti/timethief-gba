#include "game/scenes/gameplay_scene.h"
#include "engine/collision_system.h"

namespace game {

GameplayScene::GameplayScene(GameState& state)
    : _state(state)
{
}

void GameplayScene::enter() {
    // TODO: spawn player, enemies, register into _objects
}

void GameplayScene::exit() {
    // TODO: release assets
}

bn::optional<engine::SceneId> GameplayScene::update() {
    _objects.update_all();
    engine::CollisionSystem::resolve(_objects);
    _objects.draw_all();
    _objects.purge_dead();

    // TODO: check win/lose conditions and return SceneId::GameOver
    return bn::nullopt;
}

} // namespace game
