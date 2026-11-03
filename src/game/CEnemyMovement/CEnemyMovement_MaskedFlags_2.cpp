// Copy of fn_71010cc9a4 (CEnemyMovement_MaskedFlags.cpp) reached through the secondary base at +0x6e8:
// the this-adjusting entry with the body repeated. Names and layouts are placeholders.
#include "types.h"
struct UnkEnemyMovementOther2 {
    void** vtable;
    u32 vf_0x58() { return reinterpret_cast<u32 (*)(UnkEnemyMovementOther2*)>(vtable[0x58 / 8])(this); }
};
struct UnkEnemyMovement2 {
    void** vtable;
    u8 pad_0x8[0x3c8];
    u32 mask;  // 0x3d0
    u32 vf_0x3a8() { return reinterpret_cast<u32 (*)(UnkEnemyMovement2*)>(vtable[0x3a8 / 8])(this); }
};
u32 fn_71010cc9fc(char* base, UnkEnemyMovementOther2* other, u32 kind) {
    UnkEnemyMovement2* self = reinterpret_cast<UnkEnemyMovement2*>(base - 0x6e8);
    asm("" : "+r"(self), "+r"(kind));
    if (kind == 2) {
        u32 flags;
        if (self->mask & (flags = other->vf_0x58()))
            return flags;
    }
    return self->vf_0x3a8();
}
