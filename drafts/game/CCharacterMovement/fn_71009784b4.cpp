// Unit 1445 (CCharacterMovement): builds the owner's fall-end animation request: the
// CCharClassTweakFallEndActionTrack fills it when one plays, else its id is the AI's or a default one.
// Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "game/UnkGameManagerIdTable.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkFallEndDesc_l9784b4 {
    u64 words[2];
};

struct UnkFallEndBlend_l9784b4 {
    u64 kind;
    u32 value;
};

struct UnkFallEndRequest_l9784b4 {
    s64 id;
    u64 desc[2];     // 0x8
    u64 blend_kind;  // 0x18
    u32 blend_value; // 0x20
    u32 flags;       // 0x24
    u32 mask;        // 0x28
};

// The track that fills the request in.
struct UnkFallEndTrack_l9784b4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkFallEndRequest_l9784b4* request);
};

// Gives the name of the animation to play.
struct UnkFallEndActor_l9784b4;

struct UnkFallEndNamer_l9784b4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual const base::global::CStrId& vf_0x48(UnkFallEndActor_l9784b4* actor);
};

struct UnkFallEndAI_l9784b4 {
    u8 pad_0x0[0x1610];
    UnkFallEndNamer_l9784b4* namer;  // 0x1610
};

struct UnkFallEndActor_l9784b4 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x240 - 0x228];
    UnkFallEndAI_l9784b4* ai;  // 0x240
};

struct UnkCharacterMovement_l9784b4 {
    u8 pad_0x0[0x30];
    UnkFallEndActor_l9784b4* actor;  // 0x30
};

UnkFallEndRequest_l9784b4 fn_71009784b4(UnkCharacterMovement_l9784b4* self) {
    UnkFallEndRequest_l9784b4 request;
    const UnkFallEndDesc_l9784b4* desc = reinterpret_cast<const UnkFallEndDesc_l9784b4*>(&g_unk_710164b958);
    u64 desc0 = desc->words[0];
    u64 desc1 = desc->words[1];
    const UnkFallEndBlend_l9784b4* blend = reinterpret_cast<const UnkFallEndBlend_l9784b4*>(&g_unk_7172769740);
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

    UnkTrackQuery query = {fn_7100d2ffa8(), nullptr, -1.0f};
    if (self->actor->animation) {
        fn_710117b08c(self->actor->animation, &query);
        if (query.result) {
            reinterpret_cast<UnkFallEndTrack_l9784b4*>(query.result)->vf_0x18(&request);
            return request;
        }
    }
    UnkFallEndActor_l9784b4* actor = self->actor;
    if (!actor->ai || !actor->ai->namer) {
        request.id = GameManagerIdTable().field_0xfe8.data->Key();
        return request;
    }
    request.id = actor->ai->namer->vf_0x48(actor).data->Key();
    return request;
}
