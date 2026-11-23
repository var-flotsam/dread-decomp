// CChozoWarriorXLandAttack's start: after the base start, when the owner's AI (+0x240) is a
// CChozoWarriorXAIComponent and the owner has an animation (+0x220), plays the landing track, sets the
// AI's landing speed scale to the middle of its two heights over the track's whole z motion, and hooks
// the callback fn_71004837a0 up to the animation. Names and layouts are placeholders.
#include "../../../src/game_5821e4/CAttackDtor.h"
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAttack.h"
#include "game/UnkMotionRule.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkAttackBase;

// Returned in memory (it has a copy constructor).
struct UnkStrIdOut_b0266 {
    UnkAttackStrIdRaw raw;
    UnkStrIdOut_b0266(const UnkStrIdOut_b0266&);

    void Release() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (raw.data && raw.data != &pool->empty) {
                --raw.data->ref_count;
                if (raw.data->ref_count == 0) {
                    fn_71000803a4(pool, raw.data);
                    pool = g_unk_7101d47108;
                }
            }
            raw.data = &pool->empty;
        }
    }
};
UnkStrIdOut_b0266 fn_710049c59c(UnkReflObject* ai);

struct UnkTimelineId {
    u64 value;
};
UnkTimelineId fn_7101177be0(UnkAnimationComponent* self, u64 key);

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

struct UnkWarriorXAI_l482624 {
    u8 pad_0x0[0x149ac];
    f32 land_scale;  // 0x149ac
};

// The AI's high and low landing heights.
void fn_71010fc5fc(UnkWarriorXAI_l482624* self, f32* out);
void fn_71010fc554(UnkWarriorXAI_l482624* self, f32* out);
void fn_71004837a0();

struct UnkLandLink_l482624 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkLandOwner_l482624 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkReflObject* ai;  // 0x240
};

struct UnkLandAttack_l482624 {
    u8 pad_0x0[0x8];
    UnkLandOwner_l482624* owner;
    u8 pad_0x10[0x38];
    UnkLandLink_l482624 link;  // 0x48
    UnkWeakHandle* handle;     // 0x60
};

void fn_7100482624(UnkLandAttack_l482624* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    UnkReflObject* object = self->owner->ai;
    if (!object)
        return;
    UnkWarriorXAI_l482624* ai = fn_710009d50c(object->vf_0x0(), fn_710048db44())
                                    ? reinterpret_cast<UnkWarriorXAI_l482624*>(object)
                                    : nullptr;
    if (!ai)
        return;
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return;

    UnkStrIdOut_b0266 id = fn_710049c59c(reinterpret_cast<UnkReflObject*>(ai));
    id.Release();
    const u64 track = 0x6dda625e7c0919e0;
    const u64 kind = 0xf4061ff25a2163d6;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 2;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(animation, track, kind, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);

    f32 high = 0.0f;
    f32 low = 0.0f;
    fn_71010fc5fc(ai, &high);
    fn_71010fc554(ai, &low);
    f32 middle = (high + low) * 0.5f;
    UnkTimeline* timeline = static_cast<UnkTimeline*>(
        fn_7101176e0c(reinterpret_cast<UnkStruct_7101176ea8*>(animation), track,
                      fn_7101177be0(self->owner->animation, kind).value));
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    ai->land_scale = middle / motion.m[11];

    UnkLandLink_l482624* link = &self->link;
    link->context = self;
    link->callback = fn_71004837a0;
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    fn_7100316018(reinterpret_cast<UnkWeakTarget*>(animation), &self->handle);
    fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
}
