#include "engine/input_manager.h"

namespace engine {

void InputManager::update() {
    // bn::keypad updates automatically each frame via bn::core::update()
}

bool InputManager::held(bn::keypad::key_type key) {
    return bn::keypad::held(key);
}

bool InputManager::just_pressed(bn::keypad::key_type key) {
    return bn::keypad::pressed(key);
}

bool InputManager::just_released(bn::keypad::key_type key) {
    return bn::keypad::released(key);
}

} // namespace engine
