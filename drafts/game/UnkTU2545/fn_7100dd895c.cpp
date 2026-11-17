// Unit 2545 (tools/tu_map.py): adds the elapsed time to the flow's timer, polls for a skip, and once the
// wait for the previous state has passed (or it was skipped) moves the state machine on to the next state.
// Names and layouts are placeholders.
#include "game/UnkStateMachineFind.h"
#include "types.h"

struct UnkPanel_b0277;
void fn_7100dd60fc(UnkPanel_b0277* self);

struct UnkFlowState_ldd895c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

// The states are only 4-aligned after the count.
typedef UnkFlowState_ldd895c* __attribute__((aligned(4))) UnkFlowStatePtr_ldd895c;

struct UnkFlowMachine_ldd895c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkFlowState_ldd895c* previous, UnkFlowState_ldd895c* next);  // after entering
    virtual void vf_0x28(UnkFlowState_ldd895c* current, UnkFlowState_ldd895c* next);   // after leaving

    u64 field_0x8;
    UnkFlowState_ldd895c* current;
    UnkFlowState_ldd895c* previous;
    u32 count;
    UnkFlowStatePtr_ldd895c states[16];
    s32 pending;
    bool busy;

    static const s32 kNoState = 9;

    UnkFlowState_ldd895c* FindState(s32 id) { return FindStateById(states, count, id); }

    void RequestState(s32 id) {
        pending = id;
        if (busy)
            return;
        UnkFlowState_ldd895c* cur = current;
        if (!cur || cur->id != id) {
            if (UnkFlowState_ldd895c* next = FindState(id)) {
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

struct UnkFlow_ldd895c : UnkFlowMachine_ldd895c {
    u32 field_0xac;
    f32 time;
    u32 field_0xb4;
    f32 field_0xb8;
    u8 pad_0xbc[0x269 - 0xbc];
    bool skip;
};
static_assert(__builtin_offsetof(UnkFlow_ldd895c, time) == 0xb0, "layout");
static_assert(__builtin_offsetof(UnkFlow_ldd895c, skip) == 0x269, "layout");
static_assert(__builtin_offsetof(UnkFlowMachine_ldd895c, pending) == 0xa4, "layout");

// The state that follows each one and how long to stay in it first; a near-zero speed (field_0xb8)
// skips the first two waits.
struct UnkStep_ldd895c {
    s32 next;
    f32 wait;
};

static inline UnkStep_ldd895c NextStep(s32 id, bool quick) {
    if (quick) {
        switch (id) {
        case 2: return {3, 0.0f};
        case 3: return {4, 0.0f};
        case 4: return {5, 0.5f};
        case 5: return {6, 0.5f};
        case 6: return {1, 0.5f};
        }
    } else {
        switch (id) {
        case 2: return {3, 1.0f};
        case 3: return {4, 0.5f};
        case 4: return {5, 0.5f};
        case 5: return {6, 0.5f};
        case 6: return {1, 0.5f};
        }
    }
    return {8, 0.0f};
}

void fn_7100dd895c(UnkFlow_ldd895c* self, f32 dt) {
    self->time += dt;
    bool quick = self->field_0xb8 < 0.001f;
    UnkStep_ldd895c step = NextStep(self->previous ? self->previous->id : UnkFlow_ldd895c::kNoState, quick);
    fn_7100dd60fc(reinterpret_cast<UnkPanel_b0277*>(self));
    if (self->time >= step.wait || self->skip)
        self->RequestState(step.next);
}
