#pragma once

#include "engine/scene.h"
#include "bn_unique_ptr.h"

namespace engine {

class SceneManager {
public:
    void set_scene(bn::unique_ptr<Scene> scene);
    void update();

private:
    bn::unique_ptr<Scene>    _current;
    bn::optional<SceneId>    _pending;
};

} // namespace engine
