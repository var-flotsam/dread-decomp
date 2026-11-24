// Unit 437 (CAimComponent): while the aim angle is between a quarter turn and its limit and the facing
// differs from the aim's side, plays the animation the owner's CCharClassTweakInvertAnalogAimActionTrack
// asks for, or else the one the current action's timeline event asks for. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkXformNode.h"
#include "game/nn/time.h"
#include "types.h"

void* fn_7101177cac(UnkStruct_7101176ea8* self, u64 hash);

struct UnkInvertDesc_l35343c {
    u64 words[2];
};

struct __attribute__((packed, aligned(4))) UnkInvertBlend_l35343c {
    u64 kind;
    f32 value;
};

// An animation request: unset while the id is -1.
struct UnkInvertRequest_l35343c {
    s64 id;
    u64 desc[2];      // 0x8
    u64 blend_kind;   // 0x18
    u32 blend_value;  // 0x20
    u32 flags;        // 0x24
    u32 mask;         // 0x28

    UnkInvertRequest_l35343c() { Reset(); }

    void Reset() {
        const UnkInvertDesc_l35343c* d = reinterpret_cast<const UnkInvertDesc_l35343c*>(&g_unk_710164b958);
        u64 desc0 = d->words[0];
        u64 desc1 = d->words[1];
        const UnkInvertBlend_l35343c* blend = reinterpret_cast<const UnkInvertBlend_l35343c*>(&g_unk_7172769740);
        u64 kind = blend->kind;
        u32 value = *reinterpret_cast<const u32*>(&blend->value);
        u32 all = UnkAll_SpitSend().value;
        id = -1;
        desc[0] = desc0;
        desc[1] = desc1;
        blend_kind = kind;
        blend_value = value;
        flags = 0;
        mask = all;
    }
};

// A track or timeline event that fills a request in.
struct UnkInvertSource_l35343c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkInvertRequest_l35343c* request);
};

struct UnkInvertKey_l35343c {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkInvertAction_l35343c {
    u8 pad_0x0[0x24];
    s32 frames;  // 0x24
    u8 pad_0x28[0x148];
    u8 events[1];  // 0x170
};
static_assert(__builtin_offsetof(UnkInvertAction_l35343c, events) == 0x170, "layout");

struct UnkInvertAim_l35343c {
    u8 pad_0x0[0x58];
    f32 side;  // 0x58
};

struct UnkInvertOwner_l35343c {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x15c];
    UnkInvertAim_l35343c* aim;  // 0x1f0
    u8 pad_0x1f8[0x10];
    void* field_0x208;  // 0x208
    u8 pad_0x210[0x10];
    UnkAnimationComponent* animation;  // 0x220
};
static_assert(__builtin_offsetof(UnkInvertOwner_l35343c, aim) == 0x1f0, "layout");
static_assert(__builtin_offsetof(UnkInvertOwner_l35343c, animation) == 0x220, "layout");

struct UnkInvertSelf_l35343c {
    u8 pad_0x0[0x30];
    UnkInvertOwner_l35343c* owner;  // 0x30
    u8 pad_0x38[0x280];
    f32 angle;  // 0x2b8
};
static_assert(__builtin_offsetof(UnkInvertSelf_l35343c, angle) == 0x2b8, "layout");

static const u64 kInvertKey = 0xf4061ff25a2163d6;

bool fn_710035343c(UnkInvertSelf_l35343c* self) {
    UnkInvertOwner_l35343c* owner = self->owner;
    f32 limit = owner->field_0x208 ? 4.712389f : 4.1887903f;
    if (!((owner->aim != nullptr) & (self->angle < limit) & (self->angle > 1.5707964f)))
        return false;
    f32 aimSide = owner->aim->side;
    f32 y = owner->xform.GetAngle().y;
    s32 side = (y > 0.0f) - (y < 0.0f);
    if (aimSide == (f32)side)
        return false;
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return false;
    UnkStruct_7101176ea8* anim = reinterpret_cast<UnkStruct_7101176ea8*>(animation);

    UnkTrackQuery query = {fn_7100c5c0fc(), nullptr, -1.0f};
    fn_710117b08c(animation, &query);
    UnkInvertRequest_l35343c request;
    if (UnkInvertSource_l35343c* track = reinterpret_cast<UnkInvertSource_l35343c*>(query.result)) {
        track->vf_0x18(&request);
        if (request.id != -1)
            return CAnimationComponent_SendActionRequestRecordToTrack(anim, reinterpret_cast<const UnkRequest_7101176ea8*>(&request), kInvertKey);
    }
    UnkInvertKey_l35343c* key = *static_cast<UnkInvertKey_l35343c**>(fn_7101177cac(anim, kInvertKey));
    UnkInvertAction_l35343c* action =
        static_cast<UnkInvertAction_l35343c*>(fn_7101176d34(anim, reinterpret_cast<void*>(key->hash)));
    if (!action)
        return false;
    UnkInvertSource_l35343c* event = reinterpret_cast<UnkInvertSource_l35343c*>(
        fn_710006bb80((f32)(action->frames - 1), reinterpret_cast<UnkTimelineEventList*>(action->events),
                      fn_7100c5c0fc(), action->frames));
    if (!event)
        return false;
    request.Reset();
    event->vf_0x18(&request);
    if (request.id == -1)
        return false;
    return CAnimationComponent_SendActionRequestRecordToTrack(anim, reinterpret_cast<const UnkRequest_7101176ea8*>(&request), kInvertKey);
}
