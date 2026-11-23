// behaviortree::CEmmyChaseAction's unit (tools/tu_map.py): when the owner's animation plays a
// CCharClassAllowChaseTransitionTrack, plays the animation request that track fills in and, when it
// starts, clears the id property g_unk_71726debd8 of the blackboard section g_unk_71726b7cb8 (unless the
// blackboard is locked). Names and layouts are placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CAnimationComponent.h"
#include "game/CBlackboard.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CStrId;

extern UnkPropertyName g_unk_71726debd8;

struct UnkChaseDesc_l5134a8 {
    u64 words[2];
};

struct UnkChaseBlend_l5134a8 {
    u64 kind;
    u32 value;
};

struct UnkChaseRequest_l5134a8 {
    s64 id;
    u64 desc[2];     // 0x8
    u64 blend_kind;  // 0x18
    u32 blend_value; // 0x20
    u32 flags;       // 0x24
    u32 mask;        // 0x28
};

// The track that fills the request in.
struct UnkChaseTrack_l5134a8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkChaseRequest_l5134a8* request);
};

struct UnkChaseAI_l5134a8 {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;  // 0x1260
};

struct UnkChaseOwner_l5134a8 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x240 - 0x228];
    UnkChaseAI_l5134a8* ai;  // 0x240
};

struct UnkChaseAction_l5134a8 {
    u8 pad_0x0[0x18];
    UnkChaseOwner_l5134a8* owner;  // 0x18
};

void fn_71005134a8(UnkChaseAction_l5134a8* self) {
    UnkTypedValue v;
    UnkChaseOwner_l5134a8* owner = self->owner;
    if (!owner)
        return;
    UnkAnimationComponent* animation = owner->animation;
    UnkChaseAI_l5134a8* ai = owner->ai;
    if (!animation || !ai)
        return;

    UnkTrackQuery query = {fn_7100bf6348(), nullptr, -1.0f};
    bool found = fn_710117b08c(animation, &query);
    UnkChaseTrack_l5134a8* track = reinterpret_cast<UnkChaseTrack_l5134a8*>(query.result);
    if (!track || !found)
        return;

    UnkChaseRequest_l5134a8 request;
    const UnkChaseDesc_l5134a8* desc = reinterpret_cast<const UnkChaseDesc_l5134a8*>(&g_unk_710164b958);
    u64 desc0 = desc->words[0];
    u64 desc1 = desc->words[1];
    const UnkChaseBlend_l5134a8* blend = reinterpret_cast<const UnkChaseBlend_l5134a8*>(&g_unk_7172769740);
    u64 kind = blend->kind;
    u32 value = blend->value;
    u32 mask = UnkAll_SpitSend().value;
    request.id = -1;
    request.desc[0] = desc0;
    request.desc[1] = desc1;
    request.blend_kind = kind;
    request.blend_value = value;
    request.flags = 2;
    request.mask = mask;
    track->vf_0x18(&request);
    if (request.id == -1)
        request.id = fn_7101177ae8(animation, 0xf4061ff25a2163d6)->data->Key();
    if (!fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(animation),
                       reinterpret_cast<const UnkRequest_7101176ea8*>(&request), 0xf4061ff25a2163d6))
        return;

    CBlackboard* blackboard = &ai->blackboard;
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
        if (!blackboard->field_0xe8) {
            v.type = fn_7100009658();
            v.value = &g_unk_7101d470e0;
            fn_7100332c80(section, &g_unk_71726debd8, &v);
        }
    }
}
