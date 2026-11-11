// COmniLightComponent (unit 1390): after the base update, follows the owner's position (the
// translation column of its transform) and reports each change through vtable slot 0x168.
// Names and layouts are placeholders.
#include "game/CComponent.h"
#include "types.h"

struct UnkParent;

struct UnkOmniVec3 {
    f32 x, y, z;
};

// The position words, as the original reads them (integer loads).
struct UnkOmniOwner {
    u8 pad_0x0[0x44];
    u32 x;
    u8 pad_0x48[0xc];
    u32 y;
    u8 pad_0x58[0xc];
    u32 z;
};

static inline f32 AsF32(u32 bits) {
    f32 f;
    __builtin_memcpy(&f, &bits, 4);
    return f;
}

struct UnkOmniLight {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkOmniOwner* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkOmniVec3 pos;  // 0x40

    void vf_0x168(const UnkOmniVec3& p) {
        reinterpret_cast<void (*)(UnkOmniLight*, const UnkOmniVec3&)>(vtable[0x168 / 8])(this, p);
    }
};

#define UNK_CHANGED(a, b) ((a) - 0.001f > (b) || (a) + 0.001f < (b))

void fn_71008f18a4(UnkOmniLight* self) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    UnkOmniOwner* owner = self->owner;
    // The position is copied as words.
    u32 x = owner->x;
    u32 y = owner->y;
    u32 z = owner->z;
    alignas(8) u32 p[3];
    p[0] = x;
    p[1] = y;
    p[2] = z;
    if (UNK_CHANGED(AsF32(p[0]), self->pos.x) || UNK_CHANGED(AsF32(p[1]), self->pos.y) ||
        UNK_CHANGED(AsF32(p[2]), self->pos.z)) {
        self->vf_0x168(*reinterpret_cast<const UnkOmniVec3*>(p));
        u32* dst = reinterpret_cast<u32*>(&self->pos);
        dst[0] = p[0];
        dst[1] = p[1];
        dst[2] = p[2];
    }
}
