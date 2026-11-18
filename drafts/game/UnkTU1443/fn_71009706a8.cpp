// Unit 1443 of tools/tu_map.py: after the base fn_71009690a0, a message of kind 0 or 3 copies its optional
// values into the object (and its flag when the message's kind is the object's), then the owner's two vectors
// at +0x40 and +0x70 are set from the stored ones (or from the owner's own, vf_0x370), the second only when it
// is not zero, and the first, scaled, is pushed to the owner when the flag is set and clear. Names and layouts
// are placeholders.
#include "base/math/CVector.h"
#include "game/CMovementComponent.h"
#include "game/UnkTU1440.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;

template <class T>
struct UnkOptional_l9706a8 {
    T value;
    bool has;

    void Assign(const UnkOptional_l9706a8& other) {
        if (other.has) {
            value = other.value;
            has = true;
        }
    }
};

struct UnkMover_l9706a8 {
    void** vtable;
    u8 pad_0x8[0x38];
    CVector3D field_0x40;
    u8 pad_0x4c[0x24];
    CVector3D field_0x70;

    // vf_0x370: the owner's own vector.
    UnkVec3f GetVector() { return reinterpret_cast<UnkVec3f (*)(UnkMover_l9706a8*)>(vtable[0x370 / 8])(this); }
};

struct UnkMsg_l9706a8 {
    u8 pad_0x0[0x8];
    UnkOptional_l9706a8<f32> scale;           // 0x8
    UnkOptional_l9706a8<u32> field_0x10;      // 0x10
    UnkOptional_l9706a8<CVector3D> field_0x18;  // 0x18
    UnkOptional_l9706a8<CVector3D> field_0x28;  // 0x28
    u32 kind;                                 // 0x38
    UnkOptional_l9706a8<bool> flag;           // 0x3c
};

struct UnkApply_l9706a8 {
    u8 pad_0x0[0x38];
    UnkMover_l9706a8* owner;  // 0x38
    u8 pad_0x40[0x58];
    UnkOptional_l9706a8<f32> scale;             // 0x98
    UnkOptional_l9706a8<u32> field_0xa0;        // 0xa0
    UnkOptional_l9706a8<CVector3D> field_0xa8;  // 0xa8
    UnkOptional_l9706a8<CVector3D> field_0xb8;  // 0xb8
    u32 kind;                                   // 0xc8
    UnkOptional_l9706a8<bool> flag;             // 0xcc
};
static_assert(__builtin_offsetof(UnkApply_l9706a8, kind) == 0xc8, "layout");

// The stored vector if there is one, else the owner's own.
static inline CVector3D ValueOr(const UnkOptional_l9706a8<CVector3D>* v, const UnkApply_l9706a8* self) {
    if (v->has)
        return v->value;
    UnkVec3f own = self->owner->GetVector();
    return {own.x, own.y, own.z};
}

static inline bool IsNearZero(f32 v) {
    return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f;
}

void fn_71009706a8(UnkApply_l9706a8* self, const UnkMsg_l9706a8* msg, bool reset) {
    fn_71009690a0(reinterpret_cast<UnkUnit1441Apply_A4_b74*>(self),
                  reinterpret_cast<const UnkUnit1441Msg_A4_b74*>(msg), reset);
    if (msg->kind != 0 && msg->kind != 3)
        return;
    if (reset) {
        self->scale.has = false;
        self->field_0xa0.has = false;
        self->field_0xa8.has = false;
        self->field_0xb8.has = false;
    }
    self->scale.Assign(msg->scale);
    self->field_0xa0.Assign(msg->field_0x10);
    self->field_0xa8.Assign(msg->field_0x18);
    self->field_0xb8.Assign(msg->field_0x28);
    if (reset)
        self->flag.has = false;
    if (msg->kind == self->kind)
        self->flag.Assign(msg->flag);

    f32 scale = 0.0f;
    bool push = false;
    if (self->flag.has && !self->flag.value && self->scale.has) {
        scale = self->scale.value;
        push = true;
    }
    CVector3D first = ValueOr(&self->field_0xb8, self);
    self->owner->field_0x40 = first;
    CVector3D second = ValueOr(&self->field_0xa8, self);
    UnkMover_l9706a8* owner = self->owner;
    if (!(IsNearZero(second.x) && IsNearZero(second.y) && IsNearZero(second.z)))
        owner->field_0x70 = second;
    if (push) {
        CVector3D v;
        v.x = scale * first.x;
        v.y = scale * first.y;
        v.z = scale * first.z;
        fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(self->owner),
                      reinterpret_cast<const UnkMovementVec_71010d08d4*>(&v), true);
    }
}
