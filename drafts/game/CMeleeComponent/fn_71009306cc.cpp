// CMeleeComponent's unit: plays the melee fail animation on the owner's character class (filled in by its
// CCharClassMeleeFailActionTrack), stops the owner's horizontal motion while a track ignores air movement,
// then tells the other actor's AI. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/CMovementComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

bool fn_71009e12d0(void*);

struct UnkMeleeFailDesc_l9306cc {
    u64 words[2];
};

struct UnkMeleeFailBlend_l9306cc {
    u64 kind;
    f32 value;
};

struct UnkMeleeFailRequest_l9306cc {
    s64 id;
    u64 desc[2];                     // 0x8
    u64 blend_kind;                  // 0x18
    f32 blend_value;                 // 0x20
    u32 flags;                       // 0x24
    u32 mask;                        // 0x28
};

struct UnkMeleeFailKey_l9306cc {
    u8 pad_0x0[0x30];
    s64 id;  // 0x30
};

// The CCharClassMeleeFailActionTrack record.
struct UnkMeleeFailTrack_l9306cc {
    u8 pad_0x0[0x18];
    UnkMeleeFailKey_l9306cc* key;  // 0x18
    bool use_blend;                // 0x20
    u8 pad_0x21[3];
    s32 blend_frames;  // 0x24
};

// The CCharClassIgnoreAirMovementTrack record.
struct UnkMeleeAirTrack_l9306cc {
    u8 pad_0x0[0x18];
    bool stop;  // 0x18
};

struct UnkMeleeFailVec_l9306cc {
    f32 x, y, z;
};

struct UnkMeleeFailMovement_l9306cc {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
    u8 pad_0xfa[0x1dc - 0xfa];
    f32 field_0x1dc;
    f32 field_0x1e0;
};

struct UnkMeleeFailActor_l9306cc {
    u8 pad_0x0[0x1f0];
    UnkMeleeFailMovement_l9306cc* movement;  // 0x1f0
    u8 pad_0x1f8[0x208 - 0x1f8];
    void* field_0x208;
    u8 pad_0x210[0x220 - 0x210];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkMeleeFailAI_l9306cc {
    void** vtable;

    void vf_0x390(UnkMeleeFailActor_l9306cc* actor, u64 arg) {
        reinterpret_cast<void (*)(UnkMeleeFailAI_l9306cc*, UnkMeleeFailActor_l9306cc*, u64)>(vtable[0x390 / 8])(
            this, actor, arg);
    }
};

struct UnkMeleeFailOther_l9306cc {
    u8 pad_0x0[0x240];
    UnkMeleeFailAI_l9306cc* ai;  // 0x240
};

struct UnkMeleeFailComponent_l9306cc {
    u8 pad_0x0[0x30];
    UnkMeleeFailActor_l9306cc* owner;  // 0x30
};

bool fn_71009306cc(UnkMeleeFailComponent_l9306cc* self, UnkMeleeFailOther_l9306cc* other, u64 arg) {
    UnkAnimationComponent* animation = self->owner->animation;
    if (animation) {
        UnkMeleeFailRequest_l9306cc request;
        const UnkMeleeFailDesc_l9306cc* desc = reinterpret_cast<const UnkMeleeFailDesc_l9306cc*>(&g_unk_710164b958);
        u64 desc0 = desc->words[0];
        u64 desc1 = desc->words[1];
        const UnkMeleeFailBlend_l9306cc* blend = reinterpret_cast<const UnkMeleeFailBlend_l9306cc*>(&g_unk_7172769740);
        u64 kind = blend->kind;
        f32 value = blend->value;
        u32 mask = UnkAll_SpitSend().value;
        request.id = -1;
        request.desc[0] = desc0;
        request.desc[1] = desc1;
        request.blend_kind = kind;
        request.blend_value = value;
        request.flags = 0;
        request.mask = mask;
        request.id = 0xe1bfa21bbed4f4b6;

        UnkMeleeFailActor_l9306cc* owner = self->owner;
        if (owner->movement && owner->movement->field_0xf9)
            request.id = 0x2c0f6c114ab015dc;
        if (owner->field_0x208 && fn_71009e12d0(owner->field_0x208)) {
            UnkTrackQuery query = {fn_7100c3e258(), nullptr, -1.0f};
            fn_710117b08c(animation, &query);
            UnkMeleeFailTrack_l9306cc* track = reinterpret_cast<UnkMeleeFailTrack_l9306cc*>(query.result);
            request.id = track->key->id;
            request.blend_value = track->blend_frames;
            request.blend_kind = 0;
            if (track->use_blend) {
                const UnkMeleeFailBlend_l9306cc* fixed =
                    reinterpret_cast<const UnkMeleeFailBlend_l9306cc*>(&g_unk_7172769758);
                request.blend_value = fixed->value;
                request.blend_kind = fixed->kind;
            }
        }
        request.flags |= 1;
        fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(animation),
                      reinterpret_cast<const UnkRequest_7101176ea8*>(&request), 0xf4061ff25a2163d6);

        UnkTrackQuery query = {fn_7100d04b78(), nullptr, -1.0f};
        if (fn_710117b08c(animation, &query) && reinterpret_cast<UnkMeleeAirTrack_l9306cc*>(query.result)->stop) {
            UnkMeleeFailMovement_l9306cc* movement = self->owner->movement;
            UnkMeleeFailVec_l9306cc velocity = {0.0f, movement->field_0x1dc, movement->field_0x1e0};
            fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(movement),
                          reinterpret_cast<const UnkMovementVec_71010d08d4*>(&velocity), true);
        }
    }
    if (other->ai)
        other->ai->vf_0x390(self->owner, arg);
    return false;
}
