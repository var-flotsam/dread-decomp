// Unit 1385 (tools/tu_map.py, no name hint). fn_71008e8df4: pushes two of the object's values
// (+0xac, +0xb0) to its owner (+0x120): the first, at least zero, into the owner's +0x140 -> +0x78 object
// (+0x7c); the second, at least 5, through fn_710020c9ac on the owner's member at +0x28. Names and
// layouts are placeholders.
#include "types.h"

void fn_710020c9ac(void* self, f32 value);

// The store may alias any other access: the owner pointer is read again after it.
typedef f32 __attribute__((may_alias)) UnkAliasF32_b43a2;

struct UnkTarget_1385_b43a2 {
    u8 pad_0x0[0x7c];
    f32 value;  // 0x7c
};

struct UnkInner_1385_b43a2 {
    u8 pad_0x0[0x78];
    UnkTarget_1385_b43a2* target;  // 0x78
};

struct UnkOwner_1385_b43a2 {
    u8 pad_0x0[0x28];
    u8 member[0x118];  // 0x28
    UnkInner_1385_b43a2* inner;  // 0x140
};

struct UnkSelf_1385_b43a2 {
    u8 pad_0x0[0xac];
    f32 a;  // 0xac
    f32 b;  // 0xb0
    u8 pad_0xb4[0x6c];
    UnkOwner_1385_b43a2* owner;  // 0x120
};

static inline f32 Max_b43a2(f32 x, f32 y) { return x < y ? y : x; }

void fn_71008e8df4(UnkSelf_1385_b43a2* self) {
    f32 a = self->a;
    f32 b = self->b;
    *reinterpret_cast<UnkAliasF32_b43a2*>(&self->owner->inner->target->value) = Max_b43a2(a, 0.0f);
    fn_710020c9ac(self->owner->member, Max_b43a2(b, 5.0f));
}
