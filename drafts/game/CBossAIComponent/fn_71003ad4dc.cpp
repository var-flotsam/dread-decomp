// CBossAIComponent's unit (tools/tu_map.py): when the subject at +0x14908 allows it (+0x50e) and vf_0x808
// agrees, sends the owner's (+0x30) sub-object at +0x220 the current action's hash with the given value;
// on success calls vf_0x7f8(index), otherwise remembers the index at +0x14804. Names are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "game/CBossAIComponent.h"
#include "types.h"

struct UnkSubject {
    u8 pad_0x0[0x50c];
    u8 field_0x50c;
    u8 pad_0x50d;
    u8 field_0x50e;
};

struct UnkSelf {
    u8 pad_0x0[0x1140];
    s32 field_0x1140;
    u8 pad_0x1144[0x136c0];
    s32 field_0x14804;
    u8 pad_0x14808[0x100];
    UnkSubject* field_0x14908;
};

struct UnkBossOwner_l3ad4dc {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkBossActionDef_l3ad4dc {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkBossAction_l3ad4dc {
    UnkBossActionDef_l3ad4dc* def;
};

struct UnkBossSendParams_l3ad4dc {
    s64 field_0x0;
    f32 field_0x8;
    u32 field_0xc;
};

struct UnkBossAI_l3ad4dc {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkBossOwner_l3ad4dc* owner;  // 0x30

    UnkBossAction_l3ad4dc* vf_0x7f0() {
        return reinterpret_cast<UnkBossAction_l3ad4dc* (*)(UnkBossAI_l3ad4dc*)>(vtable[0x7f0 / 8])(this);
    }
    void vf_0x7f8(s32 index) {
        reinterpret_cast<void (*)(UnkBossAI_l3ad4dc*, s32)>(vtable[0x7f8 / 8])(this, index);
    }
    bool vf_0x808() { return reinterpret_cast<bool (*)(UnkBossAI_l3ad4dc*)>(vtable[0x808 / 8])(this); }
};

// Sends the request with mode 0 and the given parameter block.
inline bool Send_l3ad4dc(void* target, u64 hash, UnkBossSendParams_l3ad4dc params) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    return fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &params, &g_unk_7172769740, &mask);
}

bool fn_71003ad4dc(UnkSelf* self, f32 value, s32 index) {
    if (!self->field_0x14908->field_0x50e)
        return false;
    UnkBossAI_l3ad4dc* ai = reinterpret_cast<UnkBossAI_l3ad4dc*>(self);
    if (!ai->vf_0x808())
        return false;
    if (!ai->owner)
        return false;
    void* target = ai->owner->field_0x220;
    if (!target)
        return false;
    u64 hash = ai->vf_0x7f0()->def->hash;
    if (Send_l3ad4dc(target, hash, {-1, value, 1})) {
        ai->vf_0x7f8(index);
        return true;
    }
    self->field_0x14804 = index;
    return false;
}
