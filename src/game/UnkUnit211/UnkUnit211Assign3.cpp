// From unit 211 of tools/tu_map.py, beside UnkUnit211Assign.cpp: the same pass for 12-byte entries
// of the values' table (+0x20), set to the three words held at +0x10 of the pass, word by word as raw
// copies (each word reread after every store). Names and layouts are placeholders.
#include "types.h"

struct UnkUnit211Triple {
    u32 a;
    u32 b;
    u32 c;
};

struct UnkUnit211TripleValues {
    u8 pad_0x0[0x20];
    UnkUnit211Triple* triples;  // 0x20
};

struct UnkUnit211TriplePass {
    void* field_0x0;
    UnkUnit211TripleValues* values;  // 0x8
    u32 a;                           // 0x10
    u32 b;                           // 0x14
    u32 c;                           // 0x18
};

void fn_71001942b0(UnkUnit211TriplePass* self, const u32* indices, u32 count) {
    for (; count != 0; --count) {
        u32 index = *indices++;
        UnkUnit211Triple* e = &self->values->triples[index];
        __builtin_memcpy(&e->a, &self->a, sizeof(e->a));
        __builtin_memcpy(&e->b, &self->b, sizeof(e->b));
        __builtin_memcpy(&e->c, &self->c, sizeof(e->c));
    }
}
