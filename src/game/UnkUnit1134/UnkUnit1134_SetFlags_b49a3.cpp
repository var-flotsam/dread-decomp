// Unit 1134 of tools/tu_map.py (no name hint). fn_710070d6f8: sets or clears the bits of `mask` in the
// word at +0x170 and, when there is one, in the words at +0x50 and +0x98 of the object at +0x28. Names
// and layouts are placeholders.
#include "types.h"

struct UnkUnit1134Target_b49a3 {
    u8 pad_0x0[0x50];
    u32 field_0x50;
    u8 pad_0x54[0x44];
    u32 field_0x98;
};

struct UnkUnit1134_b49a3 {
    u8 pad_0x0[0x28];
    UnkUnit1134Target_b49a3* target;  // 0x28
    u8 pad_0x30[0x140];
    u32 field_0x170;
};

static inline u32 UnkUnit1134_Apply(u32 value, u32 mask, bool set) {
    u32 cleared = value & ~mask;
    u32 merged = mask | value;
    return set ? merged : cleared;
}

void fn_710070d6f8(UnkUnit1134_b49a3* self, const u32* mask, bool set) {
    self->field_0x170 = UnkUnit1134_Apply(self->field_0x170, *mask, set);
    if (UnkUnit1134Target_b49a3* target = self->target) {
        u32 m = *mask;
        target->field_0x50 = UnkUnit1134_Apply(target->field_0x50, m, set);
        target->field_0x98 = UnkUnit1134_Apply(target->field_0x98, m, set);
    }
}
