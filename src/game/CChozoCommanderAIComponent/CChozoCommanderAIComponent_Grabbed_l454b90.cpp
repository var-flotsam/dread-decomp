// CChozoCommanderAIComponent's unit (tools/tu_map.py unit 665): fn_7100454b90 starts the grabbed state:
// keys the owner's timeline, works out how far the owner is from the arena edge it faces, sends the grab
// track to the owner's character class and then to its target's, and points the sound player at the
// owner's grab collider. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CShotManager/CTunableShotManager.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2702.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

extern const u64 g_unk_71016496f0;
extern base::global::CStrId g_unk_71726c94d8;

struct UnkScorpiusAI_b69a4;

struct UnkChozoCommanderOwner_l454b90 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38, the transform (fn_710002614c)
    u8 pad_0x78[0x218 - 0x78];
    struct UnkSub {
        u8 pad_0x0[0x1c08];
        u8 flags_0x1c08[2];  // byte flags indexed by kind
    }* field_0x218;
    void* field_0x220;
    u8 pad_0x228[0x2a0 - 0x228];
    UnkCollisionContainer* field_0x2a0;
    u8 pad_0x2a8[0x2e8 - 0x2a8];
    UnkTimelineComponent* field_0x2e8;

    f32 PosX() const { return xform.m[3]; }
};

struct UnkChozoCommanderAI_l454b90 {
    u8 pad_0x0[0x30];
    UnkChozoCommanderOwner_l454b90* owner;
    u8 pad_0x38[0x1140 - 0x38];
    u32 field_0x1140;
    u8 pad_0x1144[0x157f0 - 0x1144];
    u8 entry_0x157f0[0x15d14 - 0x157f0];
    u32 field_0x15d14;
    u8 pad_0x15d18[4];
    f32 minX;  // 0x15d1c
    u8 pad_0x15d20[4];
    f32 maxX;  // 0x15d24
    u8 pad_0x15d28[0x15eb8 - 0x15d28];
    bool field_0x15eb8;
    bool field_0x15eb9;
    u8 pad_0x15eba[2];
    f32 edgeX;  // 0x15ebc
    f32 edgeY;
    f32 edgeZ;

    // The grab track: the second one once the entry at +0x157f0 is current with +0x15eb8 set.
    u64 GrabTrack() {
        if (field_0x1140 == 0 && !field_0x15eb9) {
            if (fn_7101105970(reinterpret_cast<UnkScorpiusAI_b69a4*>(this)) == entry_0x157f0 && field_0x15eb8) {
                field_0x15eb9 = true;
                return 0x46b21f327512700c;
            }
            return 0x8752c4f4acfc9b47;
        }
        return 0x46b21f327512700c;
    }
};
static_assert(__builtin_offsetof(UnkChozoCommanderAI_l454b90, field_0x15eb8) == 0x15eb8, "layout");

// Sends the track with mode 2 and the all-bits mask; the caller owns the two arguments.
static inline bool SendTrack(void* target, u64 hash, base::reflection::UnkReflTypeIdArg& mode,
                             base::reflection::UnkReflTypeIdArg& mask) {
    mode.id = 2;
    mask.id = UnkAll_SpitSend().value;
    return CAnimationComponent_SendActionToTrack(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

void CChozoCommanderAIComponent_StartGrab(UnkChozoCommanderAI_l454b90* self) {
    // At function scope: none of their stack slots is shared, as in the original.
    base::reflection::UnkReflTypeIdArg mode, mask, targetMode, targetMask;
    self->field_0x15d14 = 0;
    if (UnkTimelineComponent* timeline = self->owner->field_0x2e8)
        fn_7100a2b7b8(timeline, &g_unk_71016496f0);
    if (UnkChozoCommanderOwner_l454b90::UnkSub* sub = self->owner->field_0x218)
        sub->flags_0x1c08[1] = 0;
    void* cls = self->owner->field_0x220;
    if (!cls || !fn_710117b348(static_cast<UnkAnimationComponent*>(cls), fn_7100cef3e8()))
        return;

    f32 dist;
    if (fn_710002614c(&self->owner->xform).m[2] >= 0.0f)
        dist = self->maxX - self->owner->PosX();
    else
        dist = self->owner->PosX() - self->minX;
    if (dist < 1250.0f) {
        self->edgeX = dist - 1250.0f;
        self->edgeY = 0.0f;
        self->edgeZ = 0.0f;
    }

    if (!SendTrack(cls, self->GrabTrack(), mode, mask))
        return;
    if (UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)))
        if (void* targetCls = target->field_0x220)
            SendTrack(targetCls, self->GrabTrack(), targetMode, targetMask);
    if (UnkCollisionContainer* collision = self->owner->field_0x2a0)
        if (UnkCollisionObject* object = fn_71008640a4(collision, &g_unk_71726c94d8))
            fn_7100ed77b8(g_unk_71726af0b0, object);
}
