// Functions in the CSamusMovement unit. Names and layouts are placeholders.
#include "types.h"

struct UnkSamusMovementState {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58(void* arg);
    u8 pad_0x8[0x8];
    s32 kind;
};
#pragma pack(push, 4)
struct UnkSamusMovement {
    u8 pad_0x0[0x2d18];
    u32 state_count;
    UnkSamusMovementState* states[1];  // 0x2d1c, 4-byte aligned
};
#pragma pack(pop)

static inline UnkSamusMovementState* FindState(UnkSamusMovement* self, s32 kind) {
    UnkSamusMovementState** it = self->states;
    u64 size = (u64)self->state_count * sizeof(*it);
    asm("" : "+r"(size));
    for (; size != 0; ++it, size -= sizeof(*it)) {
        if ((*it)->kind == kind)
            return *it;
    }
    return nullptr;
}

// Forwards the argument to the first state of kind 13 (virtual slot 11), if any.
void fn_71009ec054(UnkSamusMovement* self, void* arg) {
    if (UnkSamusMovementState* state = FindState(self, 13))
        state->vf_0x58(arg);
}
