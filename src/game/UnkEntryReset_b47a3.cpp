// Unit near 0x7100d8bb70 (tools/tu_map.py, no name hint): clears +0x10 and the field at +8 of each of the
// 0x28-byte entries of the inline array at +0x30 (a count followed by the entries). Names and layouts are
// placeholders.
#include "types.h"

struct UnkResetEntry_b47a3 {
    u8 unk_0x0[8];
    u32 value;
    u8 unk_0xc[0x28 - 0xc];
};

struct UnkResetArray_b47a3 {
    u32 count;
    UnkResetEntry_b47a3 entries[1];

    UnkResetEntry_b47a3* begin() { return entries; }
    UnkResetEntry_b47a3* end() {
        return reinterpret_cast<UnkResetEntry_b47a3*>(reinterpret_cast<char*>(begin()) + count * sizeof(UnkResetEntry_b47a3));
    }
};

struct UnkResetHolder_b47a3 {
    u8 unk_0x0[0x10];
    u32 field_0x10;
    u8 unk_0x14[0x30 - 0x14];
    UnkResetArray_b47a3 array;
};

void fn_7100d8bb70(UnkResetHolder_b47a3* self) {
    self->field_0x10 = 0;
    UnkResetEntry_b47a3* first = self->array.begin();
    asm("" : "+r"(first));
    for (UnkResetEntry_b47a3* it = first;
         it != reinterpret_cast<UnkResetEntry_b47a3*>(reinterpret_cast<char*>(first) + self->array.count * sizeof(UnkResetEntry_b47a3));
         ++it)
        *reinterpret_cast<u32*>(reinterpret_cast<char*>(it) + 8) = 0;
}
