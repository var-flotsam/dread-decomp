// CChozoWarriorXAIComponent unit: after the base step, when `state` is the component's first state block,
// the block is active and it is the second-last entry of the component's state list, runs fn_7101105e14
// with 0. Names and layouts are placeholders.
#include "types.h"

// The state list: a ring through a sentinel node that is the list itself (tail at prev, head at next).
struct UnkChozoStateNode_l48f658 {
    u8 pad_0x0[0x8];
    void* value;                      // 0x8
    u32 count;                        // 0x10, the sentinel's entry count
    UnkChozoStateNode_l48f658* prev;  // 0x18
    UnkChozoStateNode_l48f658* next;  // 0x20

    // The entry at `index`, walked from whichever end is nearer.
    UnkChozoStateNode_l48f658* At(u32 index) {
        UnkChozoStateNode_l48f658* node = this;
        if (index < count - index) {
            for (u32 steps = index + 1; steps != 0; --steps)
                node = node->next;
        } else {
            for (u32 steps = count - index; steps != 0; --steps)
                node = node->prev;
        }
        return node;
    }
};

struct UnkChozoWarriorXAI_l48f658 {
    u8 pad_0x0[0x1160];
    UnkChozoStateNode_l48f658 states;  // 0x1160
    u8 pad_0x1188[0x149c8 - 0x1188];
    bool state_active;  // 0x149c8
    u8 pad_0x149c9[0x14b78 - 0x149c9];
    u8 first_state[0x70];  // 0x14b78
};
static_assert(__builtin_offsetof(UnkChozoWarriorXAI_l48f658, first_state) == 0x14b78, "layout");

struct UnkQuetzoaDodgeAI_A3_b76;

void fn_710049c29c(UnkChozoWarriorXAI_l48f658* self, void* state);
void fn_7101105e14(UnkQuetzoaDodgeAI_A3_b76* self, f32 value);

void fn_710048f658(UnkChozoWarriorXAI_l48f658* self, void* state) {
    fn_710049c29c(self, state);
    if (state != self->first_state || !self->state_active)
        return;
    UnkChozoStateNode_l48f658& states = self->states;
    if (states.count < 2)
        return;
    if (states.At(states.count - 2)->value == state)
        fn_7101105e14(reinterpret_cast<UnkQuetzoaDodgeAI_A3_b76*>(self), 0.0f);
}
