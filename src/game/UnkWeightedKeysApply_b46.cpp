// A small unit of its own (tools/tu_map.py unit 2199): passes each key of a list (8-byte entries,
// +0x20, count at +0x28) with its weight (+0x0) on to fn_71006cc484 on the owner's object
// (+0x30, its +0x220, at +0x80); a pending flag (+0x38) stops it through fn_7100c812e0, which
// does not return. Names and layouts are placeholders.
#include "types.h"

struct UnkWeightTarget_b46a3;
struct UnkWeightKey_b46a3 {
    u64 value;
};

void fn_71006cc484(UnkWeightTarget_b46a3* target, const UnkWeightKey_b46a3* key, f32 weight);
[[noreturn]] void fn_7100c812e0();

struct UnkWeightOwner_b46a3 {
    u8 pad_0x0[0x220];
    char* field_0x220;
};

struct UnkWeightedKeys_b46a3 {
    f32* weights;  // 0x0
    u8 pad_0x8[0x18];
    UnkWeightKey_b46a3* keys;  // 0x20
    u64 count;                 // 0x28
    UnkWeightOwner_b46a3* owner;  // 0x30
    u32 pending;               // 0x38
};

void fn_7100c81264(UnkWeightedKeys_b46a3* self) {
    if (self->pending)
        fn_7100c812e0();
    if (!self->count)
        return;
    UnkWeightTarget_b46a3* target = reinterpret_cast<UnkWeightTarget_b46a3*>(self->owner->field_0x220 + 0x80);
    for (u64 i = 0; self->count > i; ++i)
        fn_71006cc484(target, &self->keys[i], self->weights[i]);
}
