// Unit 1452 (tools/tu_map.py): two steps of the event holder (src/game/UnkUnit1452/UnkEventHolder_Dtor_A4_b74.cpp).
// fn_71009803b8 clears the flag (+0xe0) and counter (+0xe4), then requests state 3 of its machine (+0x30) when the
// object fn_710097f198 gives has its flag (+0x3a8) set, and applies whatever is pending (the state-machine
// templates of src/game/UnkStateMachineSet_b60.cpp). fn_710098202c is described below. Names and layouts are
// placeholders.
// Unit 1452 (tools/tu_map.py): a state's parameter step. The object fn_710097f1e0 gives is told (vf_0x1c8) the
// owner (fn_710097f1b8) and the flag at +0xe0 when it is a CPullableGrapplePointComponent. With the flag set the
// default CSamusStandMovementState::CParams request (as fn_7100990e14,
// src/game/CSamusCrouchMovementState/CParams/CParams_Request_ovb0131.cpp) is stored into the reflected value
// the caller passed, else fn_710097f71c does the work; fn_710097fe58 ends both. Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkTU1450.h"
#include "game/UnkTU1730.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;

struct UnkStandTrackTime_ovb0280 {
    u64 key;
    u64 time;
};
struct UnkStandTrackBlend_ovb0280 {
    u32 a, b, c;
};

struct UnkStandTrackParams_ovb0280 {
    s64 id;
    UnkStandTrackTime_ovb0280 time;
    UnkStandTrackBlend_ovb0280 blend;
    u32 flags;
    u32 mask;

    __attribute__((always_inline)) UnkStandTrackParams_ovb0280(UnkStandTrackTime_ovb0280 time_,
                                                               UnkStandTrackBlend_ovb0280 blend_, u32 mask_)
        : id(-1), time(time_), blend(blend_), flags(0), mask(mask_) {}
    __attribute__((always_inline)) UnkStandTrackParams_ovb0280()
        : UnkStandTrackParams_ovb0280(reinterpret_cast<const UnkStandTrackTime_ovb0280&>(g_unk_710164b958),
                                      reinterpret_cast<const UnkStandTrackBlend_ovb0280&>(g_unk_7172769740),
                                      base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>()) {}
};

struct UnkStandParamsVtable_ovb0280 {
    void* vtable;
    __attribute__((always_inline)) UnkStandParamsVtable_ovb0280() { vtable = g_unk_7101a22cb8 + 2; }
};

struct UnkStandRequest_ovb0280 : UnkStandParamsVtable_ovb0280 {
    UnkStandTrackParams_ovb0280 params;  // 0x8
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;

    __attribute__((always_inline)) UnkStandRequest_ovb0280() {
        field_0x38 = 3;
        __builtin_memset(&field_0x3c, 0, 8);
        field_0x44 = 0;
        vtable = g_unk_7101a22d18 + 2;
    }
};

struct UnkUnit1452_b56;
struct UnkUnit1452Owner_b56;
struct UnkPositionSource_b0249;
void fn_710097f71c(UnkUnit1452_b56* self, void* arg, UnkReflValue* value);
void fn_710097fe58(UnkUnit1452_b56* self, void* arg);

struct UnkGrapplePointObj_ovb0280 {
    virtual const CType* vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8(UnkUnit1452Owner_b56* owner, bool flag);
};

struct UnkStandStep_ovb0280 {
    u8 pad_0x0[0xe0];
    bool field_0xe0;
};

