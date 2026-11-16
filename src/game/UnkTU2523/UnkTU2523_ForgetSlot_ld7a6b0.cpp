// UnkTU2523 (tools/tu_map.py unit 2523): clears every one of an object's 42 pointer slots (at +0x1b8) that
// points at a given object. Names and layouts are placeholders.
#include "types.h"

struct UnkSlotOwner_ld7a6b0 {
    u8 pad_0x0[0x1b8];
    void* slots[42];  // 0x1b8
};

void fn_7100d7a6b0(UnkSlotOwner_ld7a6b0* self, void* object) {
    if (object == nullptr)
        return;
    for (u32 i = 0; i < 42; ++i) {
        if (self->slots[i] == object)
            self->slots[i] = nullptr;
    }
}
