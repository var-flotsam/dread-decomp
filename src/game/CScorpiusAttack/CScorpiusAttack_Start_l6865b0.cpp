// CScorpiusAttack unit: after the shared attack update, when the attack's config enables it, sends the
// attack's id under kind 0x722b01c64fe0ffe0 to the owner's character class (+0x220) with the config's
// value; the id followed by "_taillayer" when the class knows that name. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAttack.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkStruct_7101176ea8;

struct UnkScorpiusAttackConfig_l6865b0 {
    u8 pad_0x0[0xec];
    bool field_0xec;
    u8 pad_0xed[0x3];
    f32 field_0xf0;
};

struct UnkScorpiusAttack_l6865b0 : UnkAttackBase {
    u8 pad_0x20[0x8];
    UnkScorpiusAttackConfig_l6865b0* config;

    const CStrId* vf_0xb0() {
        return reinterpret_cast<const CStrId* (*)(UnkScorpiusAttack_l6865b0*)>(vtable[0xb0 / 8])(this);
    }
};

// A typed value: a 4-byte payload and its kind (1: float).
struct UnkScorpiusValue_l6865b0 {
    f32 value;
    u32 kind;
};

struct UnkScorpiusArg_l6865b0 {
    s64 field_0x0;
    UnkScorpiusValue_l6865b0 value;

    UnkScorpiusArg_l6865b0(UnkScorpiusValue_l6865b0 v) : field_0x0(-1), value(v) {}
};

static inline UnkScorpiusValue_l6865b0 MakeFloat_l6865b0(f32 value) {
    UnkScorpiusValue_l6865b0 v;
    v.value = value;
    v.kind = 1;
    return v;
}

static const u64 kScorpiusKind = 0x722b01c64fe0ffe0;

// Declared with this parameter by its caller, src/game/CScorpiusDefensiveSpikeBallPrickAttack/
// CScorpiusDefensiveSpikeBallPrickAttack_Start_b67a4.cpp (a subclass's view of the attack).
struct UnkPrickAttack_b67a4;

void fn_71006865b0(UnkPrickAttack_b67a4* attack) {
    auto* self = reinterpret_cast<UnkScorpiusAttack_l6865b0*>(attack);
    fn_71003608ac(self);
    if (!self->config->field_0xec)
        return;
    const CStrId* id = self->vf_0xb0();
    if (id->data == &g_unk_7101d47108->empty)
        return;
    u64 tail;
    {
        ++id->data->reads;
        CRntString name = CRntString_ConcatCString(&id->data->String(), "_taillayer");
        fn_7100066630(&tail, &name);
    }
    // Declared past a goto so that they get no lifetime markers: each send keeps its own slots.
    goto send;
    base::reflection::UnkReflTypeIdArg modeTail;
    base::reflection::UnkReflTypeIdArg maskTail;
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
send:
    bool hasTail = fn_7101176e78(static_cast<UnkStruct_7101176ea8*>(self->field_0x8->field_0x220),
                                 reinterpret_cast<void*>(tail), nullptr);
    void* target = self->field_0x8->field_0x220;
    if (hasTail) {
        u64 hash = tail;
        modeTail.id = 0;
        UnkScorpiusArg_l6865b0 arg(MakeFloat_l6865b0(self->config->field_0xf0));
        maskTail.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(target, hash, kScorpiusKind, &modeTail, &arg, &g_unk_7172769740, &maskTail);
    } else {
        u64 hash = id->data->Key();
        mode.id = 0;
        UnkScorpiusArg_l6865b0 arg(MakeFloat_l6865b0(self->config->field_0xf0));
        mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(target, hash, kScorpiusKind, &mode, &arg, &g_unk_7172769740, &mask);
    }
}
