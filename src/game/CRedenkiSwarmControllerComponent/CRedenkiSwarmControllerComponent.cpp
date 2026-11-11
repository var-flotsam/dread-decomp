// In the CRedenkiSwarmControllerComponent unit: fn_710064f3b4 resets the controller. It runs
// fn_7100747f74, clears the word at +0x530, calls fn_710073f7bc with 0, clears the word at +0x538,
// then runs vf_0x8 on each 0x20-byte entry of the array at +0x2d0 (count at +0x2d8) and empties it.
// Names and layouts are placeholders.
#include "game/CFlockingSwarmControllerComponent.h"
#include "types.h"

struct UnkRedenkiSwarmEntry {
    virtual void vf_0x0();
    virtual void vf_0x8();
    u8 pad_0x8[0x18];
};

struct UnkRedenkiSwarmController {
    u8 pad_0x0[0x2d0];
    UnkRedenkiSwarmEntry* entries;
    u32 count;
    u8 pad_0x2dc[0x530 - 0x2dc];
    u32 field_0x530;
    u8 pad_0x534[0x4];
    u32 field_0x538;
};

void fn_7100747f74(UnkRedenkiSwarmController* self);

void fn_710064f3b4(UnkRedenkiSwarmController* self) {
    fn_7100747f74(self);
    self->field_0x530 = 0;
    fn_710073f7bc(self, 0);
    self->field_0x538 = 0;
    for (u64 i = 0; i < self->count; ++i)
        self->entries[i].vf_0x8();
    self->count = 0;
}
