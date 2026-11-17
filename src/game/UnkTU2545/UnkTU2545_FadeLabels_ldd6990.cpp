// Unit 2545 (tools/tu_map.py): polls for a skip and, unless the step is to be passed over (field_0xbc),
// fades the panel's labels in over half a second, then (or when skipped) moves to state 8.
// Names and layouts are placeholders.
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkTU2545.h"
#include "game/UnkTU2747.h"
#include "types.h"

struct UnkPanel_b0277;
using UnkGuiObj = UnkGuiDisplayObject;

struct UnkFlowState_ldd6990 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

// The states are only 4-aligned after the count.
typedef UnkFlowState_ldd6990* __attribute__((aligned(4))) UnkFlowStatePtr_ldd6990;

struct UnkFlowMachine_ldd6990 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkFlowState_ldd6990* previous, UnkFlowState_ldd6990* next);  // after entering
    virtual void vf_0x28(UnkFlowState_ldd6990* current, UnkFlowState_ldd6990* next);   // after leaving

    u64 field_0x8;
    UnkFlowState_ldd6990* current;
    UnkFlowState_ldd6990* previous;
    u32 count;
    UnkFlowStatePtr_ldd6990 states[16];
    s32 pending;
    bool busy;

    static const s32 kNoState = 9;

    UnkFlowState_ldd6990* FindState(s32 id) { return FindStateById(states, count, id); }

    void RequestState(s32 id) {
        pending = id;
        if (busy)
            return;
        UnkFlowState_ldd6990* cur = current;
        if (!cur || cur->id != id) {
            if (UnkFlowState_ldd6990* next = FindState(id)) {
                if (cur) {
                    cur->vf_0x18(id);
                    vf_0x28(current, next);
                }
                previous = current;
                current = next;
                next->vf_0x10(previous ? previous->id : kNoState);
                vf_0x20(previous, next);
            }
        }
        pending = kNoState;
    }
};

struct UnkFlow_ldd6990 : UnkFlowMachine_ldd6990 {
    u32 field_0xac;
    f32 time;
    u8 pad_0xb4[0xbc - 0xb4];
    bool field_0xbc;
    u8 pad_0xbd[0xe0 - 0xbd];
    bool field_0xe0;
    u8 pad_0xe1[0x138 - 0xe1];
    UnkGuiObj* field_0x138;
    UnkGuiObj* field_0x140;
    UnkGuiObj* field_0x148;
    UnkGuiObj* field_0x150;
    u8 pad_0x158[0x269 - 0x158];
    bool skip;
};
static_assert(__builtin_offsetof(UnkFlow_ldd6990, time) == 0xb0, "layout");
static_assert(__builtin_offsetof(UnkFlow_ldd6990, skip) == 0x269, "layout");
static_assert(__builtin_offsetof(UnkFlowMachine_ldd6990, pending) == 0xa4, "layout");

static inline f32 Clamp01(f32 v) {
    return v <= 0.0f ? 0.0f : (!(v >= 1.0f) ? v : 1.0f);
}

void fn_7100dd6990(UnkFlow_ldd6990* self, f32 dt) {
    fn_7100dd60fc(reinterpret_cast<UnkPanel_b0277*>(self));
    if (self->field_0xbc) {
        self->RequestState(8);
        return;
    }
    self->time += dt;
    f32 alpha = Clamp01(self->time / 0.5f);
    fn_71011cfba8(self->field_0x148, alpha);
    if (self->field_0x150)
        fn_71011cfba8(self->field_0x150, alpha);
    if (self->field_0xe0) {
        fn_71011cfba8(self->field_0x140, alpha);
        fn_71011cfba8(self->field_0x138, alpha);
    }
    if (self->time >= 0.5f || self->skip)
        self->RequestState(8);
}
