#pragma once

#include "bn_keypad.h"

namespace engine {

class InputManager {
public:
    static void update();

    [[nodiscard]] static bool held(bn::keypad::key_type key);
    [[nodiscard]] static bool just_pressed(bn::keypad::key_type key);
    [[nodiscard]] static bool just_released(bn::keypad::key_type key);
};

} // namespace engine
