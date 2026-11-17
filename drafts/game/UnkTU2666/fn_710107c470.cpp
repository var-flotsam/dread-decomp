// Unit 2666 Lua binding: puts the player's movement (+0x208) into its interact state (id 12) with
// default interact params (a CSamusInteractMovementState::CParams: an unset track request, then 3 and
// zeros). The state, when present, takes the params (slot 0x30); then the movement's state machine
// (+0x2cf8) switches to it, as the door state machine does (12 pending meanwhile, 15 for none after).
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkGameManager.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct lua_State;
struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;
extern void* g_unk_7101a874c0[];  // CSamusInteractMovementState::CParams's vtable

struct UnkTrackTime_l07c470 {
    u64 key;
    u64 time;
};
struct UnkTrackBlend_l07c470 {
    u32 a, b, c;
};

// A track request with no id, the default time and blend descriptors and the all-flags mask.
struct UnkTrackParams_l07c470 {
    s64 id;
    UnkTrackTime_l07c470 time;
    UnkTrackBlend_l07c470 blend;
    u32 flags;
    u32 mask;

    UnkTrackParams_l07c470(UnkTrackTime_l07c470 time_, UnkTrackBlend_l07c470 blend_, u32 mask_)
        : id(-1), time(time_), blend(blend_), flags(0), mask(mask_) {}
    UnkTrackParams_l07c470()
        : UnkTrackParams_l07c470(reinterpret_cast<const UnkTrackTime_l07c470&>(g_unk_710164b958),
                                 reinterpret_cast<const UnkTrackBlend_l07c470&>(g_unk_7172769740),
                                 base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>()) {}
};

// The reflected object under the params: its vtable is set before the members are built.
struct UnkParamsObject_l07c470 {
    void* vtable;

    UnkParamsObject_l07c470() { vtable = g_unk_7101a22cb8 + 2; }
};

// CSamusMovementState::CParams.
struct UnkMovementParams_l07c470 : UnkParamsObject_l07c470 {
    UnkTrackParams_l07c470 track;  // 0x8
};

// CSamusInteractMovementState::CParams.
struct UnkInteractPair_l07c470 {
    u32 a, b;
};

struct UnkInteractParams_l07c470 : UnkMovementParams_l07c470 {
    u32 field_0x38;
    UnkInteractPair_l07c470 field_0x3c;
    u32 field_0x44;

    UnkInteractParams_l07c470() : field_0x38(3), field_0x3c(), field_0x44(0) { vtable = g_unk_7101a874c0 + 2; }
};

struct UnkMoveState_l07c470 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30(UnkMovementParams_l07c470* params);  // takes the params
    u64 field_0x8;
    s32 id;
};

// The states are only 4-aligned after the count.
typedef UnkMoveState_l07c470* __attribute__((aligned(4))) UnkMoveStatePtr_l07c470;

struct UnkMoveStateMachine_l07c470 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkMoveState_l07c470* previous, UnkMoveState_l07c470* next);  // after entering
    virtual void vf_0x28(UnkMoveState_l07c470* current, UnkMoveState_l07c470* next);   // after leaving

    u64 field_0x8;
    UnkMoveState_l07c470* current;
    UnkMoveState_l07c470* previous;
    u32 count;
    UnkMoveStatePtr_l07c470 states[16];
    s32 pending;
    bool locked;

    UnkMoveState_l07c470* Find(s32 id) {
        return FindStateById(states, count, id);
    }

    // Inlined in the original; the barrier in Find makes it too large for the inliner otherwise.
    __attribute__((always_inline)) void Request(s32 id) {
        pending = id;
        if (locked)
            return;
        UnkMoveState_l07c470* from = current;
        if (!from || from->id != id) {
            if (UnkMoveState_l07c470* next = Find(id)) {
                if (from) {
                    from->vf_0x18(id);
                    vf_0x28(current, next);
                }
                UnkMoveState_l07c470* prev = current;
                previous = prev;
                current = next;
                next->vf_0x10(prev ? prev->id : 15);
                vf_0x20(previous, next);
            }
        }
        pending = 15;
    }
};

struct UnkMovement_l07c470 {
    u8 pad_0x0[0x2cf8];
    UnkMoveStateMachine_l07c470 states;  // 0x2cf8
};
static_assert(__builtin_offsetof(UnkMoveStateMachine_l07c470, pending) == 0xa4, "layout");

struct UnkPlayer_l07c470 {
    u8 pad_0x0[0x208];
    UnkMovement_l07c470* movement;
};

s32 fn_710107c470(lua_State* L) {
    auto* player = reinterpret_cast<UnkPlayer_l07c470*>(fn_7100dec788(g_unk_7101d47310));
    UnkMovement_l07c470* movement = player->movement;
    if (!movement)
        return 0;
    UnkInteractParams_l07c470 params;
    if (!fn_710009d50c(fn_71009a3d24(), fn_71009b0304()))
        return 0;
    if (UnkMoveState_l07c470* state = movement->states.Find(12))
        state->vf_0x30(&params);
    movement->states.Request(12);
    return 0;
}
