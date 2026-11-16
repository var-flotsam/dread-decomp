// CCharacterLifeComponent unit (2713): the angles an actor turns to when hit. Starts from the owner's
// angles; a forced side, the attacker's movement or position, or the owner's own facing then picks a
// yaw of plus or minus a quarter turn. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkRefl_fn_710032df00;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_710032df00>(void*);

// A position returned in memory (not a plain vector: the original builds it through x8).
struct UnkSluggerPos {
    f32 x;
    f32 y;
    f32 z;
    UnkSluggerPos(const UnkSluggerPos& other);
};

// The base character class's check, called with the same arguments (declared without them in its
// generated definition).
UnkSluggerPos fn_71011008a8(void* ai, u32 index);

// What answers an actor's +0x1f8: its +0x290 is a position along x.
struct UnkLifePlacement_l158fa0 {
    u8 pad_0x0[0x290];
    f32 x;
};

struct UnkLifeOwnMovement_l158fa0 {
    u8 pad_0x0[0x10c];
    f32 speed;
};

// The attacker's "MOVEMENT" component.
struct UnkLifeMovement_l158fa0 {
    u8 pad_0x0[0x11c];
    f32 speed;
};

struct UnkLifeActor_l158fa0 {
    u8 pad_0x0[0x38];
    UnkXform xform;
    u8 pad_0x94[0x1f0 - 0x94];
    UnkLifeOwnMovement_l158fa0* movement;  // 0x1f0
    UnkLifePlacement_l158fa0* placement;   // 0x1f8
};

struct UnkLifeAttacker_l158fa0 {
    u8 pad_0x0[0x44];
    f32 x;  // translation x
    u8 pad_0x48[0x1f8 - 0x48];
    UnkLifePlacement_l158fa0* placement;  // 0x1f8
    u8 pad_0x200[0x240 - 0x200];
    void* ai;  // 0x240
};

struct UnkLifeSource_l158fa0 {
    u8 pad_0x0[0x10];
    UnkLifeAttacker_l158fa0* actor;
};

// A box whose bounds are brought up to date (vf_0x98) while its flag at +0x60 is set.
struct UnkLifeBox_l158fa0 {
    virtual void vf_0x0();
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

    u8 pad_0x8[0x38];
    f32 min_x;  // 0x40
    f32 pad_0x44;
    f32 max_x;  // 0x48
    u8 pad_0x4c[0x14];
    bool dirty;  // 0x60
};

struct UnkLifeHit_l158fa0;
typedef void (*UnkLifeFacingCallback_l158fa0)(void* context, UnkLifeActor_l158fa0* owner, UnkLifeHit_l158fa0* hit,
                                              UnkVec3f* angles);

struct UnkLifeHit_l158fa0 {
    u8 pad_0x0[0x8];
    UnkLifeSource_l158fa0* source;  // 0x8
    u8 pad_0x10[0x15];
    bool use_box;  // 0x25
    u8 pad_0x26[0x12];
    void* context;                           // 0x38
    UnkLifeFacingCallback_l158fa0 callback;  // 0x40
    u8 pad_0x48;
    bool forced;        // 0x49
    f32 forced_side;    // 0x4c
    u8 pad_0x50[0x48];
    UnkLifeBox_l158fa0* box;  // 0x98

    UnkLifeAttacker_l158fa0* Attacker() const { return source ? source->actor : nullptr; }
};

struct UnkLifeComponent_l158fa0 {
    u8 pad_0x0[0x30];
    UnkLifeActor_l158fa0* owner;
};

static const f32 kQuarterTurn = 1.5707964f;

static inline f32 Sign_l158fa0(f32 v) {
    return (f32)((s32)(v > 0.0f) - (s32)(v < 0.0f));
}

// Turns to a quarter turn about y: clockwise when `away`, else counter-clockwise.
static inline void Face_l158fa0(UnkVec3f& angles, bool away) {
    if (away)
        angles = UnkVec3f(0.0f, -kQuarterTurn, 0.0f);
    else
        angles = UnkVec3f(0.0f, kQuarterTurn, 0.0f);
}

UnkVec3f fn_7101158fa0(UnkLifeComponent_l158fa0* self, UnkLifeHit_l158fa0* hit) {
    UnkVec3f result = self->owner->xform.GetAngle();
    if (hit->forced) {
        Face_l158fa0(result, hit->forced_side > 0.0f);
        return result;
    }

    if (!hit->Attacker()) {
        // No attacker: face along the owner's own x axis.
        const UnkMatrix44& m = self->owner->xform.GetMatrix();
        f32 side = m.m[2] + m.m[6] * 0.0f + m.m[10] * 0.0f;
        Face_l158fa0(result, !(side > 0.0f));
        return result;
    }

    UnkLifeMovement_l158fa0* attackerMovement = reinterpret_cast<UnkLifeMovement_l158fa0*>(
        base::reflection::refl_FindTyped<UnkRefl_fn_710032df00>(hit->source->actor));
    UnkLifeOwnMovement_l158fa0* ownMovement = self->owner->movement;
    f32 ownSpeed = ownMovement->speed;
    f32 attackerSpeed = attackerMovement ? attackerMovement->speed : 0.0f;
    UnkLifeAttacker_l158fa0* attacker = hit->Attacker();
    if (attackerMovement && attacker && ownMovement && attackerSpeed != 0.0f) {
        // A moving attacker that is faster or comes the other way: face away from its motion.
        if (ownSpeed == 0.0f || Sign_l158fa0(ownSpeed) != Sign_l158fa0(attackerSpeed) ||
            fabsf(attackerSpeed) > fabsf(ownSpeed)) {
            Face_l158fa0(result, attackerMovement->speed > 0.0f);
            return result;
        }
    }
    if (!attacker)
        return result;

    // Face the attacker's side.
    f32 attackerX;
    if (void* ai = attacker->ai)
        attackerX = fn_71011008a8(ai, 1).x;
    else
        attackerX = attacker->placement ? attacker->placement->x : attacker->x;
    if (hit->use_box) {
        if (UnkLifeBox_l158fa0* box = hit->box) {
            if (box->dirty)
                box->vf_0x98();
            attackerX = (box->min_x + box->max_x) * 0.5f;
        }
    }
    result = UnkVec3f(0.0f, -kQuarterTurn, 0.0f);
    if (self->owner->placement->x < attackerX)
        result.y = kQuarterTurn;
    if (hit->callback)
        hit->callback(hit->context, self->owner, hit, &result);
    return result;
}
