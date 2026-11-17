// CSamusInteractMovementState (unit 1465): when the movement's value at +0x7c is away from zero,
// asks the state machine for the next movement state, with a transition whose id depends on the
// sign of the value at +0x58. Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkTU1730.h"
#include "types.h"

using base::reflection::CClass;

struct __attribute__((packed, aligned(4))) UnkTrackBlend_l9a400c {
    u64 kind;
    f32 value;
};

struct UnkDesc16_l9a400c {
    u64 a, b;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkMoveTransition_l9a400c {
    s64 id;
    UnkDesc16_l9a400c desc;
    UnkTrackBlend_l9a400c blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_l9a400c(s64 i, UnkDesc16_l9a400c d, UnkTrackBlend_l9a400c b, u32 m)
        : id(i), desc(d), blend(b), field_0x24(0), mask(m) {}
    explicit UnkMoveTransition_l9a400c(s64 i = -1)
        : UnkMoveTransition_l9a400c(i, *reinterpret_cast<const UnkDesc16_l9a400c*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend_l9a400c*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_l9a400c) == 0x30, "layout");

typedef u64 UnkU64Align4_l9a400c __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8), given the vtable of the state it asks for.
struct UnkParams_l9a400c {
    void* vtable;
    UnkMoveTransition_l9a400c transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_l9a400c field_0x3c;
    u32 field_0x44;

    explicit UnkParams_l9a400c(void* state_vtable)
        : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = state_vtable;
    }
};
static_assert(sizeof(UnkParams_l9a400c) == 0x48, "layout");

// The params seen through their vtable.
struct UnkEvent_l9a400c {
    virtual const CClass* vf0();
    virtual void vf8();
    virtual void vf10();
    virtual u32 vf18();
};

struct UnkState_l9a400c {
    virtual void vf0();
    virtual void vf8();
    virtual void vf10(u32 prevId);
    virtual void vf18(u32 nextId);
    virtual void vf20();
    virtual void vf28();
    virtual void vf30(UnkEvent_l9a400c* ev);
    void* field_0x8;
    u32 field_0x10;
};

struct UnkStateList_l9a400c {
    u32 count;
    u8 storage[16 * sizeof(void*)];
    UnkState_l9a400c** begin() { return reinterpret_cast<UnkState_l9a400c**>(storage); }
    UnkState_l9a400c** end() { return begin() + count; }
};

// The movement state machine (fn_710098ae04's).
struct UnkStateMachine_l9a400c {
    virtual void vf0();
    virtual void vf8();
    virtual void vf10();
    virtual void vf18();
    virtual void vf20(UnkState_l9a400c* prev, UnkState_l9a400c* next);
    virtual void vf28(UnkState_l9a400c* prev, UnkState_l9a400c* next);
    void* field_0x8;
    UnkState_l9a400c* field_0x10;
    UnkState_l9a400c* field_0x18;
    UnkStateList_l9a400c field_0x20;
    u32 field_0xa4;
    bool field_0xa8;

    // Looks up the registered state with `id`; false when there is none.
    bool Find(u32 id, UnkState_l9a400c*& out) {
        UnkState_l9a400c** it = field_0x20.begin();
        for (; it != field_0x20.end(); ++it) {
            if ((*it)->field_0x10 == id)
                break;
        }
        if (it == field_0x20.end())
            return false;
        out = *it;
        return out != nullptr;
    }

    void ApplyPending() {
        if (field_0xa4 < 15) {
            ChangeState(field_0xa4);
            field_0xa4 = 15;
        }
    }

    void ChangeState(u32 id) {
        if (field_0x10 && field_0x10->field_0x10 == id)
            return;
        UnkState_l9a400c* next = nullptr;
        bool found = Find(id, next);
        if (!found && id != 0)
            return;
        if (field_0x10)
            field_0x10->vf18(id);
        vf28(field_0x10, next);
        UnkState_l9a400c* prev = field_0x10;
        field_0x10 = next;
        field_0x18 = prev;
        if (!found)
            return;
        next->vf10(prev ? prev->field_0x10 : 15);
        vf20(field_0x18, next);
    }

    void Request(UnkEvent_l9a400c* ev) {
        if (!fn_710009d50c(ev->vf0(), fn_71009b0304()))
            return;
        u32 id = ev->vf18();
        if (id <= 14) {
            UnkState_l9a400c* s;
            if (Find(id, s))
                s->vf30(ev);
        }
        field_0xa4 = id;
        if (!field_0xa8)
            ApplyPending();
    }
};

struct UnkMovement_l9a400c {
    u8 pad_0x0[0x58];
    f32 field_0x58;
    u8 pad_0x5c[0x7c - 0x5c];
    f32 field_0x7c;
};

struct UnkInteractState_l9a400c {
    void* vtable;
    UnkStateMachine_l9a400c* machine;  // 0x8
    u8 pad_0x10[0x28];
    UnkMovement_l9a400c* movement;  // 0x38
};

void fn_71009a400c(UnkInteractState_l9a400c* self) {
    f32 value = self->movement->field_0x7c;
    // Nothing to do while the value is within 0.001 of zero.
    if (value + 0.001f >= 0.0f && value - 0.001f <= 0.0f)
        return;
    UnkParams_l9a400c params(reinterpret_cast<char*>(g_unk_7101a22d18) + 0x10);
    if (self->movement->field_0x58 > 0.0f)
        params.transition = UnkMoveTransition_l9a400c(0xb983e7b418df08c0);
    else
        params.transition = UnkMoveTransition_l9a400c(0xac84166680d35ac4);
    self->machine->Request(reinterpret_cast<UnkEvent_l9a400c*>(&params));
}
