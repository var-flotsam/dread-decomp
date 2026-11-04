// CAttack's unit (tools/tu_map.py): when the attack has its preset (+0x20) and the preset's
// vf_0x48 agrees (and, when the owner (+0x8) has the object at +0x1b8, its state at +0x70 isn't 2),
// takes the preset's string id (vf_0x50), or the one at +0x2b8 of the owner's sub-object at +0x220
// when that is empty, and sends its hash to that sub-object with mode 2 (descriptors as UnkSpitSend,
// src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkAttackState_b55 {
    u8 pad_0x0[0x70];
    s32 field_0x70;
};

struct UnkAttackTarget_b55 {
    u8 pad_0x0[0x2b8];
    CStrId field_0x2b8;
};

struct UnkAttackOwner_b55 {
    u8 pad_0x0[0x1b8];
    UnkAttackState_b55* field_0x1b8;
    u8 pad_0x1c0[0x60];
    UnkAttackTarget_b55* field_0x220;
};

struct UnkAttackPreset_b55 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual bool vf_0x48();
    virtual const CStrId* vf_0x50();
};

struct UnkAttack_b55 {
    u8 pad_0x0[0x8];
    UnkAttackOwner_b55* owner;
    u8 pad_0x10[0x10];
    UnkAttackPreset_b55* preset;
};

static inline void SendMode_b55(void* target, u64 hash, u32 m) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = m;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

void fn_7100362a50(UnkAttack_b55* self) {
    if (!self->preset)
        return;
    UnkAttackState_b55* state = self->owner->field_0x1b8;
    UnkAttackTarget_b55* target = self->owner->field_0x220;
    bool ok = self->preset->vf_0x48();
    if (state && ok)
        ok = state->field_0x70 != 2;
    if (!target || !ok)
        return;
    const CStrId* src = self->preset->vf_0x50();
    CStrId id(*src);
    if (id.data == &g_unk_7101d47108->empty)
        id = target->field_0x2b8;
    if (id.data != &g_unk_7101d47108->empty)
        SendMode_b55(target, *reinterpret_cast<u64*>(reinterpret_cast<char*>(id.data) + 0x30), 2);
}
