#pragma once

#include "engine/object_manager.h"

namespace engine {

class CollisionSystem {
public:
    static void resolve(ObjectManager& manager);
};

} // namespace engine
