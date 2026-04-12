#include "engine/collision_system.h"

namespace engine {

bool CollisionSystem::aabb_overlap(const GameObject& a, const GameObject& b) {
    bn::fixed ax = a.position().x(), ay = a.position().y();
    bn::fixed bx = b.position().x(), by = b.position().y();
    return bn::abs(ax - bx) < (a.half_w() + b.half_w()) &&
           bn::abs(ay - by) < (a.half_h() + b.half_h());
}

void CollisionSystem::resolve(ObjectManager& manager) {
    GameObject* const* begin = manager.begin();
    GameObject* const* end   = manager.end();

    for (GameObject* const* i = begin; i != end; ++i) {
        if (!*i || !(*i)->is_alive()) continue;
        for (GameObject* const* j = i + 1; j != end; ++j) {
            if (!*j || !(*j)->is_alive()) continue;

            GameObject& a = **i;
            GameObject& b = **j;

            bool a_hits_b = (a.mask() & b.layer()) != CollisionLayer::None;
            bool b_hits_a = (b.mask() & a.layer()) != CollisionLayer::None;

            if ((a_hits_b || b_hits_a) && aabb_overlap(a, b)) {
                if (a_hits_b) a.on_collision(b);
                if (b_hits_a) b.on_collision(a);
            }
        }
    }
}

} // namespace engine
