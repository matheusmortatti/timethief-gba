#include "engine/scene_manager.h"

// TODO: include concrete scene headers and implement scene factory

namespace engine {

void SceneManager::set_scene(bn::unique_ptr<Scene> scene) {
    if (_current) {
        _current->exit();
    }
    _current = bn::move(scene);
    if (_current) {
        _current->enter();
    }
}

void SceneManager::update() {
    if (!_current) return;

    bn::optional<SceneId> next = _current->update();

    if (next.has_value()) {
        // TODO: build next scene from SceneId and call set_scene()
    }
}

} // namespace engine
