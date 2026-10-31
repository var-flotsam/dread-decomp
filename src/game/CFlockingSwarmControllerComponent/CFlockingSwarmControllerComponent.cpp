// In the CFlockingSwarmControllerComponent unit: destroys the 0x20-byte polymorphic elements of the
// array at +0x2d0 (through vtable slot 1), empties it and sets the flag at +0x395. Names and
// layouts are placeholders.
#include "types.h"

struct UnkSwarmElement {
    void (**vtable)(UnkSwarmElement*);
    u8 pad_0x8[0x18];
};

struct UnkSwarmController {
    u8 pad_0x0[0x2d0];
    UnkSwarmElement* elements;
    u32 count;
    u8 pad_0x2dc[0xb9];
    bool field_0x395;
};

void fn_710073d958(UnkSwarmController* self) {
    for (u64 i = 0; i < self->count; ++i)
        self->elements[i].vtable[1](&self->elements[i]);
    self->count = 0;
    self->field_0x395 = true;
}
