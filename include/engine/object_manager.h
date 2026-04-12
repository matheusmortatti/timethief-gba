#pragma once

#include "engine/game_object.h"

namespace engine {

class ObjectManager {
public:
    static constexpr int MAX_OBJECTS = 32;

    void add(GameObject* obj);
    void remove(GameObject* obj);
    void update_all();
    void draw_all();
    void purge_dead();

    [[nodiscard]] int count() const { return _count; }

    GameObject* const* begin() const { return _objects; }
    GameObject* const* end()   const { return _objects + MAX_OBJECTS; }

private:
    GameObject* _objects[MAX_OBJECTS] = {};
    int         _count = 0;
};

} // namespace engine
