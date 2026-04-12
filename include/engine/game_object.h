#pragma once

#include "bn_fixed_point.h"
#include "bn_optional.h"
#include "bn_sprite_ptr.h"

namespace engine {

enum class CollisionLayer : uint8_t {
    None   = 0,
    Player = 1 << 0,
    Enemy  = 1 << 1,
    Weapon = 1 << 2,
    Wall   = 1 << 3,
};

inline constexpr CollisionLayer operator|(CollisionLayer a, CollisionLayer b) {
    return static_cast<CollisionLayer>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}
inline constexpr CollisionLayer operator&(CollisionLayer a, CollisionLayer b) {
    return static_cast<CollisionLayer>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

class GameObject {
public:
    virtual ~GameObject() = default;

    virtual void update() = 0;
    virtual void draw();
    virtual void on_collision(GameObject& other);

    [[nodiscard]] const bn::fixed_point& position() const { return _position; }
    [[nodiscard]] const bn::fixed_point& velocity() const { return _velocity; }
    void set_position(const bn::fixed_point& p) { _position = p; }
    void set_velocity(const bn::fixed_point& v) { _velocity = v; }

    [[nodiscard]] int8_t half_w() const { return _half_w; }
    [[nodiscard]] int8_t half_h() const { return _half_h; }

    [[nodiscard]] CollisionLayer layer() const { return _layer; }
    [[nodiscard]] CollisionLayer mask()  const { return _mask; }

    [[nodiscard]] bool is_alive() const { return _alive; }
    void destroy() { _alive = false; }

protected:
    GameObject(bn::fixed_point position,
               int8_t half_w, int8_t half_h,
               CollisionLayer layer, CollisionLayer mask);

    bn::fixed_point              _position;
    bn::fixed_point              _velocity;
    bn::optional<bn::sprite_ptr> _sprite;

    int8_t         _half_w = 8;
    int8_t         _half_h = 8;
    CollisionLayer _layer  = CollisionLayer::None;
    CollisionLayer _mask   = CollisionLayer::None;
    bool           _alive  = true;
};

} // namespace engine
