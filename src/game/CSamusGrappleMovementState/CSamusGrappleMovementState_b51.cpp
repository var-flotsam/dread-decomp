// CSamusGrappleMovementState (unit 1461).
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"

using base::reflection::UnkReflObject;

struct UnkGrappleSubState {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(UnkReflObject* event);
    u8 pad_0x8[0x8];
    u32 kind;  // 0x10
};

// The sub-states follow their count directly (4-byte aligned).
typedef UnkGrappleSubState* __attribute__((aligned(4))) UnkGrappleSubStatePtr;

struct UnkGrappleState {
    u8 pad_0x0[0x1a8];
    u32 count;
    UnkGrappleSubStatePtr states[1];  // 0x1ac
};

// Passes a CCharClassGrappleFinalPullEndEvent on to the first sub-state of kind 4.
void fn_710099a478(UnkGrappleState* self, UnkReflObject* event) {
    if (!fn_710009d50c(event->vf_0x0(), fn_7100cc21a8()))
        return;
    // The search runs over the byte size of the array.
    u64 n = (u64)self->count * sizeof(UnkGrappleSubStatePtr);
    asm("" : "+r"(n));
    if (n == 0)
        return;
    UnkGrappleSubStatePtr* it = self->states;
    while ((*it)->kind != 4) {
        ++it;
        n -= 8;
        if (n == 0)
            return;
    }
    if (UnkGrappleSubState* s = *it)
        s->vf_0x48(event);
}
