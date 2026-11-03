// CAbilitySonar's unit (tools/tu_map.py): clears the word at +0x3d0, calls the virtual at +0x20 of
// every object in the inline pointer list at +0x3d8 (count at +0x3d4) and empties the list. Names
// and layouts are placeholders.
#include "types.h"

struct UnkSonarItem_b46a1 {
    struct VTable {
        void* slots[4];
        void (*vf_0x20)(UnkSonarItem_b46a1* self);
    };
    VTable* vtable;
};

struct UnkAbilitySonar_b46a1 {
    u8 pad_0x0[0x3d0];
    u32 field_0x3d0;
    u32 count;
    UnkSonarItem_b46a1* items[1];
};

void fn_7100336474(UnkAbilitySonar_b46a1* self) {
    self->field_0x3d0 = 0;
    u64 bytes = (u64)self->count * 8;
    asm("" : "+r"(bytes));  // the original tests the byte size, not the count, before the loop
    for (u64 offset = 0; offset != bytes; offset += 8) {
        UnkSonarItem_b46a1* item = *reinterpret_cast<UnkSonarItem_b46a1**>(reinterpret_cast<char*>(self->items) + offset);
        item->vtable->vf_0x20(item);
    }
    self->count = 0;
}
