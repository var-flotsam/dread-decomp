// CAreaFXComponent unit (tools/tu_map.py unit 1263). fn_71007fe794: captures the owner's state: the
// playing track's time and action id when its movement is in state 4, else the facing built from the
// movement's direction, the owner's angles and position, then sets three components' vf_0x98 to zero.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkTU1134.h"
#include "game/UnkTU54.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

// Builds a matrix from its three axes and its translation.
void fn_71000769f0(UnkMatrix44* m, const UnkVec3f& x, const UnkVec3f& y, const UnkVec3f& z, const UnkVec3f& t);

struct UnkComponent_e794 {
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
    virtual void vf_0x98(f32 value);
};

struct UnkMovement_e794 : UnkComponent_e794 {
    u8 pad_0x8[0x68];
    UnkVec3f direction;  // 0x70
    u8 pad_0x7c[0x1922];
    bool field_0x199e;
};
static_assert(__builtin_offsetof(UnkMovement_e794, field_0x199e) == 0x199e, "UnkMovement_e794 layout");

struct UnkOwner_e794 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x184];
    UnkMovement_e794* movement;    // 0x218
    UnkComponent_e794* animation;  // 0x220
    u8 pad_0x228[0x70];
    UnkComponent_e794* field_0x298;
};
static_assert(__builtin_offsetof(UnkOwner_e794, field_0x298) == 0x298, "UnkOwner_e794 layout");

struct UnkAreaFX_e794 {
    u8 pad_0x0[0x30];
    UnkOwner_e794* owner;  // 0x30
    u8 pad_0x38[0x3c8];
    bool field_0x400;
    u8 pad_0x401[0x7];
    CStrId action;   // 0x408
    f32 time;        // 0x410
    bool has_angle;  // 0x414
    u8 pad_0x415[3];
    UnkVec3f angle;  // 0x418
    bool has_pos;    // 0x424
    u8 pad_0x425[3];
    UnkVec3f pos;    // 0x428
};
static_assert(__builtin_offsetof(UnkAreaFX_e794, pos) == 0x428, "UnkAreaFX_e794 layout");

static inline UnkVec3f Up(UnkOwner_e794* owner) {
    const UnkMatrix44& m = owner->xform.GetMatrix();
    return UnkVec3f(m.m[1], m.m[5], m.m[9]);
}

void fn_71007fe794(UnkAreaFX_e794* self) {
    UnkOwner_e794* owner = self->owner;
    UnkMovement_e794* movement = owner ? owner->movement : nullptr;
    UnkComponent_e794* animation = owner ? owner->animation : nullptr;
    UnkTrack* track = animation ? fn_71011771e0(animation, 0xf4061ff25a2163d6) : nullptr;
    UnkComponent_e794* other = self->owner ? self->owner->field_0x298 : nullptr;

    self->field_0x400 = false;
    self->action = g_unk_7101d470e0;
    self->time = -1.0f;
    self->has_angle = false;
    self->angle = self->owner->xform.GetAngle();
    self->has_pos = false;
    self->pos = self->owner->xform.GetTranslation();

    if (movement) {
        if (fn_71010cc3a4(movement) == 4 && track && movement->field_0x199e) {
            self->field_0x400 = true;
            self->time = fn_710070d89c(track);
            self->action = *reinterpret_cast<CStrId*>(fn_710070d7f8(reinterpret_cast<UnkObj_710117af10*>(track)));
        } else if (fn_71010cab0c(movement)) {
            self->has_angle = true;
            UnkVec3f direction = movement->direction;
            UnkMatrix44 frame;
            fn_71000769f0(&frame, -Cross(direction, Up(self->owner)), Up(self->owner), direction,
                          self->owner->xform.GetTranslation());
            self->angle = fn_7100076acc(reinterpret_cast<const UnkXform*>(&frame));
        }
        switch (fn_71010cc3a4(movement)) {
        case 1:
        case 4:
            self->has_pos = true;
            self->pos = self->owner->xform.GetTranslation();
            break;
        case 2:
        case 3:
            self->has_pos = false;
            break;
        }
        movement->vf_0x98(0.0f);
        movement->vf_0x98(0.0f);
    }
    if (animation)
        animation->vf_0x98(0.0f);
    if (other)
        other->vf_0x98(0.0f);
}
