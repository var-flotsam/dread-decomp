// CSamusStandMovementState unit (tools/tu_map.py, unit 1470): entering the state. For kind 8 it takes
// +0x148 from the movement's first state of kind 8; then it arms the +0x18c/+0x190 pair and fires the
// movement's pending +0xf9 notification (slot 0x260). Names and layouts are placeholders.
#include "types.h"

struct UnkStandMoveState_A6_b70 {
    void* vtable;
    u8 pad_0x8[0x8];
    s32 kind;
    u8 pad_0x14[0x148 - 0x14];
    u32 field_0x148;
};

#pragma pack(push, 4)
struct UnkStandMove_A6_b70 {
    void** vtable;
    u8 pad_0x8[0xf9 - 0x8];
    bool pending;
    u8 pad_0xfa[0x2d18 - 0xfa];
    u32 state_count;
    UnkStandMoveState_A6_b70* states[1];  // 0x2d1c, 4-byte aligned
};
#pragma pack(pop)

struct UnkStandState_A6_b70 {
    u8 pad_0x0[0x38];
    UnkStandMove_A6_b70* movement;
    u8 pad_0x40[0x148 - 0x40];
    u32 field_0x148;
    u8 pad_0x14c[0x18c - 0x14c];
    bool armed;
    f32 timer;
};

void fn_710099d6f0(UnkStandState_A6_b70* self);

static inline UnkStandMoveState_A6_b70* FindState(UnkStandMove_A6_b70* self, s32 kind) {
    u64 size = (u64)self->state_count * sizeof(UnkStandMoveState_A6_b70*);
    asm("" : "+r"(size));
    if (size == 0)
        return nullptr;
    UnkStandMoveState_A6_b70** it = self->states;
    asm("" : "+r"(it));
    for (; size != 0; ++it, size -= sizeof(*it)) {
        if ((*it)->kind == kind)
            return *it;
    }
    return nullptr;
}

void fn_71009b3b48(UnkStandState_A6_b70* self, s32 kind) {
    fn_710099d6f0(self);
    if (kind == 8) {
        UnkStandMove_A6_b70* m = *reinterpret_cast<UnkStandMove_A6_b70**>(reinterpret_cast<char*>(self) + 0x38);
        if (UnkStandMoveState_A6_b70* state = FindState(m, 8))
            self->field_0x148 = state->field_0x148;
    }
    UnkStandMove_A6_b70* movement = self->movement;
    self->armed = true;
    self->timer = kind == 4 ? 0.0f : -1.0f;
    if (movement->pending) {
        movement->pending = false;
        reinterpret_cast<void (*)(UnkStandMove_A6_b70*)>(movement->vtable[0x260 / 8])(movement);
    }
}
