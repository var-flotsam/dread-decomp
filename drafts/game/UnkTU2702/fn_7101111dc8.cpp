// CAIComponent's unit (unit 2702, hint only): the message handler for a subarea change. When the message's
// flag is set, the AI is enabled and its attack is running, and the player exists: unless the owner keeps
// its attack (a CCharClassKeepAttackOnSubareaChangeTrack), or is near the player and not told to abort
// (CCharClassAbortAttackOnSubareaChangeTrack), the attack is stopped and the owner is sent the track the
// AI names, with mode 2. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CAttackComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::global::CStrId;

struct UnkTrackId_1111dc8 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkAttack_1111dc8 {
    void Stop() {
        reinterpret_cast<void (*)(UnkAttack_1111dc8*)>((*reinterpret_cast<void***>(this))[0x178 / 8])(this);
    }
};

struct UnkVec3_1111dc8 {
    f32 x, y, z;

    f32 SquaredLength() const { return x * x + y * y + z * z; }
};
inline UnkVec3_1111dc8 operator-(const UnkVec3_1111dc8& a, const UnkVec3_1111dc8& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

// An actor: its transform's translation column.
struct UnkActor_1111dc8 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38
    u8 pad_0x68[0x220 - 0x68];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x50];
    UnkAttack_1111dc8* attack;  // 0x278

    UnkVec3_1111dc8 Pos() const { return {transform[0][3], transform[1][3], transform[2][3]}; }
};

struct UnkAISettings_1111dc8 {
    u8 pad_0x0[0x4f0];
    bool enabled;  // 0x4f0
};

struct UnkAI_1111dc8 {
    u8 pad_0x0[0x30];
    UnkActor_1111dc8* owner;  // 0x30
    u8 pad_0x38[0x1610 - 0x38];
    UnkAISettings_1111dc8* settings;  // 0x1610

    // The track sent when the attack is dropped (vtable slot 0x6b0).
    CStrId DropTrack() {
        return reinterpret_cast<CStrId (*)(UnkAI_1111dc8*)>((*reinterpret_cast<void***>(this))[0x6b0 / 8])(this);
    }
};

struct UnkMsg_1111dc8 {
    u8 pad_0x0[0x20];
    bool flag;  // 0x20
};

// The distance beyond which the attack is always dropped.
struct UnkAITunable_1111dc8 {
    u8 pad_0x0[0x34];
    f32 distance;  // 0x34
};

u32 fn_7101111dc8(UnkNailongAI_b46a2* ai, UnkNailongMsg_b46a2* message) {
    UnkAI_1111dc8* self = reinterpret_cast<UnkAI_1111dc8*>(ai);
    UnkMsg_1111dc8* msg = reinterpret_cast<UnkMsg_1111dc8*>(message);
    if (!msg->flag || !self->settings->enabled)
        return 0;
    if (!self->owner->attack || !fn_7101136308(self->owner->attack))
        return 0;
    UnkActor_1111dc8* player = reinterpret_cast<UnkActor_1111dc8*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return 0;
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return 0;
    UnkTrackQuery keep = {fn_7100c37d18(), nullptr, -1.0f};
    fn_710117b08c(animation, &keep);
    if (keep.result && fn_710117b348(animation, fn_7100c37d18()))
        return 0;
    UnkActor_1111dc8* owner = self->owner;
    UnkVec3_1111dc8 d = owner->Pos() - player->Pos();
    f32 limit = reinterpret_cast<const UnkAITunable_1111dc8*>(&g_unk_71727bc558)->distance;
    if (!(d.SquaredLength() > limit * limit)) {
        UnkTrackQuery query = {fn_7100bef064(), nullptr, -1.0f};
        fn_710117b08c(animation, &query);
        if (query.result && !fn_710117b348(animation, fn_7100bef064()))
            return 0;
    }
    self->owner->attack->Stop();
    CStrId track = self->DropTrack();
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 2;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(animation, reinterpret_cast<UnkTrackId_1111dc8*>(track.data)->hash, 0xf4061ff25a2163d6, &mode,
                  &g_unk_710164b958, &g_unk_7172769740, &mask);
    return 0;
}
