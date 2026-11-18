// CShakernautPiercingLaserAttack's unit: picks the id of the next animation. While the owner's usual
// track kind plays g_unk_7172705ce8, the right or left id by the AI component's facing; otherwise, from
// the playing CCharClassShakernautInitAttackTrack record, one of its two ids by whether the point the
// record's timeline reaches along the facing passes the AI's test. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkMotionRule.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

extern CStrId g_unk_7172705c70 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172705c78 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172705ce8 __attribute__((visibility("hidden")));

struct UnkStruct_71007142e8 {
    f32 m[12];
    u8 pad_0x30[0x40];
};

struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

// The pooled data of a string id: the string at +0x10, whose key is the id's hash.
struct UnkStrIdData_l6a6eac {
    u8 pad_0x0[0x10];
    CRntString str;
};

// The init attack track record: the id played when the reach passes (+0x18) and the one when not (+0x38).
struct UnkLaserTrack_l6a6eac {
    u8 pad_0x0[0x18];
    CStrId hit_id;  // 0x18
    u8 pad_0x20[0x18];
    CStrId miss_id;  // 0x38
};

struct UnkShakernautAI_l6a6eac {
    u8 pad_0x0[0x14810];
    f32 facing_x;  // 0x14810
    f32 facing_y;  // 0x14814
};

struct UnkShakernautAttack_l6a6eac {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
};

// The AI component as a CShakernautAIComponent, or null when it is none.
static inline UnkShakernautAI_l6a6eac* ShakernautAI(UnkReflObject* object) {
    if (!object)
        return nullptr;
    if (!fn_710009d50c(object->vf_0x0(), fn_71006ad0a4()))
        return nullptr;
    return reinterpret_cast<UnkShakernautAI_l6a6eac*>(object);
}

bool fn_71006af438(UnkShakernautAI_l6a6eac* self, const CVector3D* target, bool* out);

const CStrId* fn_71006a6eac(UnkShakernautAttack_l6a6eac* self) {
    CStrId id(*fn_7101177ae8(static_cast<UnkAnimationComponent*>(self->owner->field_0x220),
                             0xf4061ff25a2163d6));
    UnkShakernautAI_l6a6eac* ai = ShakernautAI(static_cast<UnkReflObject*>(self->owner->field_0x240));
    if (id == g_unk_7172705ce8) {
        f32 along = ai->facing_x * 1.0f + ai->facing_y * 0.0f;
        return along > 0.0f ? &g_unk_7172705c70 : &g_unk_7172705c78;
    }

    UnkTrackQuery query = {fn_7100c4dd48(), nullptr, -1.0f};
    fn_710117b08c(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), &query);
    if (!query.result)
        return &g_unk_7101d470e0;
    CStrId name(query.result->sources);
    UnkStruct_7101176ea8* animations = static_cast<UnkStruct_7101176ea8*>(self->owner->field_0x220);
    if (!animations)
        return &g_unk_7101d470e0;
    s64 key = reinterpret_cast<UnkStrIdData_l6a6eac*>(name.data)->str.key;
    UnkTimeline* timeline = static_cast<UnkTimeline*>(fn_7101176d34(animations, reinterpret_cast<void*>(key)));
    if (!timeline)
        return &g_unk_7101d470e0;

    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    {
        UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
        fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    }
    f32 reach = motion.m[11];
    UnkActor* owner = self->owner;
    CVector3D target = {owner->field_0x44 + reach * ai->facing_x, owner->field_0x54 + reach * ai->facing_y,
                        owner->field_0x64 + 0.0f};
    bool out = false;
    UnkLaserTrack_l6a6eac* track = reinterpret_cast<UnkLaserTrack_l6a6eac*>(query.result);
    return fn_71006af438(ai, &target, &out) ? &track->hit_id : &track->miss_id;
}
