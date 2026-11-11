// CChozoCommanderHypersparkAttack's unit (tools/tu_map.py): two identical constructors of attack
// objects on the same base (fn_710042baa8): the vtable, a cleared run at +0x5c-0x70 and an embedded
// listener at +0x78 (vtable g_unk_7101a22d48) with its fields cleared. Names and layouts are
// placeholders.
#include "game/CChozoCommanderGroundAttack.h"
#include "types.h"

extern void* g_unk_7101a358f8[];
extern void* g_unk_7101a22d48[];  // the listener's vtable

struct UnkHypersparkListener_b46a1 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct UnkHypersparkAttack_b46a1 {
    void** vtable;
    u8 pad_0x8[0x54];
    u8 field_0x5c[0xd];
    u8 pad_0x69[3];
    u32 field_0x6c;
    u8 pad_0x70[8];
    UnkHypersparkListener_b46a1 listener;
};

static inline __attribute__((always_inline)) void Construct(UnkHypersparkAttack_b46a1* self) {
    fn_710042baa8(self);
    self->vtable = &g_unk_7101a358f8[2];
    self->field_0x6c = 0;
    __builtin_memset(self->field_0x5c, 0, sizeof(self->field_0x5c));
    self->listener.vtable = &g_unk_7101a22d48[2];
    self->listener.field_0x8 = 0;
    self->listener.field_0x10 = 0;
    self->listener.field_0x18 = 0;
}

void fn_710042c930(UnkHypersparkAttack_b46a1* self) { Construct(self); }
void fn_710042de9c(UnkHypersparkAttack_b46a1* self) { Construct(self); }
