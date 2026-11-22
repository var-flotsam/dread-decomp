// Unit 1454 (tools/tu_map.py): fn_7100984e54 takes a new swing value and kind. Kind 2 hands the grapple state
// its parameters with the tunable's braking friction; kind 1 picks the swing animation (by the value's side,
// the grapple track's name or a table id), sends it to the owner's animation component and keeps the value's
// sign. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1450.h"
#include "types.h"

using base::global::CStrId;

// Declared as src/game/CSamusGrappleMovementState/CSamusGrappleMovementState_SetParams_b65a6.cpp defines it.
struct UnkGrappleParams_b65a6 {
    u32 words[5];
};

struct UnkGrappleState_b65a6 {
    u8 pad_0x0[0xb74];
    UnkGrappleParams_b65a6 params;
};

void fn_710099c920(UnkGrappleState_b65a6* self, const UnkGrappleParams_b65a6* params);

// The grapple state's parameters, as floats.
struct UnkGrappleParams_l984e54 {
    f32 field_0x0;
    f32 friction;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
};

struct UnkGrappleStateParams_l984e54 {
    u8 pad_0x0[0xb74];
    UnkGrappleParams_l984e54 params;
};

// CSamusGrappleMovementState::CTunableSamusGrappleMovementState, the tunable g_unk_717273ba88 is.
struct UnkGrappleTunable_l984e54 {
    u8 pad_0x0[0x6c];
    f32 fSwingLongPerpendicularFrictionInImpulseBraking;
    u8 pad_0x70[0x80 - 0x70];
    bool bSwingPlayAnimations;
};
extern base::reflection::UnkTunable g_unk_717273ba88;

static inline const UnkGrappleTunable_l984e54& GrappleTunable() {
    return reinterpret_cast<const UnkGrappleTunable_l984e54&>(g_unk_717273ba88);
}

struct UnkTrackRecord_l984e54 {
    u8 pad_0x0[0x20];
    CStrId name;
};

struct UnkSwingInfo_l984e54 {
    u8 pad_0x0[0x3c];
    f32 side;
    u8 pad_0x40[0x49 - 0x40];
    bool field_0x49;
};

struct UnkOwner_l984e54 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkGrappleLink_l984e54 {
    u8 pad_0x0[0x8];
    UnkGrappleStateParams_l984e54* state;
};

struct Unk_l984e54 {
    u8 pad_0x0[0x8];
    UnkGrappleLink_l984e54* link;
    u8 pad_0x10[0x3c4 - 0x10];
    f32 value;
    s32 kind;
    f32 field_0x3cc;
    f32 field_0x3d0;
    u8 pad_0x3d4[0x3e4 - 0x3d4];
    f32 sign;
};
static_assert(__builtin_offsetof(Unk_l984e54, sign) == 0x3e4, "layout");

static inline u64 TableKey(u32 offset) {
    return reinterpret_cast<const CStrId*>(reinterpret_cast<const char*>(&GameManagerIdTable()) + offset)->data->Key();
}

void fn_7100984e54(Unk_l984e54* self, UnkSwingInfo_l984e54* info, f32 value, s32 kind) {
    self->value = value;
    self->kind = kind;
    self->field_0x3cc = 0.0f;
    self->field_0x3d0 = 0.0f;
    switch (kind) {
    case 1:
        base::reflection::UnkReflTypeIdArg track_mode;
        base::reflection::UnkReflTypeIdArg track_mask;
        base::reflection::UnkReflTypeIdArg grapple_mode;
        base::reflection::UnkReflTypeIdArg grapple_mask;
        base::reflection::UnkReflTypeIdArg side_mode;
        base::reflection::UnkReflTypeIdArg side_mask;
        base::reflection::UnkReflTypeIdArg default_mode;
        base::reflection::UnkReflTypeIdArg default_mask;
        UnkOwner_l984e54* owner;
        owner = reinterpret_cast<UnkOwner_l984e54*>(fn_710097f1b8(reinterpret_cast<UnkUnit1452_b56*>(self)));
        if (!GrappleTunable().bSwingPlayAnimations)
            break;
        UnkAnimationComponent* animation;
        animation = owner->animation;
        if (!(info->side * value < 0.0f)) {
            UnkTrackQuery query = {fn_7100d001f8(), nullptr, -1.0f};
            fn_710117b08c(animation, &query);
            UnkTrackRecord_l984e54* record = reinterpret_cast<UnkTrackRecord_l984e54*>(query.result);
            if (record && record->name.data != &g_unk_7101d47108->empty) {
                track_mode.id = 1;
                track_mask.id = UnkAll_SpitSend().value;
                fn_7101176690(animation, record->name.data->Key(), 0xf4061ff25a2163d6, &track_mode, &g_unk_710164b958,
                              &g_unk_7172769740, &track_mask);
            } else {
                grapple_mode.id = 1;
                grapple_mask.id = UnkAll_SpitSend().value;
                fn_7101176690(animation, TableKey(0x1278), 0xf4061ff25a2163d6, &grapple_mode, &g_unk_710164b958,
                              &g_unk_7172769740, &grapple_mask);
            }
        } else if (info->field_0x49) {
            side_mode.id = 1;
            side_mask.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, TableKey(0x12a8), 0xf4061ff25a2163d6, &side_mode, &g_unk_710164b958,
                          &g_unk_7172769740, &side_mask);
        } else {
            default_mode.id = 1;
            default_mask.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, 0x5c3ed5663c71384c, 0xf4061ff25a2163d6, &default_mode, &g_unk_710164b958,
                          &g_unk_7172769740, &default_mask);
        }
        self->field_0x3d0 = 0.0f;
        f32 current;
        current = self->value;
        self->sign = (current > 0.0f) - (current < 0.0f);
        break;
    case 2: {
        UnkGrappleStateParams_l984e54* state = self->link ? self->link->state : nullptr;
        UnkGrappleParams_l984e54 params = state->params;
        params.friction = GrappleTunable().fSwingLongPerpendicularFrictionInImpulseBraking;
        fn_710099c920(reinterpret_cast<UnkGrappleState_b65a6*>(state),
                      reinterpret_cast<const UnkGrappleParams_b65a6*>(&params));
        break;
    }
    }
}
