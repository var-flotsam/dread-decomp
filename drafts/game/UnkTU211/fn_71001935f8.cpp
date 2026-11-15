// UnkTU211: writes one value into each slot of a table named by an index list.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSlotTable {
    u8 pad_0x0[0x18];
    u32* slots_0x18;
};

struct UnkSlotWriter {
    u8 pad_0x0[8];
    UnkSlotTable* table_0x8;
    u32 value_0x10;

    void Set(u32 index) { table_0x8->slots_0x18[index] = value_0x10; }
};

void fn_71001935f8(UnkSlotWriter* self, const u32* indices, u32 count) {
    const u32* end = indices + count;
    while (indices != end)
        self->Set(*indices++);
}
