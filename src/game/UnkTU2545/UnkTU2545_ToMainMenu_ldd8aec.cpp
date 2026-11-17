// Unit 2545 (tools/tu_map.py): adds the elapsed time to the flow's timer and, unless the game manager's
// effect player vetoes it, moves the flow's state machine to state 1 and switches the game to "MAINMENU".
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "game/UnkGameManager.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2656.h"
#include "types.h"

struct UnkFlowState_ldd8aec {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

// The states are only 4-aligned after the count.
typedef UnkFlowState_ldd8aec* __attribute__((aligned(4))) UnkFlowStatePtr_ldd8aec;

struct UnkFlowMachine_ldd8aec {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkFlowState_ldd8aec* previous, UnkFlowState_ldd8aec* next);  // after entering
    virtual void vf_0x28(UnkFlowState_ldd8aec* current, UnkFlowState_ldd8aec* next);   // after leaving

    u64 field_0x8;
    UnkFlowState_ldd8aec* current;
    UnkFlowState_ldd8aec* previous;
    u32 count;
    UnkFlowStatePtr_ldd8aec states[16];
    s32 pending;
    bool busy;

    static const s32 kNoState = 9;

    UnkFlowState_ldd8aec* FindState(s32 id) { return FindStateById(states, count, id); }

    void RequestState(s32 id) {
        pending = id;
        if (busy)
            return;
        UnkFlowState_ldd8aec* cur = current;
        if (!cur || cur->id != id) {
            if (UnkFlowState_ldd8aec* next = FindState(id)) {
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

struct UnkFlow_ldd8aec : UnkFlowMachine_ldd8aec {
    u32 field_0xac;
    f32 time;
};
static_assert(__builtin_offsetof(UnkFlow_ldd8aec, time) == 0xb0, "layout");
static_assert(__builtin_offsetof(UnkFlowMachine_ldd8aec, pending) == 0xa4, "layout");

void fn_7100dd8aec(UnkFlow_ldd8aec* self, f32 dt) {
    self->time += dt;
    if (fn_7101053a98(g_unk_7101d47310->field_0x1040))
        return;
    self->RequestState(1);
    fn_7100deac9c(g_unk_7101d47310, base::global::CRntString("MAINMENU"));
}
