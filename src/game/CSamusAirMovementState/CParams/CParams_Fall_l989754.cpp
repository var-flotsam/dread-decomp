// The CSamusAirMovementState::CParams unit: while the vertical speed is below zero, plays the tweak-fall
// request the current fall track fills in (the forward one while the horizontal speed is not zero, otherwise
// the plain one while a fall-forward track plays). Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkFallDesc_l989754 {
    u64 words[2];
};

struct UnkFallBlend_l989754 {
    u64 kind;
    u32 value;
};

struct UnkFallRequest_l989754 {
    s64 id;
    u64 desc[2];     // 0x8
    u64 blend_kind;  // 0x18
    u32 blend_value; // 0x20
    u32 flags;       // 0x24
    u32 mask;        // 0x28
};

// The track that fills the request in.
struct UnkFallTrack_l989754 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkFallRequest_l989754* request);
};

struct UnkAirParams_l989754 {
    u8 pad_0x0[0x7c];
    f32 speed_x;  // 0x7c
    u8 pad_0x80[0x1dc - 0x80];
    f32 speed_y;  // 0x1dc
};

struct UnkAirOwner_l989754 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkAirState_l989754 {
    u8 pad_0x0[0x30];
    UnkAirOwner_l989754* owner;     // 0x30
    UnkAirParams_l989754* params;   // 0x38
};

static inline bool IsNearZero(f32 v) {
    const f32 eps = 0.001f;
    return v - eps <= 0.0f && v + eps >= 0.0f;
}

static inline void FillFromTrack(UnkAnimationComponent* animation, UnkTrackQuery& query,
                                 const base::reflection::CClass* cls, UnkFallRequest_l989754* request) {
    query.cls = cls;
    query.result = nullptr;
    query.value = -1.0f;
    fn_710117b08c(animation, &query);
    if (query.result)
        reinterpret_cast<UnkFallTrack_l989754*>(query.result)->vf_0x18(request);
}

void fn_7100989754(UnkAirState_l989754* self) {
    f32 speed_y = self->params->speed_y;
    if (!(!IsNearZero(speed_y) && speed_y < 0.0f))
        return;
    UnkAnimationComponent* animation = self->owner->animation;

    UnkFallRequest_l989754 request;
    const UnkFallDesc_l989754* desc = reinterpret_cast<const UnkFallDesc_l989754*>(&g_unk_710164b958);
    u64 desc0 = desc->words[0];
    u64 desc1 = desc->words[1];
    const UnkFallBlend_l989754* blend = reinterpret_cast<const UnkFallBlend_l989754*>(&g_unk_7172769740);
    u64 kind = blend->kind;
    u32 value = blend->value;
    u32 mask = UnkAll_SpitSend().value;
    request.id = -1;
    request.desc[0] = desc0;
    request.desc[1] = desc1;
    request.blend_kind = kind;
    request.blend_value = value;
    request.flags = 0;
    request.mask = mask;

    UnkTrackQuery query;
    if (!IsNearZero(self->params->speed_x))
        FillFromTrack(animation, query, fn_7100c5741c(), &request);
    else if (fn_710117b348(animation, fn_7100c19a3c()))
        FillFromTrack(animation, query, fn_7100d2fa08(), &request);

    if (request.id == -1)
        return;
    fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(animation),
                  reinterpret_cast<const UnkRequest_7101176ea8*>(&request), 0xf4061ff25a2163d6);
}
