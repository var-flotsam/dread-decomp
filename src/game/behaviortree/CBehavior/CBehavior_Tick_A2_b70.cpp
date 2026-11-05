// behaviortree::CBehavior's unit (tools/tu_map.py, the name from reflection): runs a behaviour for one
// status: 3 resumes (slot 0x88), 1 starts (slot 0x78) and then updates (slot 0x80), 2 updates; a finishing
// status (5-7) is reported through slot 0x90. Returns the status. Names and layouts are placeholders.
// (CBehaviorTree_ClearPending_A2_b69.cpp declared it returning void; it returns the status.)
#include "types.h"

struct UnkBehaviorNode_A2_b69 {
    void** vtable;
    void vf_0x78() { reinterpret_cast<void (*)(UnkBehaviorNode_A2_b69*)>(vtable[0x78 / 8])(this); }
    s32 vf_0x80(f32 dt) { return reinterpret_cast<s32 (*)(UnkBehaviorNode_A2_b69*, f32)>(vtable[0x80 / 8])(this, dt); }
    s32 vf_0x88(f32 dt) { return reinterpret_cast<s32 (*)(UnkBehaviorNode_A2_b69*, f32)>(vtable[0x88 / 8])(this, dt); }
    void vf_0x90(s32 s) { reinterpret_cast<void (*)(UnkBehaviorNode_A2_b69*, s32)>(vtable[0x90 / 8])(this, s); }
};

s32 fn_7100394b30(UnkBehaviorNode_A2_b69* node, f32 value, s32 state) {
    if (state == 1)
        node->vf_0x78();
    if (state == 3)
        state = node->vf_0x88(value);
    else if (static_cast<u32>(state - 1) < 2)
        state = node->vf_0x80(value);
    if (state >= 5 && state <= 7)
        node->vf_0x90(state);
    return state;
}
