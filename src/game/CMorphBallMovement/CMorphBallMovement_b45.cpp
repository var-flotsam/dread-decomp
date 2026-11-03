// CMorphBallMovement's unit (tools/tu_map.py): with the flag at +0xf9 set, clears it and calls the
// virtual at slot 0x260; then resets a set of state fields. Names and layout are placeholders.
#include "types.h"

struct UnkVec2_b45a2 {
    f32 x;
    f32 y;
};

// The block at +0x23c4, addressed from its start.
struct UnkMorphBallState_b45a2 {
    UnkVec2_b45a2 field_0x0;
    u8 pad_0x8[0xa];
    u16 field_0x12;
    bool field_0x14;
    u8 pad_0x15[0x22f];
    bool field_0x244;
};

struct UnkMorphBallMovement_b45a2 {
    void** vtable;
    u8 pad_0x8[0xf1];
    bool field_0xf9;
    u8 pad_0xfa[0xe2];
    UnkVec2_b45a2 field_0x1dc;
    u8 pad_0x1e4[0x21e0];
    UnkMorphBallState_b45a2 state;  // 0x23c4
    u8 pad_0x260c[0x50];
    u32 field_0x265c;

    void vf_0x260() { reinterpret_cast<void (*)(UnkMorphBallMovement_b45a2*)>(vtable[0x260 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkMorphBallMovement_b45a2, field_0x265c) == 0x265c, "layout");

void fn_710094a43c(UnkMorphBallMovement_b45a2* self) {
    UnkMorphBallState_b45a2* state = &self->state;
    asm("" : "+r"(state));  // the original addresses the block from its start
    if (self->field_0xf9) {
        self->field_0xf9 = false;
        self->vf_0x260();
    }
    state->field_0x14 = false;
    state->field_0x12 = 1;
    self->field_0x1dc = UnkVec2_b45a2{0.0f, 0.0f};
    state->field_0x244 = false;
    self->field_0x265c = 0;
    state->field_0x0 = UnkVec2_b45a2{0.0f, 0.0f};
}
