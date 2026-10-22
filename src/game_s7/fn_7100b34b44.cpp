// Refills an object's list with the addresses of fourteen of its members. Layout is a
// placeholder; the members' types are unknown.
#include "base/UnkVector.h"
#include "types.h"

struct UnkObj_7100b34b44 {
    u8 pad_0x0[0x2b8];
    base::UnkVector<void*> list;
    u8 pad_0x2e0[0x300];

    void* At(u32 offset) { return reinterpret_cast<u8*>(this) + offset; }
};

// NON_MATCHING(minor): from the second push on, the pushed address and the new capacity swap
// registers (x21 and x22).
void fn_7100b34b44(UnkObj_7100b34b44* self) {
    self->list.count = 0;
    self->list.PushBack(self->At(0x2e0));
    self->list.PushBack(self->At(0x2f8));
    self->list.PushBack(self->At(0x340));
    self->list.PushBack(self->At(0x350));
    self->list.PushBack(self->At(0x398));
    self->list.PushBack(self->At(0x3b0));
    self->list.PushBack(self->At(0x3c0));
    self->list.PushBack(self->At(0x3d0));
    self->list.PushBack(self->At(0x4e8));
    self->list.PushBack(self->At(0x508));
    self->list.PushBack(self->At(0x518));
    self->list.PushBack(self->At(0x528));
    self->list.PushBack(self->At(0x3e0));
    self->list.PushBack(self->At(0x4d8));
}
