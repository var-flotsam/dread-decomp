// CChozoRobotSoldierUppercutAttack's unit (tools/tu_map.py): sends the owner's (+0x8) sub-object at +0x220
// the value 1 under a fixed hash, with a local parameter block (a null pointer and 12.0) as the second
// descriptor (refl_EmitU32 written out, base/reflection/UnkReflStatics.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkCrsSendOwner_b57 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkCrsSend_b57 {
    u8 pad_0x0[0x8];
    UnkCrsSendOwner_b57* owner;
};

struct UnkCrsParams_b57 {
    void* ptr;
    f32 value;
};

void fn_710046ad08(UnkCrsSend_b57* self) {
    void* target = self->owner->field_0x220;
    if (target) {
        base::reflection::UnkReflTypeIdArg value;
        value.id = 1;
        UnkCrsParams_b57 params;
        params.ptr = nullptr;
        params.value = 12.0f;
        base::reflection::UnkReflTypeIdArg arg;
        arg.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
        fn_7101176690(target, 0x76e5ef8727346470, 0xf4061ff25a2163d6, &value, &g_unk_710164b958, &params, &arg);
    }
}

struct UnkCrsHashed_b57 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkCrsProvider_b57 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual UnkCrsHashed_b57** vf_0x18(UnkCrsSendOwner_b57* owner);
};

struct UnkCrsOnce_b57 {
    u8 pad_0x0[0x8];
    UnkCrsSendOwner_b57* owner;
    u8 pad_0x10[0x18];
    UnkCrsProvider_b57* field_0x28;
    u8 pad_0x30[0x10];
    bool field_0x40;
};

// Once (flag at +0x40): unless the sub-object knows the hash the member at +0x28 gives, sends it a zero
// under a fixed hash and sets the flag.
void fn_710046aa10(UnkCrsOnce_b57* self) {
    if (self->field_0x40) {
        return;
    }
    UnkCrsSendOwner_b57* owner = self->owner;
    UnkAnimationComponent* target = static_cast<UnkAnimationComponent*>(owner->field_0x220);
    if (!target) {
        return;
    }
    if (fn_710117ae38(target, (*self->field_0x28->vf_0x18(owner))->hash, false)) {
        return;
    }
    base::reflection::refl_EmitU32(self->owner->field_0x220, 0xde6f5a5d963ca497, 0xf4061ff25a2163d6, 0,
                                   &g_unk_710164b958, &g_unk_7172769740);
    self->field_0x40 = true;
}
