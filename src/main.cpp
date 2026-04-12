#include "bn_core.h"

#include "engine/scene_manager.h"
#include "game/game_state.h"
#include "game/scenes/title_scene.h"

int main() {
    bn::core::init();

    game::GameState state;
    engine::SceneManager scene_manager;
    scene_manager.set_scene(bn::make_unique<game::TitleScene>());

    while (true) {
        scene_manager.update();
        bn::core::update();
    }
}
