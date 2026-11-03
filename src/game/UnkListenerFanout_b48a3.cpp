// Unit 2514 (tools/tu_map.py, no name hint): passes `arg` to the virtual at slot 7 of every entry of the
// inline array at +0xac (count +0xa8, re-read every step; the array is only 4-byte aligned). Names
// and layout are placeholders.
#include "types.h"

class UnkFanoutTarget_b48a3 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38(void* arg);
};

struct __attribute__((packed, aligned(4))) UnkFanoutSlot_b48a3 {
    UnkFanoutTarget_b48a3* target;
};

struct UnkFanout_b48a3 {
    u8 pad_0x0[0xa8];
    u32 count;
    UnkFanoutSlot_b48a3 slots[1];
};

void fn_7100d6229c(UnkFanout_b48a3* self, void* arg) {
    if (self->count == 0)
        return;
    u64 i = 0;
    do {
        u64 next = i + 1;
        self->slots[i].target->vf_0x38(arg);
        i = next;
    } while (i < self->count);
}
