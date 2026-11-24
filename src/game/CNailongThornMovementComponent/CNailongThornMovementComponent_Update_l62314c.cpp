// CNailongThornMovementComponent unit (tools/tu_map.py): the update. After the base update, when the
// thorn's shooter (+0x10 of the link at +0x230) has a CNailongAIComponent (+0x240) and the thorn has
// gone farther from it than the AI's range (+0x4d8 of its tunables at +0x1610), the owner's timeline
// gets the id hashed from "onimpact" set to the all-flags mask. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "base/math/CVectorOps.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkMovementComponent.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkNailongTunables_l62314c {
    u8 pad_0x0[0x4d8];
    f32 range;  // 0x4d8
};

struct UnkNailongAI_l62314c : base::reflection::UnkReflObject {
    u8 pad_0x8[0x1608];
    UnkNailongTunables_l62314c* tunables;  // 0x1610
};

struct UnkThornLink_l62314c {
    u8 pad_0x0[0x10];
    UnkActor* shooter;
};

struct UnkThorn_l62314c {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x230 - 0x38];
    UnkThornLink_l62314c* link;  // 0x230
};

// The actor's position (the translation of its transform).
inline base::math::CVector3D Pos_l62314c(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

// `dt` (s0) reaches the base update untouched in the original (tools/forwarded_args.py).
void CNailongThornMovementComponent_ImpactWhenOutOfRange(UnkThorn_l62314c* self, f32 dt) {
    fn_71010d1a90(reinterpret_cast<UnkMovementComponent*>(self), dt);
    if (!self->link)
        return;
    UnkActor* shooter = self->link->shooter;
    if (!shooter)
        return;
    auto* ai = static_cast<UnkNailongAI_l62314c*>(shooter->field_0x240);
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_710061f464()))
        return;
    UnkActor* owner = self->owner;
    shooter = self->link->shooter;
    base::math::CVector3D from = Pos_l62314c(owner);
    base::math::CVector3D to = Pos_l62314c(shooter);
    UnkNailongTunables_l62314c* tunables = ai->tunables;
    if (!(base::math::DistanceSq(from, to) > tunables->range * tunables->range))
        return;
    if (auto* timeline = static_cast<UnkTimelineComponent*>(owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        base::global::CName name("onimpact");
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &name.hash, &mask.id, false, -1.0f);
    }
}
