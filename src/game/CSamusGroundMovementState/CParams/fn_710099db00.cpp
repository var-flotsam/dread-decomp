// CSamusGroundMovementState::CParams' unit (tools/tu_map.py unit 1462): unless told to skip or a state change
// is already pending, sends the state machine a ground request that the tweak-fall track fills in; while one of
// three animations plays it also gets a facing speed (boosted, fixed or from the params) and mode 1.
// Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAbilitySpeedBooster.h"
#include "game/CAnimationComponent.h"
#include "game/CSamusGroundMovementState/CParams.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkSamusGroundState.h"
#include "game/UnkTU1455.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "types.h"

namespace base { namespace reflection { struct UnkTunable; } }
__attribute__((visibility("hidden"))) extern UnkCls_710117af10* g_unk_717273be00;
struct UnkPlayer;

struct UnkGroundDesc_l99db00 {
    u64 words[2];
};

struct UnkGroundBlend_l99db00 {
    u32 a, b, c;
};

struct UnkSpeedBoosterTunable_l99db00 {
    u8 pad_0x0[0x30];
    f32 field_0x30;  // the speed factor while the speed booster runs
};

struct UnkGroundTrackParams_l99db00 {
    s64 id;
    u64 desc[2];      // 0x8
    UnkGroundBlend_l99db00 blend;  // 0x18
    u32 flags;                     // 0x24
    u32 mask;         // 0x28

    UnkGroundTrackParams_l99db00(UnkGroundDesc_l99db00 d, UnkGroundBlend_l99db00 b, u32 mask_)
        : id(-1), desc{d.words[0], d.words[1]}, blend(b), flags(0), mask(mask_) {}
};

struct UnkGroundRequestBase_l99db00 {
    void* vtable;
    UnkGroundRequestBase_l99db00() { vtable = g_unk_7101a22cb8 + 2; }
};

// The request's speed: x the facing speed.
struct UnkGroundSpeed_l99db00 {
    f32 x;
    f32 y;
};

struct UnkGroundRequest_l99db00 : UnkGroundRequestBase_l99db00 {
    UnkGroundTrackParams_l99db00 params;  // 0x8
    u32 mode;                             // 0x38
    UnkGroundSpeed_l99db00 speed;         // 0x3c
    u32 field_0x44;

    UnkGroundRequest_l99db00(const UnkGroundDesc_l99db00& d, const UnkGroundBlend_l99db00& b)
        : params(d, b, UnkAll_SpitSend().value) {
        mode = 3;
        speed = UnkGroundSpeed_l99db00();
        field_0x44 = 0;
        vtable = g_unk_7101a22ce8 + 0x10;
    }
};
static_assert(__builtin_offsetof(UnkGroundRequest_l99db00, mode) == 0x38, "layout");

// The track that fills the request in.
struct UnkGroundTrack_l99db00 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkGroundTrackParams_l99db00* params);
};

struct UnkAnimEntry_l99db00 {
    u8 pad_0x0[0x34];
    bool field_0x34 : 1;
};

static inline f32 Sign(f32 v) {
    return (f32)((v > 0.0f) - (v < 0.0f));
}

static inline bool IsPlaying(UnkAnimationComponent* animation, UnkCls_710117af10** name) {
    return fn_710117af10(reinterpret_cast<UnkStruct_710117af10*>(animation), name, false);
}

void fn_710099db00(UnkSamusGroundState* self, bool skip) {
    if (skip)
        return;
    if (self->machine->pending != 15)
        return;

    UnkGroundRequest_l99db00 request(reinterpret_cast<const UnkGroundDesc_l99db00&>(g_unk_710164b958),
                                     reinterpret_cast<const UnkGroundBlend_l99db00&>(g_unk_7172769740));
    request.params.id = GameManagerIdTable().field_0xff8.data->Key();

    UnkTrackQuery query = {fn_7100d2fa08(), nullptr, -1.0f};
    if (UnkAnimationComponent* animation = self->owner->animation) {
        fn_710117b08c(animation, &query);
        if (query.result)
            reinterpret_cast<UnkGroundTrack_l99db00*>(query.result)->vf_0x18(&request.params);
    }

    bool turning =
        IsPlaying(self->owner->animation, reinterpret_cast<UnkCls_710117af10**>(&GameManagerIdTable().field_0x1e50)) ||
        IsPlaying(self->owner->animation, reinterpret_cast<UnkCls_710117af10**>(&GameManagerIdTable().field_0x1d78));
    bool fixed = IsPlaying(self->owner->animation, &g_unk_717273be00);
    if (turning || fixed) {
        UnkAnimEntry_l99db00* entry = static_cast<UnkAnimEntry_l99db00*>(
            fn_7101176d34(reinterpret_cast<UnkStruct_7101176ea8*>(self->owner->animation),
                          reinterpret_cast<void*>(request.params.id)));
        if (!entry || !entry->field_0x34) {
            f32 facing = Sign(fn_710002614c(&self->owner->transform).m[2]);
            request.speed.x = self->params->field_0xd0 * facing;
        }

        UnkAbility* booster = fn_7100df5928(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310));
        if (booster && fn_710033cf20(booster))
            request.speed.x = reinterpret_cast<const UnkSpeedBoosterTunable_l99db00&>(g_unk_71726b0fc0).field_0x30 *
                            request.speed.x;
        else if (fixed)
            request.speed.x = Sign(request.speed.x) * 700.0f;
        else if (self->movement->field_0x7e4) {
            f32 sign = Sign(request.speed.x);
            request.speed.x = self->params->field_0xd4 * sign;
        }
        request.mode = 1;
        self->movement->field_0x7c8 = 0.2f;
    }
    fn_710098ae04(reinterpret_cast<UnkAirTarget_B71a6*>(self->machine), reinterpret_cast<UnkAirRequest_B71a6*>(&request));
}
