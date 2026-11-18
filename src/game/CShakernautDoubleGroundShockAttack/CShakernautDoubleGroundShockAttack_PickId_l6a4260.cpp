// CShakernautDoubleGroundShockAttack's unit: picks the id of the next animation. While the owner's
// usual track kind plays the id g_unk_7172705848, one of two ids by whether the AI component's
// facing points along +x; otherwise the id of the CCharClassShakernautInitAttackTrack record now
// playing, or the empty id. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CShakernautDoubleGroundShockAttack.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

extern __attribute__((visibility("hidden"))) CStrId g_unk_7172705848;

struct UnkShakernautAI_l6a4260 {
    u8 pad_0x0[0x14810];
    f32 facing_x;  // 0x14810
    f32 facing_y;  // 0x14814
};

struct UnkShakernautAttack_l6a4260 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
};

const CStrId* fn_71006a4260(UnkShakernautAttack_l6a4260* self) {
    CStrId id(*fn_7101177ae8(static_cast<UnkAnimationComponent*>(self->owner->field_0x220),
                             0xf4061ff25a2163d6));
    const CStrId* result;
    if (id == g_unk_7172705848) {
        UnkReflObject* ai = static_cast<UnkReflObject*>(self->owner->field_0x240);
        UnkShakernautAI_l6a4260* shakernaut = fn_710009d50c(ai->vf_0x0(), fn_71006ad0a4())
                                                  ? reinterpret_cast<UnkShakernautAI_l6a4260*>(ai)
                                                  : nullptr;
        f32 along = shakernaut->facing_x * 1.0f + shakernaut->facing_y * 0.0f;
        result = along > 0.0f ? &g_unk_71727057c0[1] : &g_unk_71727057c0[0];
    } else {
        UnkTrackQuery query = {fn_7100c4dd48(), nullptr, -1.0f};
        fn_710117b08c(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), &query);
        if (!query.result)
            return &g_unk_7101d470e0;
        result = &query.result->sources;
    }
    return result;
}
