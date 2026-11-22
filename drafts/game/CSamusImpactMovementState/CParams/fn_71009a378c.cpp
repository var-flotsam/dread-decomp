// CSamusImpactMovementState (unit 1464): after the base update, unless the movement holds the state,
// asks the state machine for the air state (id 1) and switches to it at once. Names and layouts are
// placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkTU1730.h"
#include "types.h"

struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");

struct UnkDesc16_ovb0004 {
    u64 a, b;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkMoveTransition_ovb0004 {
    s64 id;
    UnkDesc16_ovb0004 desc;
    UnkTrackBlend blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_ovb0004(UnkDesc16_ovb0004 d, UnkTrackBlend b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
    UnkMoveTransition_ovb0004()
        : UnkMoveTransition_ovb0004(*reinterpret_cast<const UnkDesc16_ovb0004*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_ovb0004) == 0x30, "layout");

// A 64-bit word at a 4-byte aligned offset.
typedef u64 UnkU64Align4_ovb0004 __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8), given the vtable of the state it asks for.
struct UnkLeaveParams_l9968ec {
    void* vtable;
    UnkMoveTransition_ovb0004 transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_ovb0004 field_0x3c;
    u32 field_0x44;

    explicit UnkLeaveParams_l9968ec(void* state_vtable)
        : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = state_vtable;
    }
};
static_assert(sizeof(UnkLeaveParams_l9968ec) == 0x48, "layout");

struct UnkMoveState_l9a378c {
    virtual void vf0();
    virtual void vf8();
    virtual void vf10(u32 prev_id);
    virtual void vf18(u32 next_id);
    virtual void vf20();
    virtual void vf28();
    virtual void vf30(UnkLeaveParams_l9968ec* params);
    void* field_0x8;
    u32 id;  // 0x10
};

// The movement state machine: the active state and the one before it, the states by id, and the
// requested id (15 for none), applied at once unless changes are locked.
struct UnkMoveMachine_l9a378c {
    virtual void vf0();
    virtual void vf8();
    virtual void vf10();
    virtual void vf18();
    virtual void vf20(UnkMoveState_l9a378c* prev, UnkMoveState_l9a378c* next);
    virtual void vf28(UnkMoveState_l9a378c* prev, UnkMoveState_l9a378c* next);
    void* field_0x8;
    UnkMoveState_l9a378c* current;   // 0x10
    UnkMoveState_l9a378c* previous;  // 0x18
    u32 count;                       // 0x20
    u32 states[0x20];                // 0x24: UnkMoveState_l9a378c* states, 4-byte aligned
    u32 requested;                   // 0xa4
    bool locked;                     // 0xa8

    UnkMoveState_l9a378c** begin() { return reinterpret_cast<UnkMoveState_l9a378c**>(states); }
    UnkMoveState_l9a378c** end() { return begin() + count; }

    UnkMoveState_l9a378c* Find(u32 id) {
        UnkMoveState_l9a378c** it = begin();
        for (; it != end(); ++it) {
            if ((*it)->id == id)
                break;
        }
        return it != end() ? *it : nullptr;
    }
};
static_assert(__builtin_offsetof(UnkMoveMachine_l9a378c, locked) == 0xa8, "layout");

// Switches the machine to the state with this id (none for 0).
static void ChangeState(UnkMoveMachine_l9a378c* self, u32 id) {
    if (self->current && self->current->id == id)
        return;
    UnkMoveState_l9a378c* next = self->Find(id);
    if (!next && id != 0)
        return;
    if (self->current) {
        self->current->vf18(id);
        self->vf28(self->current, next);
    }
    UnkMoveState_l9a378c* prev = self->current;
    self->current = next;
    self->previous = prev;
    if (!next)
        return;
    next->vf10(self->previous ? self->previous->id : 15);
    self->vf20(self->previous, next);
}

// Hands the air state (id 1) its params and switches to it unless changes are locked.
static void RequestAir(UnkMoveMachine_l9a378c* self, UnkLeaveParams_l9968ec* params) {
    if (!fn_710009d50c(fn_7100988350(), fn_71009b0304()))
        return;
    if (UnkMoveState_l9a378c* state = self->Find(1))
        state->vf30(params);
    self->requested = 1;
    if (!self->locked) {
        ChangeState(self, 1);
        self->requested = 15;
    }
}

struct UnkImpactMovement_l9a378c {
    void** vtable;

    bool vf_0x3b0() { return reinterpret_cast<bool (*)(UnkImpactMovement_l9a378c*)>(vtable[0x3b0 / 8])(this); }
};

struct UnkImpactState_l9a378c {
    void* vtable;
    UnkMoveMachine_l9a378c* machine;  // 0x8
    u8 pad_0x10[0x28];
    UnkImpactMovement_l9a378c* movement;  // 0x38
};

void fn_71009b0d24(UnkImpactState_l9a378c* self);

void fn_71009a378c(UnkImpactState_l9a378c* self) {
    fn_71009b0d24(self);
    if (self->movement->vf_0x3b0())
        return;
    UnkLeaveParams_l9968ec params(g_unk_7101a22ce8 + 0x10);
    RequestAir(self->machine, &params);
}
