// From unit 211 of tools/tu_map.py, beside UnkUnit211Scale.cpp and UnkUnit211Accumulate.cpp: a pass
// over a list of indices that sets each 8-byte entry of the values' table (+0x18) to the pair held at
// +0x10 of the pass, word by word as raw copies (each word reread after
// every store, as in the original). Names and layouts are placeholders.
#include "types.h"

struct UnkUnit211AssignPair {
    u32 a;
    u32 b;
};

struct UnkUnit211AssignValues {
    u8 pad_0x0[0x18];
    UnkUnit211AssignPair* pairs;  // 0x18
};

struct UnkUnit211AssignPass {
    void* field_0x0;
    UnkUnit211AssignValues* values;  // 0x8
    u32 a;          // 0x10
    u32 b;          // 0x14
};

void fn_7100193c40(UnkUnit211AssignPass* self, const u32* indices, u32 count) {
    for (; count != 0; --count) {
        u32 index = *indices++;
        UnkUnit211AssignPair* e = &self->values->pairs[index];
        __builtin_memcpy(&e->a, &self->a, sizeof(e->a));
        __builtin_memcpy(&e->b, &self->b, sizeof(e->b));
    }
}
