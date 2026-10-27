// From unit 237 of tools/tu_map.py (0x71001afb24-0x71001b1228), msapi::res::CProgramResource's
// unit (its static initialiser is src/game_s1/fn_71001b10b0.cpp): a program resource's variants.
// The resource keeps a table of (key, variant) pairs whose first entry is the fallback, and a
// variant for when the table is empty. Names and layouts are placeholders.
#include "types.h"

struct UnkProgramVariant {
    u32 field_0x0;
    s32 key;
    u32 variant;
};

struct UnkProgramResource {
    u8 pad_0x0[0x1280];
    u32 variant_count;
    u32 default_variant;  // when the table is empty
    u8 pad_0x1288[0x8];
    UnkProgramVariant* variants;
};
static_assert(__builtin_offsetof(UnkProgramResource, variants) == 0x1290, "UnkProgramResource layout");

// The variant for `key`: the last entry with that key, searched down to the second; the first
// entry otherwise.
// NON_MATCHING(minor): register allocation; the original keeps the next index (i - 1) live and
// rebuilds the index from it at the exit (one more add, and the entry size materialised again).
u32 fn_71001b0f24(const UnkProgramResource* self, s32 key) {
    for (u32 i = self->variant_count - 1; i < self->variant_count; --i)
        if (i == 0 || self->variants[i].key == key)
            return self->variants[i].variant;
    return self->default_variant;
}
