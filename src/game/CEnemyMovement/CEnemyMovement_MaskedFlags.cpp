// CEnemyMovement unit: for kind 2, the other object's flags (vtable slot 0x58) when they share a bit
// with the movement's mask at +0x3d0; otherwise the movement's own answer (vtable slot 0x3a8).
// Names and layouts are placeholders.
#include "types.h"
struct UnkEnemyMovementOther {
    void** vtable;
    u32 vf_0x58() { return reinterpret_cast<u32 (*)(UnkEnemyMovementOther*)>(vtable[0x58 / 8])(this); }
};
struct UnkEnemyMovement {
    void** vtable;
    u8 pad_0x8[0x3c8];
    u32 mask;  // 0x3d0
    u32 vf_0x3a8() { return reinterpret_cast<u32 (*)(UnkEnemyMovement*)>(vtable[0x3a8 / 8])(this); }
};
u32 fn_71010cc9a4(UnkEnemyMovement* self, UnkEnemyMovementOther* other, u32 kind) {
    if (kind == 2) {
        u32 flags;
        if (self->mask & (flags = other->vf_0x58()))
            return flags;
    }
    return self->vf_0x3a8();
}
