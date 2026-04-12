#include "engine/object_manager.h"

namespace engine {

void ObjectManager::add(GameObject* obj) {
    for (int i = 0; i < MAX_OBJECTS; ++i) {
        if (!_objects[i]) {
            _objects[i] = obj;
            ++_count;
            return;
        }
    }
}

void ObjectManager::remove(GameObject* obj) {
    for (int i = 0; i < MAX_OBJECTS; ++i) {
        if (_objects[i] == obj) {
            _objects[i] = nullptr;
            --_count;
            return;
        }
    }
}

void ObjectManager::update_all() {
    for (int i = 0; i < MAX_OBJECTS; ++i) {
        if (_objects[i] && _objects[i]->is_alive()) {
            _objects[i]->update();
        }
    }
}

void ObjectManager::draw_all() {
    for (int i = 0; i < MAX_OBJECTS; ++i) {
        if (_objects[i] && _objects[i]->is_alive()) {
            _objects[i]->draw();
        }
    }
}

void ObjectManager::purge_dead() {
    for (int i = 0; i < MAX_OBJECTS; ++i) {
        if (_objects[i] && !_objects[i]->is_alive()) {
            _objects[i] = nullptr;
            --_count;
        }
    }
}

} // namespace engine
