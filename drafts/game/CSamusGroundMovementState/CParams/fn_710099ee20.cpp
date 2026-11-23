// CSamusGroundMovementState::CParams' unit (tools/tu_map.py): the parkour fall step. The current
// CCharClassTweakParkourFallActionTrack fills in the track params; a valid track id that the animation
// knows (fn_710117ae38) gets flag bit 1, the aim holder adds the aim component's value, and the params go
// to the animation directly (fn_7101176ea8) or, wrapped in a request, to the target (fn_710098ae04).
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
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

// The record a track query resolves; slot 0x18 fills in track params.
struct UnkGroundTrackRecord_l99 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkGroundTrackParams_l99* params);
};

// The reflected params base (vtable g_unk_7101a22cb8) holding the track params.
struct UnkGroundParamsBase_l99 {
    void* vtable;
    UnkGroundTrackParams_l99 params;  // 0x8

    UnkGroundParamsBase_l99() { vtable = g_unk_7101a22cb8 + 2; }
};

// The parkour request (vtable g_unk_7101a22d18).
struct UnkGroundParkourRequest_l99 : UnkGroundParamsBase_l99 {
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;

    UnkGroundParkourRequest_l99() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x40 = 0;
        field_0x44 = 0;
        vtable = g_unk_7101a22d18 + 2;
    }
};

struct UnkGroundAimHolder_l99 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* aim;  // 0x10
};

struct UnkGroundAimComponent_l99 {
    u8 pad_0x0[0x94];
    f32 field_0x94;
};

struct UnkGroundOwner_l99 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2b0 - 0x228];
    UnkGroundAimHolder_l99* aim_holder;  // 0x2b0
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

void fn_71003529d4(UnkGroundAimHolder_l99* self, f32 value);

void fn_710099ee20(UnkGroundState_l99* self) {
    UnkGroundTrackParams_l99 params;
    UnkTrackQuery query = {fn_7100d32a38(), nullptr, -1.0f};
    fn_710117b08c(self->owner->animation, &query);
    if (!query.result)
        return;
    reinterpret_cast<UnkGroundTrackRecord_l99*>(query.result)->vf_0x18(&params);
    if (params.id != -1 && fn_710117ae38(self->owner->animation, params.id, false))
        params.flags |= 2;
    if (params.id == -1)
        return;

    UnkGroundAimHolder_l99* holder = self->owner->aim_holder;
    base::reflection::UnkReflObject* object = holder->aim;
    auto* aim = reinterpret_cast<UnkGroundAimComponent_l99*>(
        fn_710009d50c(object->vf_0x0(), fn_7100359cb0()) ? object : nullptr);
    fn_71003529d4(holder, aim->field_0x94);

    UnkGroundTargetMode_l99* mode = self->target->mode;
    if (mode && mode->mode == 2) {
        fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(self->owner->animation),
                      reinterpret_cast<const UnkRequest_7101176ea8*>(&params), 0xf4061ff25a2163d6);
        return;
    }
    UnkGroundParkourRequest_l99 req;
    req.params = params;
    fn_710098ae04(reinterpret_cast<UnkAirTarget_B71a6*>(self->target), reinterpret_cast<UnkAirRequest_B71a6*>(&req));
}
