// CCharClassChozoWarriorAIComponent's unit (tools/tu_map.py unit 732): the reaction to a hit
// (fn_710049770c). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CMeleeComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

using base::global::CStrId;

struct UnkChozoWarriorActorView_ovb0132 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;  // +0x2a0
    u8 pad_0x2a8[0x50];
    void* field_0x2f8;
};

static inline f32 Sign(f32 v) {
    return (f32)((v > 0.0f) - (v < 0.0f));
}

static inline f32 FacingSign(const UnkMatrix44* xform) {
    return Sign(fn_710002614c(xform).m[2]);
}

static inline UnkChozoWarriorActorView_ovb0132* View(UnkActor* actor) {
    return reinterpret_cast<UnkChozoWarriorActorView_ovb0132*>(actor);
}

// The reaction of `self` to a hit from `other`: 2 when, with crazy tracking allowed, `other` faces
// away from `self` from `self`'s facing side; 7 while melee damage is reverted; 4 otherwise.
u32 fn_710049770c(void* component, UnkActor* self, UnkActor* other) {
    if (UnkAnimationComponent* anim = static_cast<UnkAnimationComponent*>(self->field_0x220)) {
        if (fn_710117b348(anim, fn_7100ce9338())) {
            const UnkMatrix44* own_xform = reinterpret_cast<const UnkMatrix44*>(self->field_0x38);
            const UnkMatrix44* other_xform = reinterpret_cast<const UnkMatrix44*>(other->field_0x38);
            f32 side = Sign(other->field_0x44 - self->field_0x44);
            f32 own = FacingSign(own_xform);
            f32 theirs = FacingSign(other_xform);
            return theirs != own && side == own ? 2 : 4;
        }
        anim = static_cast<UnkAnimationComponent*>(self->field_0x220);
        if (anim && fn_710117b348(anim, fn_7100c47fe8()))
            return 7;
    }
    void* shield_owner = View(other)->field_0x2f8;
    if (shield_owner && fn_710092b6f8(shield_owner)) {
        UnkCollisionContainer* collision = View(self)->collision;
        CStrId name("shield", true);
        if (fn_71008640a4(collision, &name))
            return 4;
    }
    return 4;
}