void fn_710098202c(UnkStandStep_ovb0280* self, void* arg, UnkReflValue* value) {
    UnkUnit1452_b56* base = reinterpret_cast<UnkUnit1452_b56*>(self);
    if (UnkGrapplePointObj_ovb0280* obj = reinterpret_cast<UnkGrapplePointObj_ovb0280*>(fn_710097f1e0(base))) {
        UnkGrapplePointObj_ovb0280* grapple =
            fn_710009d50c(obj->vf_0x0(), fn_710082c6ac()) ? obj : nullptr;
        grapple->vf_0x1c8(fn_710097f1b8(base), self->field_0xe0);
    }
    if (self->field_0xe0) {
        UnkStandRequest_ovb0280 req;
        const CType* t = fn_71009b3854();
        const CType* cur = reinterpret_cast<const CType*>(value->type);
        if (cur && value->field_0x14 == 0 &&
            (cur == t ? *reinterpret_cast<void* const*>(reinterpret_cast<const char*>(t) + 0x48) != nullptr
                      : fn_710009d52c(t, cur))) {
            fn_71000a1cc8(value, t, &req);
        } else {
            fn_71000a1c60(value);
            fn_71000a18d4(value, t, &req, false);
        }
    } else {
        fn_710097f71c(base, arg, value);
    }
    fn_710097fe58(base, arg);
}

struct UnkSmState_b60 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_b60* __attribute__((aligned(4))) UnkSmStatePtr_b60;

struct UnkSmMachine_b60 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_b60* previous, UnkSmState_b60* next);
    virtual void vf_0x28(UnkSmState_b60* current, UnkSmState_b60* next);

    u64 field_0x8;
    UnkSmState_b60* current;
    UnkSmState_b60* previous;
    u32 count;
    UnkSmStatePtr_b60 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_b60 {
    u8 pad_0x0[Offset];
    UnkSmMachine_b60 machine;
};

__attribute__((always_inline)) static inline bool FindState_b60(UnkSmMachine_b60* self, u32 id,
                                                               UnkSmState_b60*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_b60);
    asm("" : "+r"(n));
    for (UnkSmStatePtr_b60* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtr_b60)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

// Switches to state `id` (the b59 "apply pending" body) and clears the pending id.
template <u32 None>
__attribute__((always_inline)) static inline void SwitchState_b60(UnkSmMachine_b60* self, u32 id,
                                                                 UnkSmState_b60* current) {
    if (!current || current->id != id) {
        UnkSmState_b60* found;
        UnkSmState_b60* next;
        u32 enter;
        if (FindState_b60(self, id, found)) {
            next = found;
            enter = true;
        } else {
            if (id != 0)
                goto done;
            enter = false;
            next = nullptr;
        }
        if (current) {
            current->vf_0x18(id);
            self->vf_0x28(self->current, next);
        }
        self->previous = self->current;
        self->current = found;
        if (enter) {
            typedef void (*EnteredFn)(UnkSmState_b60*, s32);
            s32 previous_id = self->previous ? self->previous->id : None;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = None;
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void RequestState_b60(UnkSmMachine_b60* self, u32 id) {
    // Stored through a plain u32 pointer: the original rereads a state id the caller tested after this.
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_b60<None>(self, id, self->current);
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void ApplyPending_b60(UnkSmMachine_b60* self) {
    u32 id = self->pending;
    if (id > Last)
        return;
    SwitchState_b60<None>(self, id, self->current);
}

struct UnkSmBase_b60;
struct UnkSmOther_b60 {
    u8 pad_0x0[0x3a8];
    bool field_0x3a8;
};

struct UnkSm1452_b0280 {
    u8 pad_0x0[0x30];
    UnkSmMachine_b60 machine;
    bool field_0xe0;
    u8 pad_0xe1[3];
    u32 field_0xe4;
};

void fn_71009803b8(UnkSm1452_b0280* self) {
    self->field_0xe0 = false;
    self->field_0xe4 = 0;
    if (fn_710097f198(reinterpret_cast<UnkSmBase_b60*>(self))->field_0x3a8)
        RequestState_b60<3, 4>(&self->machine, 3);
    else
        fn_710097f1e0(reinterpret_cast<UnkUnit1452_b56*>(self));
    ApplyPending_b60<3, 4>(&self->machine);
}
