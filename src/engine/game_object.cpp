#include "engine/game_object.h"

namespace engine {

GameObject::GameObject(bn::fixed_point position,
                       int8_t half_w, int8_t half_h,
                       CollisionLayer layer, CollisionLayer mask)
    : _position(position),
      _velocity(0, 0),
      _half_w(half_w),
      _half_h(half_h),
      _layer(layer),
      _mask(mask)
{
}

void GameObject::draw() {
    if (_sprite.has_value()) {
        _sprite->set_x(_position.x());
        _sprite->set_y(_position.y());
    }
}

void GameObject::on_collision([[maybe_unused]] GameObject& other) {
}

} // namespace engine
