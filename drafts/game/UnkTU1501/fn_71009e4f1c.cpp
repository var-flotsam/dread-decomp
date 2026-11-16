// Unit 1501 (CSamusMovement): fn_71009e4f1c looks at two or more contact points: when all of them lie on
// the side the movement faces, some above and some below the actor and all within 42 units of it in x,
// it starts a 0.1 s timer (+0x5450) with that direction (+0x5454). Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkActor_l9e4f1c {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38

    f32 X() const { return transform.m[3]; }
    f32 Y() const { return transform.m[7]; }
    f32 Z() const { return transform.m[11]; }
    CVector3D Pos() const { return {transform.m[3], transform.m[7], transform.m[11]}; }
};

struct UnkMovement_l9e4f1c {
    u8 pad_0x0[0x30];
    UnkActor_l9e4f1c* actor;  // 0x30
    u8 pad_0x38[0x370];
    bool field_0x3a8;  // 0x3a8
    u8 pad_0x3a9[0x50a7];
    f32 field_0x5450;  // 0x5450, a timer
    f32 field_0x5454;  // 0x5454, the direction
};
static_assert(__builtin_offsetof(UnkMovement_l9e4f1c, field_0x5450) == 0x5450, "UnkMovement_l9e4f1c layout");

struct UnkContact_l9e4f1c {
    u8 pad_0x0[0x14];
    CVector3D pos;  // 0x14
    u8 pad_0x20[0x28];
};
static_assert(sizeof(UnkContact_l9e4f1c) == 0x48, "UnkContact_l9e4f1c layout");

struct UnkContactList_l9e4f1c {
    UnkContact_l9e4f1c* data;  // 0x0
    u64 count;  // 0x8

    UnkContact_l9e4f1c* begin() const { return data; }
    UnkContact_l9e4f1c* end() const { return data + count; }
};

struct UnkVelocity_l9e4f1c {
    u8 pad_0x0[0x28];
    f32 x;  // 0x28
};

struct UnkInput_l9e4f1c {
    u8 pad_0x0[0x20];
    f32 x;  // 0x20
};

static inline CVector3D Sub(const CVector3D& a, const CVector3D& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline f32 Sign(f32 v) {
    return (f32)((v > 0.0f) - (v < 0.0f));
}

void fn_71009e4f1c(UnkMovement_l9e4f1c* self, const UnkContactList_l9e4f1c* contacts,
                   const UnkVelocity_l9e4f1c* velocity, const UnkInput_l9e4f1c* input) {
    f32 dir = Sign(velocity->x);
    if (self->field_0x3a8)
        return;
    if (!(input->x > 0.0f || input->x < 0.0f))
        return;
    if (contacts->count < 2)
        return;

    bool sameSide = true;
    for (const UnkContact_l9e4f1c& contact : *contacts) {
        CVector2D flat;
        CVector3D offset = Sub(contact.pos, self->actor->Pos());
        fn_710007b430(&flat, &offset);
        sameSide &= dir == Sign(flat.x);
    }
    if (!sameSide)
        return;

    bool above = false;
    bool below = false;
    for (const UnkContact_l9e4f1c& contact : *contacts) {
        if (!above && contact.pos.y > self->actor->Y())
            above = true;
        else if (!below && contact.pos.y < self->actor->Y())
            below = true;
    }
    if (!(above && below))
        return;

    for (const UnkContact_l9e4f1c& contact : *contacts) {
        if (fabsf(contact.pos.x - self->actor->X()) >= 42.0f)
            return;
    }
    self->field_0x5450 = 0.1f;
    self->field_0x5454 = dir;
}
