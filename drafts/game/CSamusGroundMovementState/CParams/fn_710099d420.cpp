// CSamusGroundMovementState::CParams' unit (tools/tu_map.py): handles an event of kind 1 whose source
// is idle. While a CCharClassFloorSlideRunTurnTrack plays, the run-turn track id goes to the animation
// (fn_7101176690) or, in a request with flag bit 1, to the target (fn_710098ae04); otherwise the event is
// taken only while both the converting-to-morph-ball and climbing tracks play. Names and layouts are
// placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1455.h"
#include "game/UnkTU1730.h"
#include "types.h"

struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;

struct UnkGroundTrackTime_l99 {
    u64 key;
    u64 time;
};
struct UnkGroundTrackBlend_l99 {
    u32 a, b, c;
};

struct UnkGroundTrackParams_l99 {
    s64 id;
    UnkGroundTrackTime_l99 time;
    UnkGroundTrackBlend_l99 blend;
    u32 flags;
    u32 mask;

    UnkGroundTrackParams_l99(UnkGroundTrackTime_l99 time_, UnkGroundTrackBlend_l99 blend_, u32 mask_)
        : id(-1), time(time_), blend(blend_), flags(0), mask(mask_) {}
    UnkGroundTrackParams_l99()
        : UnkGroundTrackParams_l99(reinterpret_cast<const UnkGroundTrackTime_l99&>(g_unk_710164b958),
                                   reinterpret_cast<const UnkGroundTrackBlend_l99&>(g_unk_7172769740),
                                   base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>()) {}
};

// The reflected params base (vtable g_unk_7101a22cb8) holding the track params.
struct UnkGroundParamsBase_l99 {
    void* vtable = g_unk_7101a22cb8 + 2;
    UnkGroundTrackParams_l99 params;  // 0x8
};

struct UnkGroundWordPair_l99 {
    u32 a;
    u32 b;
};

// The track request (vtable g_unk_7101a22d18).
struct UnkGroundParkourRequest_l99 : UnkGroundParamsBase_l99 {
    u32 field_0x38;
    UnkGroundWordPair_l99 field_0x3c;
    u32 field_0x44;

    UnkGroundParkourRequest_l99() : field_0x38(3), field_0x3c(), field_0x44(0) {
        vtable = g_unk_7101a22d18 + 2;
    }
};

struct UnkGroundOwner_l99 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkGroundTargetMode_l99 {
    u8 pad_0x0[0x10];
    s32 mode;
};

struct UnkGroundTarget_l99 {
    u8 pad_0x0[0x10];
    UnkGroundTargetMode_l99* mode;  // 0x10
};

struct UnkGroundState_l99 {
    void* vtable;
    UnkGroundTarget_l99* target;  // 0x8
    u8 pad_0x10[0x20];
    UnkGroundOwner_l99* owner;  // 0x30
};

struct UnkGroundEventSourceState_l99d420 {
    u8 pad_0x0[0x14];
    s32 busy;
};

struct UnkGroundEventSource_l99d420 {
    u8 pad_0x0[0x10];
    UnkGroundEventSourceState_l99d420* state;  // 0x10
};

struct UnkGroundEvent_l99d420 {
    s32 kind;
    UnkGroundEventSource_l99d420* source;  // 0x8
};

bool fn_710099d420(UnkGroundState_l99* self, const UnkGroundEvent_l99d420* event) {
    if (!event->source || event->kind != 1 || event->source->state->busy != 0)
        return false;
    if (!fn_710117b348(self->owner->animation, fn_7100c1bda8()))
        return fn_710117b348(self->owner->animation, fn_7100cf2ae8()) &&
               fn_710117b348(self->owner->animation, fn_7100cf25d8());

    const u64 track = 0x4a900487a7d725b8;
    base::reflection::UnkReflTypeIdArg value;
    base::reflection::UnkReflTypeIdArg mask;
    UnkGroundTargetMode_l99* mode = self->target->mode;
    if (mode && mode->mode == 2) {
        UnkAnimationComponent* animation = self->owner->animation;
        value.id = 0;
        mask.id = base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>();
        fn_7101176690(animation, track, 0xf4061ff25a2163d6, &value, &g_unk_710164b958, &g_unk_7172769740, &mask);
        return true;
    }
    UnkGroundParkourRequest_l99 req;
    req.params.id = track;
    req.params.flags |= 2;
    fn_710098ae04(reinterpret_cast<UnkAirTarget_B71a6*>(self->target), reinterpret_cast<UnkAirRequest_B71a6*>(&req));
    return true;
}
