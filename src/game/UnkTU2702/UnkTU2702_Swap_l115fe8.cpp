// Unit 2702: the swap of two std::function objects (an out-of-line instance of std::function::swap): objects
// held in the local buffer are cloned across, others only exchange their pointers. Names and layouts are
// placeholders.
#include "types.h"

// std::function's callable holder: slot 0x18 clones it into the given buffer, slot 0x20 destroys it in place.
struct UnkCallable_l115fe8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkCallable_l115fe8* dst);
    virtual void vf_0x20();
};

struct UnkFunction_l115fe8 {
    alignas(16) u8 buffer[0x20];
    UnkCallable_l115fe8* callable;  // 0x20

    UnkCallable_l115fe8* Local() { return reinterpret_cast<UnkCallable_l115fe8*>(this); }
};

void fn_7101115fe8(UnkFunction_l115fe8* self, UnkFunction_l115fe8* other) {
    if (other == self)
        return;
    if ((void*)self->callable == self->buffer && (void*)other->callable == other->buffer) {
        alignas(16) u8 temp[0x20];
        UnkCallable_l115fe8* t = reinterpret_cast<UnkCallable_l115fe8*>(temp);
        self->callable->vf_0x18(t);
        self->callable->vf_0x20();
        self->callable = nullptr;
        other->callable->vf_0x18(self->Local());
        other->callable->vf_0x20();
        other->callable = nullptr;
        self->callable = self->Local();
        t->vf_0x18(other->Local());
        t->vf_0x20();
        other->callable = other->Local();
    } else if ((void*)self->callable == self->buffer) {
        self->callable->vf_0x18(other->Local());
        self->callable->vf_0x20();
        self->callable = other->callable;
        other->callable = other->Local();
    } else if ((void*)other->callable == other->buffer) {
        other->callable->vf_0x18(self->Local());
        other->callable->vf_0x20();
        other->callable = self->callable;
        self->callable = self->Local();
    } else {
        UnkCallable_l115fe8* tmp = self->callable;
        self->callable = other->callable;
        other->callable = tmp;
    }
}
