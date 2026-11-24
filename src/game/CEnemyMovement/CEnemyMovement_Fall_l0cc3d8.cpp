// CEnemyMovement unit: a step of the movement: settles a pending virtual call and the state's own step, then,
// while active and falling is allowed, plays the owner's tweak-fall animation request (the
// CCharClassTweakFallActionTrack fills it) unless a gravity-ignoring or flying track plays.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkGameManagerIdTable.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkFallDesc_l0cc3d8 {
    u64 words[2];
};

struct UnkFallBlend_l0cc3d8 {
    u64 kind;
    u32 value;
};

struct UnkFallRequest_l0cc3d8 {
    s64 id;
    u64 desc[2];     // 0x8
    u64 blend_kind;  // 0x18
    u32 blend_value; // 0x20
    u32 flags;       // 0x24
    u32 mask;        // 0x28
};

// The track that fills the request in.
struct UnkFallTrack_l0cc3d8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkFallRequest_l0cc3d8* request);
};

struct UnkFallAI_l0cc3d8 {
    void** vtable;

    void vf_0x4f8() { reinterpret_cast<void (*)(UnkFallAI_l0cc3d8*)>(vtable[0x4f8 / 8])(this); }
    void vf_0x500() { reinterpret_cast<void (*)(UnkFallAI_l0cc3d8*)>(vtable[0x500 / 8])(this); }
};

struct UnkFallMode_l0cc3d8 {
    u8 pad_0x0[0x70];
    s32 kind;  // 0x70
};

struct UnkFallActor_l0cc3d8 {
    u8 pad_0x0[0x1b8];
    UnkFallMode_l0cc3d8* mode;  // 0x1b8
    u8 pad_0x1c0[0x220 - 0x1c0];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x240 - 0x228];
    UnkFallAI_l0cc3d8* ai;  // 0x240
};

struct UnkFallState_l0cc3d8 {
    void** vtable;

    void vf_0x88() { reinterpret_cast<void (*)(UnkFallState_l0cc3d8*)>(vtable[0x88 / 8])(this); }
};

struct UnkFallOwner_l0cc3d8 {
    u8 pad_0x0[0x69];
    bool can_fall;  // 0x69
};

struct UnkEnemyMovement_l0cc3d8 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkFallActor_l0cc3d8* actor;  // 0x30
    u8 pad_0x38[0xf9 - 0x38];
    bool active;  // 0xf9
    u8 pad_0xfa[0x3a8 - 0xfa];
    bool pending;  // 0x3a8
    u8 pad_0x3a9[0x848 - 0x3a9];
    UnkFallState_l0cc3d8* state;  // 0x848
    u8 pad_0x850[0x19f9 - 0x850];
    bool fall_enabled;  // 0x19f9
    u8 pad_0x19fa[0x1c18 - 0x19fa];
    UnkFallOwner_l0cc3d8* owner;  // 0x1c18

    void vf_0x2b8() { reinterpret_cast<void (*)(UnkEnemyMovement_l0cc3d8*)>(vtable[0x2b8 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkEnemyMovement_l0cc3d8, owner) == 0x1c18, "layout");

void fn_71010cc3d8(UnkEnemyMovement_l0cc3d8* self) {
    if (self->active && self->pending) {
        self->pending = false;
        self->vf_0x2b8();
    }
    if (self->state)
        self->state->vf_0x88();
    UnkFallActor_l0cc3d8* actor = self->actor;
    UnkFallAI_l0cc3d8* ai = actor->ai;
    if (!self->active) {
        if (ai)
            ai->vf_0x4f8();
        return;
    }
    if (!self->owner->can_fall || !self->fall_enabled)
        return;
    if (actor->mode && actor->mode->kind == 2)
        return;
    UnkAnimationComponent* animation = actor->animation;
    if (!animation)
        return;
    if (fn_710117b348(animation, fn_7100d0c7a8()))
        return;
    if (fn_710117b348(animation, fn_7100cf96a8()))
        return;

    UnkFallRequest_l0cc3d8 request;
    const UnkFallDesc_l0cc3d8* desc = reinterpret_cast<const UnkFallDesc_l0cc3d8*>(&g_unk_710164b958);
    u64 desc0 = desc->words[0];
    u64 desc1 = desc->words[1];
    const UnkFallBlend_l0cc3d8* blend = reinterpret_cast<const UnkFallBlend_l0cc3d8*>(&g_unk_7172769740);
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
    request.id = GameManagerIdTable().field_0xfd0.data->Key();
    request.flags |= 1;

    UnkTrackQuery query = {fn_7100d2fa08(), nullptr, -1.0f};
    if (self->actor->animation) {
        fn_710117b08c(self->actor->animation, &query);
        if (query.result)
            reinterpret_cast<UnkFallTrack_l0cc3d8*>(query.result)->vf_0x18(&request);
    }
    if (request.id == -1)
        return;
    if (ai)
        ai->vf_0x500();
    CAnimationComponent_SendActionRequestRecordToTrack(reinterpret_cast<UnkStruct_7101176ea8*>(animation),
                  reinterpret_cast<const UnkRequest_7101176ea8*>(&request), 0xf4061ff25a2163d6);
}
