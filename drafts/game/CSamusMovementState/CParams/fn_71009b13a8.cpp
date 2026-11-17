// CSamusMovementState::CParams's unit (tools/tu_map.py): starts a movement state's turn. When the owner's
// animation has a tweak-turn track, the track fills a transition that is played; otherwise the state's own
// action is sent with mode 2 (or 0). Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "types.h"

struct UnkTurnDesc_l9b13a8 {
    u64 words[2];
};

struct UnkTurnBlend_l9b13a8 {
    u64 kind;
    u32 value;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h, flags and a mask.
struct UnkTurnRequest_l9b13a8 {
    s64 id;
    u64 desc[2];      // 0x8
    u64 blend_kind;   // 0x18
    u32 blend_value;  // 0x20
    u32 flags;        // 0x24
    u32 mask;         // 0x28
};

// The tweak-turn track that fills the transition in.
struct UnkTurnTrack_l9b13a8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkTurnRequest_l9b13a8* request);

    u8 pad_0x8[0x2c];
    bool field_0x34;
};
static_assert(__builtin_offsetof(UnkTurnTrack_l9b13a8, field_0x34) == 0x34, "layout");

struct UnkTurnMovement_l9b13a8 {
    void** vtable;
    u8 pad_0x8[0x5c];
    u8 field_0x64[4];

    void vf_0x208(void* arg) {
        reinterpret_cast<void (*)(UnkTurnMovement_l9b13a8*, void*)>(vtable[0x208 / 8])(this, arg);
    }
};

struct UnkTurnOwner_l9b13a8 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2a8 - 0x228];
    void* field_0x2a8;  // 0x2a8
};

struct UnkTurnState_l9b13a8 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkTurnOwner_l9b13a8* owner;        // 0x30
    UnkTurnMovement_l9b13a8* movement;  // 0x38
    u8 pad_0x40[0x119];
    bool field_0x159;  // 0x159

    u64 vf_0x148() { return reinterpret_cast<u64 (*)(UnkTurnState_l9b13a8*)>(vtable[0x148 / 8])(this); }
    void vf_0x150(const s64* id) {
        reinterpret_cast<void (*)(UnkTurnState_l9b13a8*, const s64*)>(vtable[0x150 / 8])(this, id);
    }
};
static_assert(__builtin_offsetof(UnkTurnState_l9b13a8, field_0x159) == 0x159, "layout");

void fn_710083a3a0(void* component);

static const u64 kTurnKind = 0xf4061ff25a2163d6ull;

bool fn_71009b13a8(UnkTurnState_l9b13a8* self, bool flag) {
    UnkTrackQuery query = {fn_7100d34868(), nullptr, -1.0f};
    UnkAnimationComponent* animation = self->owner->animation;
    if (animation)
        fn_710117b08c(animation, &query);
    if (animation && query.result) {
        const UnkTurnDesc_l9b13a8* desc = reinterpret_cast<const UnkTurnDesc_l9b13a8*>(&g_unk_710164b958);
        u64 desc0 = desc->words[0];
        u64 desc1 = desc->words[1];
        const UnkTurnBlend_l9b13a8* blend = reinterpret_cast<const UnkTurnBlend_l9b13a8*>(&g_unk_7172769740);
        u64 kind = blend->kind;
        u32 value = blend->value;
        u32 mask = UnkAll_SpitSend().value;
        UnkTurnRequest_l9b13a8 request;
        request.id = -1;
        request.desc[0] = desc0;
        request.desc[1] = desc1;
        request.blend_kind = kind;
        request.blend_value = value;
        request.flags = 0;
        request.mask = mask;
        UnkTurnTrack_l9b13a8* track = reinterpret_cast<UnkTurnTrack_l9b13a8*>(query.result);
        track->vf_0x18(&request);
        if (request.id == -1)
            return false;
        if (flag)
            request.flags |= 1;
        if (!fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(self->owner->animation),
                           reinterpret_cast<const UnkRequest_7101176ea8*>(&request), kTurnKind))
            return false;
        self->vf_0x150(&request.id);
        if (reinterpret_cast<UnkTurnTrack_l9b13a8*>(query.result)->field_0x34)
            self->movement->vf_0x208(self->movement->field_0x64);
    } else {
        base::reflection::UnkReflTypeIdArg mode;
        base::reflection::UnkReflTypeIdArg mask;
        s64 id = self->vf_0x148();
        mode.id = flag ? 2 : 0;
        UnkAnimationComponent* target = self->owner->animation;
        mask.id = UnkAll_SpitSend().value;
        if (!fn_7101176690(target, id, kTurnKind, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask))
            return false;
        self->vf_0x150(&id);
    }
    self->field_0x159 = false;
    if (void* component = self->owner->field_0x2a8)
        fn_710083a3a0(component);
    return true;
}
