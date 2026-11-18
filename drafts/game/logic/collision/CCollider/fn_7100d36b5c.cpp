// game::logic::collision::CCollider (unit 2503 of tools/tu_map.py): rebuilds the collider's 2D bounds
// from its shape placed with its transform (or, with no shape, from the transform's position), grown
// over the old bounds when keep_bounds is set, then updates every attached child (slot 0x38).
// Names and layouts are placeholders.
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

void fn_7100d35aec(void* self);

struct UnkBox_ovb0066 {
    f32 min_x, min_y;
    f32 max_x, max_y;

    UnkBox_ovb0066& operator=(const UnkBox_ovb0066& o) {
        min_x = o.min_x;
        min_y = o.min_y;
        max_x = o.max_x;
        max_y = o.max_y;
        return *this;
    }
};

struct UnkCollider_ld36b5c {
    void** vtable;
    u8 pad_0x8[0x34];
    bool keep_bounds;  // 0x3c
    u8 pad_0x3d[0x3];
    UnkBox_ovb0066 bounds;       // 0x40
    UnkBox_ovb0066 last_bounds;  // 0x50
    u8 pad_0x60[0x9];
    bool bounds_reset;  // 0x69
    u8 pad_0x6a[0x2];
    UnkMtx44 transform;  // 0x6c
    u8 pad_0xac[0x158 - 0xac];
    UnkShape* shape;  // 0x158
    u8 pad_0x160[0x4];
    u32 count;                    // 0x164
    u8 children[8];  // 0x168, raw storage

    UnkBvhCollider** begin() { return reinterpret_cast<UnkBvhCollider**>(children); }
    UnkBvhCollider** end() { return begin() + count; }
};
static_assert(__builtin_offsetof(UnkCollider_ld36b5c, children) == 0x168, "UnkCollider_ld36b5c layout");

void fn_7100d36b5c(UnkCollider_ld36b5c* self) {
    fn_7100d35aec(self);
    self->last_bounds = self->bounds;
    UnkShape* shape = self->shape;
    if (!shape) {
        const f32* m = self->transform.m;
        f32 x = m[3], y = m[7], z = m[11];
        Vec3 pos = {x, y, z};
        Vec2 p(pos);
        self->bounds.min_x = p.x;
        self->bounds.min_y = p.y;
        self->bounds.max_x = p.x;
        self->bounds.max_y = p.y;
    } else if (self->keep_bounds) {
        UnkBox box(shape->vf_0x28(self->transform * shape->transform));
        UnkBox_ovb0066& b = self->bounds;
        f32 cx = box.min.x, cy = box.min.y, bx = b.min_x, by = b.min_y;
        b.min_x = bx < cx ? bx : cx;
        b.min_y = by < cy ? by : cy;
        cx = box.max.x, cy = box.max.y, bx = b.max_x, by = b.max_y;
        b.max_x = bx > cx ? bx : cx;
        b.max_y = by > cy ? by : cy;
    } else {
        UnkBox box(shape->vf_0x28(self->transform * shape->transform));
        self->bounds.min_x = box.min.x;
        self->bounds.min_y = box.min.y;
        self->bounds.max_x = box.max.x;
        self->bounds.max_y = box.max.y;
    }
    if (self->bounds_reset) {
        self->last_bounds = self->bounds;
        self->bounds_reset = false;
    }
    for (UnkBvhCollider** it = self->begin(); it != self->end(); ++it)
        (*it)->vf_0x38();
}
