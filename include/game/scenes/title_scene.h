#pragma once

#include "engine/scene.h"

namespace game {

class TitleScene : public engine::Scene {
public:
    void enter() override;
    void exit()  override;
    [[nodiscard]] bn::optional<engine::SceneId> update() override;
};

} // namespace game
