// Unit 2545 (tools/tu_map.py): adds the elapsed time to the flow's timer, polls for a skip, fades the rows
// in one after another (0.8 s each) and, once all have faded in (or it was skipped), moves to state 8.
// Names and layouts are placeholders.
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkStateMachineFind.h"
#include "types.h"

struct UnkPanel_b0277;
void fn_7100dd60fc(UnkPanel_b0277* self);
using UnkGuiObj = UnkGuiDisplayObject;
void fn_71011cfba8(UnkGuiObj* object, f32 alpha);

struct UnkFlowRow_ldd7598 {
    UnkGuiDisplayObject* label;
    UnkGuiDisplayObject* value;
    u8 pad_0x10[0x18];
};

struct UnkFlowState_ldd7598 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

// The states are only 4-aligned after the count.
typedef UnkFlowState_ldd7598* __attribute__((aligned(4))) UnkFlowStatePtr_ldd7598;

struct UnkFlowMachine_ldd7598 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkFlowState_ldd7598* previous, UnkFlowState_ldd7598* next);  // after entering
    virtual void vf_0x28(UnkFlowState_ldd7598* current, UnkFlowState_ldd7598* next);   // after leaving

    u64 field_0x8;
    UnkFlowState_ldd7598* current;
    UnkFlowState_ldd7598* previous;
    u32 count;
    UnkFlowStatePtr_ldd7598 states[16];
    s32 pending;
    bool busy;

    static const s32 kNoState = 9;

    UnkFlowState_ldd7598* FindState(s32 id) { return FindStateById(states, count, id); }

    void RequestState(s32 id) {
        pending = id;
        if (busy)
            return;
        UnkFlowState_ldd7598* cur = current;
        if (!cur || cur->id != id) {
            if (UnkFlowState_ldd7598* next = FindState(id)) {
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

struct UnkFlow_ldd7598 : UnkFlowMachine_ldd7598 {
    u32 field_0xac;
    f32 time;
    u8 pad_0xb4[0x198 - 0xb4];
    UnkFlowRow_ldd7598 rows[5];
    u32 row_count;
    u8 pad_0x264[0x269 - 0x264];
    bool skip;
};
static_assert(__builtin_offsetof(UnkFlow_ldd7598, time) == 0xb0, "layout");
static_assert(__builtin_offsetof(UnkFlow_ldd7598, row_count) == 0x260, "layout");
static_assert(__builtin_offsetof(UnkFlow_ldd7598, skip) == 0x269, "layout");
static_assert(__builtin_offsetof(UnkFlowMachine_ldd7598, pending) == 0xa4, "layout");

static inline f32 Clamp01(f32 v) {
    return v <= 0.0f ? 0.0f : (!(v >= 1.0f) ? v : 1.0f);
}

static const f32 kRowFadeTime = 0.8f;

void fn_7100dd7598(UnkFlow_ldd7598* self, f32 dt) {
    fn_7100dd60fc(reinterpret_cast<UnkPanel_b0277*>(self));
    self->time += dt;
    f32 total = self->row_count * kRowFadeTime;
    for (u32 i = 0; i < self->row_count; ++i) {
        f32 alpha = Clamp01(Clamp01((self->time - i * kRowFadeTime) / kRowFadeTime) * 2.0f);
        fn_71011cfba8(self->rows[i].value, alpha);
        fn_71011cfba8(self->rows[i].label, alpha);
    }
    if (self->time >= total || self->skip)
        self->RequestState(8);
}
