#include "game/scenes/title_scene.h"

namespace game {

void TitleScene::enter() {
    // TODO: show title background / text
}

void TitleScene::exit() {
    // TODO: release title assets
}

bn::optional<engine::SceneId> TitleScene::update() {
    // TODO: on Start press, return SceneId::Gameplay
    return bn::nullopt;
}

} // namespace game
