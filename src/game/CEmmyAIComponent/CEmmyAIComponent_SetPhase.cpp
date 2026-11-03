// CEmmyAIComponent's unit (tools/tu_map.py unit 811): stores the phase at +0x14b48 and enters it:
// phase 1 calls vtable slot 249 with 0, phase 2 slot 248; other phases call slot 250. Names are
// placeholders.
#include "types.h"

struct UnkEmmyVtable811 {
    void* slots[248];
    void (*vf_0x7c0)(void* self);
    void (*vf_0x7c8)(void* self, u32 arg);
    void (*vf_0x7d0)(void* self);
};

struct UnkEmmy811 {
    UnkEmmyVtable811* vtable;
    u8 pad_0x8[0x14b48 - 0x8];
    s32 phase;  // 0x14b48
};

void fn_71005480dc(UnkEmmy811* self, s32 phase) {
    self->phase = phase;
    switch (phase) {
    case 2:
        self->vtable->vf_0x7c0(self);
        break;
    case 1:
        self->vtable->vf_0x7c8(self, 0);
        break;
    default:
        self->vtable->vf_0x7d0(self);
        break;
    }
}
