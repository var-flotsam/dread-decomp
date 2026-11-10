// Unit 2524 (no reliable name): a table of frame-timed slots. A count followed by inline slots, each a
// growable array of pointers, and one more array of pointers at +8. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkSlotTable_b0225 {
    u32 id;                              // 0x0
    f32 total_time;                      // 0x4
    base::UnkVector<void*> all;          // 0x8
    u32 slot_count;                      // 0x30
    base::UnkVector<void*> slots[1];     // 0x34, inline
} __attribute__((packed, aligned(4)));

// Frees every slot's array and the main one.
void fn_7100d8b7e4(UnkSlotTable_b0225* self) {
    for (u32 i = 0; i < self->slot_count; ++i)
        self->slots[i].Free();
    self->slot_count = 0;
    self->all.Free();
}
