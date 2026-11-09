// game::logic::collision::CCollider's unit (tools/tu_map.py): a collider's two matrices (+0x6c and
// +0xac) in world space: times the parent's (its +0x8) when there is a parent (+0x158), as they are
// otherwise. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "types.h"

struct UnkColliderParent_A5 {
    u8 pad_0x0[0x8];
    UnkMatrix44 world;
};

struct UnkCollider_A5 {
    u8 pad_0x0[0x6c];
    UnkMatrix44 local_a;  // 0x6c
    UnkMatrix44 local_b;  // 0xac
    u8 pad_0xec[0x6c];
    UnkColliderParent_A5* parent;  // 0x158
};
static_assert(__builtin_offsetof(UnkCollider_A5, parent) == 0x158, "UnkCollider_A5 layout");

static inline UnkMatrix44 Copy(const UnkMatrix44& m) {
    UnkMatrix44 r;
    UnkCopyMatrix(&r, m);
    return r;
}

UnkMatrix44 fn_7100d36a2c(UnkCollider_A5* self) {
    if (self->parent)
        return fn_71000127c8(self->local_a, self->parent->world);
    return Copy(self->local_a);
}

UnkMatrix44 fn_7100d36ac4(UnkCollider_A5* self) {
    if (self->parent)
        return fn_71000127c8(self->local_b, self->parent->world);
    return Copy(self->local_b);
}
