// CQuetzoaChargeAttack's unit (tools/tu_map.py): the charge's update. After the base update, while the
// owner plays the charge animation it steers toward the target; while it plays the turn animation it
// decides whether to stop charging (fn_7100644780). Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/behaviortree/CSwitchBetweenAIModesAction.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"

struct UnkChargeVec_l644148 {
    f32 x, y, z;
};

// The steering block handed to the owner's navigation (+0x298).
struct UnkChargeQuery_l644148 {
    void** vtable;
    UnkChargeVec_l644148 dir;
    u8 field_0x14;
    u8 pad_0x15[7];
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;

    UnkChargeQuery_l644148(const UnkChargeVec_l644148& d)
        : vtable(g_unk_7101a24b58 + 2), dir(d), field_0x1c(0), field_0x20(0), field_0x28(0), field_0x30(0),
          field_0x34(0) {
        field_0x14 = 1;
    }
};

struct UnkChargeAnim_l644148 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkQuetzoaChargeAttack_l644148;

struct UnkChargeVtable_l644148 {
    void* pad_0x0[22];
    UnkChargeAnim_l644148** (*vf_0xb0)(UnkQuetzoaChargeAttack_l644148* self);
};

struct UnkQuetzoaChargeAttack_l644148 {
    const UnkChargeVtable_l644148* vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x20];
    UnkChargeVec_l644148 origin;  // 0x30
};

bool fn_71006444ac(UnkQuetzoaChargeAttack_l644148* self, const UnkChargeVec_l644148* from,
                   const UnkChargeVec_l644148* dir, UnkChargeVec_l644148* out);
void fn_7100644780(UnkQuetzoaChargeAttack_l644148* self);

static inline UnkChargeVec_l644148 Position(const UnkActor* actor) {
    UnkChargeVec_l644148 pos = {actor->field_0x44, actor->field_0x54, actor->field_0x64};
    return pos;
}

static inline UnkChargeVec_l644148 Forward(const UnkActor* actor) {
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
    UnkChargeVec_l644148 dir = {m.m[2], m.m[6], m.m[10]};
    return dir;
}

static inline f32 Sign(f32 x) {
    return (f32)((x > 0.0f) - (x < 0.0f));
}

static inline f32 DistanceSq(const UnkChargeVec_l644148& a, const UnkChargeVec_l644148& b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    f32 dz = a.z - b.z;
    return dx * dx + dy * dy + dz * dz;
}

void fn_7100644148(UnkQuetzoaChargeAttack_l644148* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    UnkActor* owner = self->owner;
    UnkAnimationComponent* anim = static_cast<UnkAnimationComponent*>(owner->field_0x220);
    void* nav = owner->field_0x298;
    base::reflection::UnkReflObject* component =
        static_cast<base::reflection::UnkReflObject*>(owner->field_0x240);
    bool isQuetzoa = component && fn_710009d50c(component->vf_0x0(), fn_7100649630());
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!anim || !nav || !isQuetzoa || !target)
        return;

    UnkChargeVec_l644148 pos = Position(self->owner);
    f32 targetX = target->field_0x44;
    if (fn_710117ae38(anim, (*self->vtable->vf_0xb0(self))->id, false)) {
        UnkChargeVec_l644148 dir = Forward(self->owner);
        target = fn_7100d7c990(self->owner);
        if (!target)
            return;
        f32 side = Sign(dir.x);
        f32 dx = target->field_0x44 - pos.x;
        f32 dy = target->field_0x54 - pos.y;
        f32 angle = atan2f(dy, dx * side);
        if (!(angle >= -1.2217305f && angle <= 0.34906584f))
            return;
        UnkChargeVec_l644148 aimed = {cosf(angle) * side, sinf(angle), 0.0f};
        dir = aimed;
        UnkChargeVec_l644148 out;
        out.x = 0.0f;
        out.y = 0.0f;
        out.z = 0.0f;
        if (fn_71006444ac(self, &pos, &dir, &out)) {
            UnkChargeQuery_l644148 query(dir);
            fn_71006f4b0c(nav, &query, 0);
        }
        return;
    }

    if (!fn_710117ae38(anim, 0xb30ae46e4f7cf0f6, false))
        return;
    f32 travelled = DistanceSq(pos, self->origin);
    f32 offset = targetX - pos.x;
    bool turnedAway = fabsf(offset) > 200.0f && travelled > 90000.0f &&
                      Sign(offset) != Sign(Forward(self->owner).x);
    if (!turnedAway) {
        UnkChargeVec_l644148 out;
        out.x = 0.0f;
        out.y = 0.0f;
        out.z = 0.0f;
        UnkChargeVec_l644148 dir = Forward(self->owner);
        if (fn_71006444ac(self, &pos, &dir, &out))
            return;
        if (!(travelled > 90000.0f))
            return;
        if (!(DistanceSq(out, pos) < 22500.0f))
            return;
    }
    fn_7100644780(self);
}
