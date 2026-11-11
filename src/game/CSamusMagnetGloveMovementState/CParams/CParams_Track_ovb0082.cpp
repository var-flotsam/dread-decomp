// In the CSamusMagnetGloveMovementState::CParams unit: the track requests of the surface kind at +0x1c0
// (wall, ceiling, ...): fn_71009ac4ac takes the id from the table g_unk_710164b3f8; fn_71009ac5f8 the id from
// g_unk_710164b410 with its own time, then lets the current CCharClassToRelaxTrack fill it in. Names and layouts
// are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "types.h"

struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;

struct UnkTrackTime_ovb0082 {
    u64 key;
    f32 time;
    u32 count;
};
struct UnkTrackBlend_ovb0082 {
    u32 a, b, c;
};

// A track request (0x30 bytes): no id, the default time and blend descriptors, flag bit 0 set and an
// all-bits mask.
struct UnkTrackParams_ovb0082 {
    s64 id;
    UnkTrackTime_ovb0082 time;
    UnkTrackBlend_ovb0082 blend;
    u32 flags;
    u32 mask;

    __attribute__((always_inline)) UnkTrackParams_ovb0082(UnkTrackTime_ovb0082 time_, UnkTrackBlend_ovb0082 blend_,
                                                          u32 mask_)
        : id(-1), time(time_), blend(blend_), mask(mask_) { flags = 1; }
    __attribute__((always_inline)) UnkTrackParams_ovb0082()
        : UnkTrackParams_ovb0082(reinterpret_cast<const UnkTrackTime_ovb0082&>(g_unk_710164b958),
                                 reinterpret_cast<const UnkTrackBlend_ovb0082&>(g_unk_7172769740),
                                 base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>()) {}
};
static_assert(sizeof(UnkTrackParams_ovb0082) == 0x30, "layout");

struct UnkTrackRecord_ovb0082 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkTrackParams_ovb0082* params);  // fills in the request
};

struct UnkTrackOwner_ovb0082 {
    u8 unk_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkMagnetTrackState_ovb0082 {
    u8 unk_0x0[0x30];
    UnkTrackOwner_ovb0082* owner;  // 0x30
    u8 unk_0x38[0x188];
    s32 surface;  // 0x1c0
};

extern const s64 g_unk_710164b3f8[3] __attribute__((visibility("hidden")));
extern const s64 g_unk_710164b410[3] __attribute__((visibility("hidden")));

UnkTrackParams_ovb0082 fn_71009ac4ac(UnkMagnetTrackState_ovb0082* self) {
    UnkTrackParams_ovb0082 params;
    if (static_cast<u32>(self->surface) <= 2)
        params.id = g_unk_710164b3f8[self->surface];
    return params;
}

UnkTrackParams_ovb0082 fn_71009ac5f8(UnkMagnetTrackState_ovb0082* self) {
    UnkTrackParams_ovb0082 params;
    if (static_cast<u32>(self->surface) <= 2) {
        params.id = g_unk_710164b410[self->surface];
        params.time.key = -1;
        params.time.time = 0.4f;
        params.time.count = 1;
    }
    UnkTrackQuery query = {fn_7100d28cc8(), nullptr, -1.0f};
    if (fn_710117b08c(self->owner->animation, &query))
        reinterpret_cast<UnkTrackRecord_ovb0082*>(query.result)->vf_0x18(&params);
    return params;
}
