// Unit 1449 (tools/tu_map.py): starts the component's grapple state: stores the target point, hands the
// grapple tunable's parameters to the grapple state, faces the movement towards the target, then requests
// state 3 or 2 on the state machine at +0x30 (last state 4, "none" 5) and applies it. Names and layouts are
// placeholders.
#include "base/math/CVector.h"
#include "game/CSamusGrappleMovementState.h"
#include "game/UnkGrappleParams_b65a6.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkTU1450.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkGrappleState_b65a6;
void fn_710099c920(UnkGrappleState_b65a6* self, const UnkGrappleParams_b65a6* params);

// The grapple movement tunable (g_unk_717273ba88).
struct UnkGrappleTunable_l97c268 {
    u8 pad_0x0[0x24];
    f32 field_0x24;
    u8 pad_0x28[0x34 - 0x28];
    f32 field_0x34;
    u8 pad_0x38[0x40 - 0x38];
    base::math::CVector2D field_0x40;
    u8 pad_0x48[0x4c - 0x48];
    f32 field_0x4c;
};

static inline const UnkGrappleTunable_l97c268& GrappleTunable() {
    return reinterpret_cast<const UnkGrappleTunable_l97c268&>(g_unk_717273ba88);
}

// What fn_710099c920 copies into the grapple state.
struct UnkGrappleSetup_l97c268 {
    base::math::CVector2D field_0x0;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
};

struct UnkSmState_l97c268 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_l97c268* __attribute__((aligned(4))) UnkSmStatePtr_l97c268;

struct UnkSmMachine_l97c268 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_l97c268* previous, UnkSmState_l97c268* next);
    virtual void vf_0x28(UnkSmState_l97c268* current, UnkSmState_l97c268* next);

    u64 field_0x8;
    UnkSmState_l97c268* current;
    UnkSmState_l97c268* previous;
    u32 count;
    UnkSmStatePtr_l97c268 states[16];
    u32 pending;
    bool busy;
};

static inline bool FindState_l97c268(UnkSmMachine_l97c268* self, u32 id, UnkSmState_l97c268*& out) {
    out = FindStateById(self->states, self->count, id);
    return out != nullptr;
}

template <u32 None>
static inline void SwitchState_l97c268(UnkSmMachine_l97c268* self, u32 id, UnkSmState_l97c268* current) {
    if (!current || current->id != id) {
        UnkSmState_l97c268* found;
        UnkSmState_l97c268* next;
        u32 enter;
        if (FindState_l97c268(self, id, found)) {
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
            typedef void (*EnteredFn)(UnkSmState_l97c268*, s32);
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
static inline void RequestState_l97c268(UnkSmMachine_l97c268* self, u32 id) {
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_l97c268<None>(self, id, self->current);
}

template <u32 Last, u32 None>
static inline void ApplyPending_l97c268(UnkSmMachine_l97c268* self) {
    u32 id = self->pending;
    if (id > Last)
        return;
    SwitchState_l97c268<None>(self, id, self->current);
}

struct UnkGrappleHolder_l97c268 {
    u64 field_0x0;
    UnkGrappleState_b65a6* state;
};

struct UnkTarget_l97c268 {
    virtual void vf_0x0();
    // vf_0x188: the point to face.
};

struct UnkOwner_l97c268 {
    u8 pad_0x0[0x38];
    UnkXform xform;
};

struct UnkMovement_l97c268;
typedef void (*UnkFaceFn_l97c268)(UnkMovement_l97c268* self, const base::math::CVector3D* dir);
typedef UnkVec3f (*UnkPointFn_l97c268)(UnkPositionSource_b0249* self);

struct UnkGrappleStart_l97c268 {
    u64 field_0x0;
    UnkGrappleHolder_l97c268* field_0x8;
    u8 pad_0x10[0x30 - 0x10];
    UnkSmMachine_l97c268 machine;
    u8 pad_0xe0[0x230 - 0xe0];
    UnkVec3f field_0x230;
    UnkVec3f field_0x23c;
    u8 pad_0x248[0x278 - 0x248];
    bool field_0x278;
};
static_assert(__builtin_offsetof(UnkGrappleStart_l97c268, machine) + __builtin_offsetof(UnkSmMachine_l97c268, pending) == 0xd4, "pending");

UnkVec3f fn_710097c53c(UnkPositionSource_b0249* source, UnkUnit1452Owner_b56* owner);
bool fn_710097c738(UnkGrappleStart_l97c268* self, UnkVec3f* point);

static inline void Request_l97c268(UnkGrappleStart_l97c268* self, u32 id) {
    RequestState_l97c268<4, 5>(&self->machine, id);
}

void fn_710097c268(UnkGrappleStart_l97c268* self) {
    self->field_0x278 = false;
    UnkMovement_l97c268* movement =
        reinterpret_cast<UnkMovement_l97c268*>(fn_710097f198(reinterpret_cast<UnkSmBase_b60*>(self)));
    UnkGrappleState_b65a6* grapple = self->field_0x8 ? self->field_0x8->state : nullptr;
    UnkUnit1452Owner_b56* owner = fn_710097f1b8(reinterpret_cast<UnkUnit1452_b56*>(self));
    UnkPositionSource_b0249* source = fn_710097f1e0(reinterpret_cast<UnkUnit1452_b56*>(self));
    self->field_0x230 = fn_710097c53c(source, owner);

    {
        const UnkGrappleTunable_l97c268& tunable = GrappleTunable();
        UnkGrappleSetup_l97c268 setup;
        setup.field_0xc = tunable.field_0x4c;
        setup.field_0x8 = tunable.field_0x24;
        setup.field_0x10 = tunable.field_0x34;
        setup.field_0x0 = tunable.field_0x40;
        fn_710099c920(grapple, reinterpret_cast<const UnkGrappleParams_b65a6*>(&setup));
    }

    f32 side;
    {
        base::math::CVector2D flat;
        {
            base::math::CVector3D delta;
            UnkVec3f target =
                reinterpret_cast<UnkPointFn_l97c268*>(*reinterpret_cast<void***>(source))[0x188 / 8](source);
            const UnkXform& xform = reinterpret_cast<UnkOwner_l97c268*>(owner)->xform;
            f32 tx = xform.m[0][3], ty = xform.m[1][3], tz = xform.m[2][3];
            delta.x = target.x - tx;
            delta.y = target.y - ty;
            delta.z = target.z - tz;
            fn_710007b430(&flat, &delta);
        }
        side = (f32)((flat.x > 0.0f) - (flat.x < 0.0f));
    }
    base::math::CVector3D dir = {side, 0.0f, 0.0f};
    reinterpret_cast<UnkFaceFn_l97c268*>(*reinterpret_cast<void***>(movement))[0x208 / 8](movement, &dir);

    if (fn_710097c738(self, &self->field_0x23c))
        Request_l97c268(self, 3);
    else
        Request_l97c268(self, 2);
    ApplyPending_l97c268<4, 5>(&self->machine);
}
