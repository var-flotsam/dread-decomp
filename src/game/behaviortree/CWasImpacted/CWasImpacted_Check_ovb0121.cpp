// behaviortree::CWasImpacted's unit (tools/tu_map.py): holds when the AI's last impact time (+0x112c, -1 for
// none) is below the limit at +0x38; with the flag at +0x3c set, the impact must also not be blocked (vf_0x170 of
// the object at +0x218) and the actor must face away from its target. Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkImpactAI_ovb0121 {
    u8 pad_0x0[0x112c];
    f32 last_impact;  // 0x112c, -1 when none
};

struct UnkImpactShield_ovb0121;
struct UnkImpactShieldVtbl_ovb0121 {
    u8 pad_0x0[0x170];
    bool (*vf_0x170)(UnkImpactShield_ovb0121*);  // whether the impact was blocked
};
struct UnkImpactShield_ovb0121 {
    UnkImpactShieldVtbl_ovb0121* vtbl;
};

struct UnkImpactActor_ovb0121 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x218 - 0x94];
    UnkImpactShield_ovb0121* field_0x218;
    u8 pad_0x220[0x240 - 0x220];
    UnkImpactAI_ovb0121* ai;  // 0x240
};
static_assert(__builtin_offsetof(UnkImpactActor_ovb0121, ai) == 0x240, "layout");

struct UnkWasImpacted_ovb0121 {
    u8 pad_0x0[0x18];
    UnkImpactActor_ovb0121* actor;  // 0x18
    u8 pad_0x20[0x38 - 0x20];
    f32 max_time;      // 0x38
    bool from_front;   // 0x3c
};

static inline bool Blocked(UnkImpactShield_ovb0121* obj) {
    return obj->vtbl->vf_0x170(obj);
}

static inline bool Facing(UnkWasImpacted_ovb0121* self) {
    auto* target = reinterpret_cast<UnkImpactActor_ovb0121*>(
        fn_7100d7c990(reinterpret_cast<UnkActor*>(self->actor)));
    UnkVec3f angle = self->actor->xform.GetAngle();
    UnkVec3f pos = self->actor->xform.GetTranslation();
    UnkVec3f tpos = target->xform.GetTranslation();
    if (angle.y > 0.0f && pos.x > tpos.x)
        return true;
    if (angle.y < 0.0f && pos.x < tpos.x)
        return true;
    return false;
}

bool fn_71003a3480(UnkWasImpacted_ovb0121* self) {
    UnkImpactActor_ovb0121* actor = self->actor;
    if (actor && actor->ai) {
        f32 last = actor->ai->last_impact;
        if (last != -1.0f) {
            bool result = last < self->max_time;
            if (self->from_front && last < self->max_time) {
                UnkImpactShield_ovb0121* shield = actor->field_0x218;
                result = shield && !Blocked(shield) && Facing(self);
            }
            return result;
        }
    }
    return false;
}
