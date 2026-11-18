// CShelmitAIComponent's unit (tools/tu_map.py unit 1054): turns towards a target. The target's weak handle is
// kept at +0x14788; the angle between the owner's facing and the direction to the target (both positions from
// their models' root node when there is one) picks the left or right turn track, sent to the owner's animation,
// and is written in degrees to its property bag. Names and layouts are placeholders.
#include <math.h>

#include "../../game_s2/UnkSpitSend.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2523.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

// The two turn tracks.
__attribute__((visibility("hidden"))) extern const u64 g_unk_7101647930;
__attribute__((visibility("hidden"))) extern const u64 g_unk_71016478f8;
// The property the turn angle is written to.
__attribute__((visibility("hidden"))) extern const CStrId g_unk_71727071a0;

struct UnkShelmitTurnAI_l6bba7c {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x14788 - 0x38];
    UnkWeakHandle* target;  // 0x14788
    u32 field_0x14790;
};

static inline UnkXform* Xform(UnkActor* actor) {
    return reinterpret_cast<UnkXform*>(actor->field_0x38);
}

// Where an actor is: its model's root node when it has one (its transform read into `m`), otherwise its
// own translation.
static inline UnkVec3f Position(UnkActor* actor, UnkMatrix44& m) {
    UnkVec3f pos = Xform(actor)->GetTranslation();
    UnkCopyMatrix(&m, g_unk_7101a1a680);
    if (UnkEntity* entity = static_cast<UnkEntity*>(actor->field_0x228)) {
        u64 key = unk_prop_bag::Key(0xd58).data->Key();
        if (fn_71011735e8(entity, &key, &m, 0))
            pos = UnkVec3f(m.m[3], m.m[7], m.m[11]);
    }
    return pos;
}

void fn_71006bba7c(UnkShelmitTurnAI_l6bba7c* self, UnkWeakTarget* target) {
    self->field_0x14790 = 0;
    AssignRef(&self->target, fn_7100d78378(target));

    UnkActor* owner = self->owner;
    UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(owner->field_0x220);
    UnkActor* other = fn_7100d7c990(owner);
    if (!animation || !other)
        return;

    UnkMatrix44 m;
    UnkVec3f to = Position(other, m);
    UnkVec3f from = Position(self->owner, m);
    f32 dx = to.x - from.x;
    f32 dy = to.y - from.y;
    f32 dz = to.z - from.z;
    f32 distance2 = dx * dx;
    distance2 += dy * dy;
    distance2 += dz * dz;
    f32 inv = 1.0f / (sqrtf(distance2) + 0.0001f);
    f32 heading = atan2f(dy * inv, dx * inv);
    f32 facing = atan2f(Xform(self->owner)->GetMatrix().m[5], Xform(self->owner)->GetMatrix().m[1]);
    f32 angle = heading - facing;
    if (Xform(self->owner)->GetAngle().y < 0.0f)
        angle = -angle;

    u64 track = angle > 0.0f ? g_unk_7101647930 : g_unk_71016478f8;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 1;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(animation, track, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
    unk_prop_bag::fn_710033187c(reinterpret_cast<unk_prop_bag::UnkPropBag*>(animation->field_0x80),
                                g_unk_71727071a0, angle * 180.0f / 3.141592653589793);
}
