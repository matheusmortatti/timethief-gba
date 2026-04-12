#pragma once

#include "bn_optional.h"

namespace engine {

enum class SceneId : uint8_t {
    Title,
    Gameplay,
    GameOver,
};

class Scene {
public:
    virtual ~Scene() = default;

    virtual void enter() {}
    virtual void exit()  {}
    [[nodiscard]] virtual bn::optional<SceneId> update() = 0;

protected:
    Scene() = default;
};

} // namespace engine
