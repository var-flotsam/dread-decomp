// UnkTU1501 (CSamusMovement's unit, tools/tu_map.py unit 1501): picks Samus's stealth animation. While a
// schedule-stealth track runs, only sets the flag at +0x51d4; otherwise asks the tweak-stealth (or
// tweak-stealth-end) track to fill in a request and plays it. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2689.h"
#include "types.h"


struct UnkStealthDesc_l9e4598 {
    u64 words[2];
};

struct __attribute__((packed, aligned(4))) UnkStealthBlend_l9e4598 {
    u64 kind;
    f32 value;
};

// An animation request: unset while the id is -1.
struct UnkStealthRequest_l9e4598 {
    s64 id;
    u64 desc[2];      // 0x8
    u64 blend_kind;   // 0x18
    u32 blend_value;  // 0x20
    u32 flags;        // 0x24
    u32 mask;         // 0x28
};

// The track a query resolves, as it fills in a request.
struct UnkStealthTrack_l9e4598 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkStealthRequest_l9e4598* request);
    u8 pad_0x8[0x10];
    base::global::CStrId sources;  // 0x18
};

struct UnkStealthTunable_l9e4598 {
    u8 pad_0x0[0x38];
    bool field_0x38;
    bool field_0x39 : 1;
};

struct UnkStealthOwner_l9e4598 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkStealthState_l9e4598 {
    u8 pad_0x0[0x10];
    s32 field_0x10;
};

struct UnkStealthMovement_l9e4598 {
    u8 pad_0x0[0x30];
    UnkStealthOwner_l9e4598* owner;  // 0x30
    u8 pad_0x38[0x3a8 - 0x38];
    bool field_0x3a8;
    u8 pad_0x3a9[0x7e4 - 0x3a9];
    bool stealth;  // 0x7e4
    u8 pad_0x7e5[0x2d08 - 0x7e5];
    UnkStealthState_l9e4598* field_0x2d08;
    u8 pad_0x2d10[0x51d4 - 0x2d10];
    bool scheduled;  // 0x51d4
};

// Whether a query found a track with sources.
static inline bool HasSources_l9e4598(const UnkTrackQuery& query) {
    return query.result && query.result->sources.data != &g_unk_7101d47108->empty;
}

static inline UnkStealthTrack_l9e4598* Track_l9e4598(const UnkTrackQuery& query) {
    return reinterpret_cast<UnkStealthTrack_l9e4598*>(query.result);
}

void fn_71009e4598(UnkSamusMovementUpdate* movement) {
    UnkStealthMovement_l9e4598* self = reinterpret_cast<UnkStealthMovement_l9e4598*>(movement);
    UnkAnimationComponent* animation = self->owner->animation;
    if (animation && fn_710117b348(animation, fn_7100d263b8())) {
        self->scheduled = true;
        return;
    }
    self->scheduled = false;

    const UnkStealthDesc_l9e4598* desc = reinterpret_cast<const UnkStealthDesc_l9e4598*>(&g_unk_710164b958);
    u64 desc0 = desc->words[0];
    u64 desc1 = desc->words[1];
    const UnkStealthBlend_l9e4598* blend = reinterpret_cast<const UnkStealthBlend_l9e4598*>(&g_unk_7172769740);
    u64 kind = blend->kind;
    u32 value = *reinterpret_cast<const u32*>(&blend->value);
    u32 mask = UnkAll_SpitSend().value;
    UnkStealthRequest_l9e4598 request;
    request.id = -1;
    request.desc[0] = desc0;
    request.desc[1] = desc1;
    request.blend_kind = kind;
    request.blend_value = value;
    request.flags = 1;
    request.mask = mask;

    if (self->stealth) {
        if (fn_7100df5874(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310)) && self->field_0x3a8) {
            UnkStealthState_l9e4598* state = self->field_0x2d08;
            if (!state || state->field_0x10 != 3)
                request.id = 0xf646dbcca84b1693;
        }
        UnkTrackQuery query = {fn_7100d33d28(), nullptr, -1.0f};
        if (UnkAnimationComponent* animation = self->owner->animation) {
            fn_710117b08c(animation, &query);
            if (HasSources_l9e4598(query)) {
                const UnkStealthTunable_l9e4598* tunable =
                    reinterpret_cast<const UnkStealthTunable_l9e4598*>(&g_unk_7172a03d80);
                if (!fn_710117ae38(self->owner->animation, 0xc5528c327ce1413a, false) ||
                    (!tunable->field_0x38 && tunable->field_0x39))
                    Track_l9e4598(query)->vf_0x18(&request);
            }
        }
    } else {
        UnkTrackQuery query = {fn_7100d342c8(), nullptr, -1.0f};
        if (UnkAnimationComponent* animation = self->owner->animation) {
            fn_710117b08c(animation, &query);
            if (HasSources_l9e4598(query))
                Track_l9e4598(query)->vf_0x18(&request);
        }
    }

    if (request.id != -1)
        fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(self->owner->animation),
                      reinterpret_cast<const UnkRequest_7101176ea8*>(&request), 0xf4061ff25a2163d6);
}
