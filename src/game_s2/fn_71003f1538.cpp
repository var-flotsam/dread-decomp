// CBigkranXSpitAttack: picks the id of the attack animation from the state its owner is in: one of
// the two ids set up in the unit initialiser (fn_71003f4220), or the global one when the owner
// isn't ready.
#include "base/global/CStrId.h"

using base::global::UnkStrIdBase;

extern UnkStrIdBase g_unk_71726c2888[2] __attribute__((visibility("hidden")));

struct UnkSpitOwnerState {
    u8 pad_0x0[0x6f8];
    s32 field_0x6f8;
};

struct UnkSpitOwnerTarget {
    u8 pad_0x0[0x14518];
    s32 field_0x14518;
};

struct UnkSpitOwner {
    u8 pad_0x0[0x218];
    UnkSpitOwnerState* field_0x218;
    u8 pad_0x220[0x20];
    UnkSpitOwnerTarget* field_0x240;
};

struct UnkSpitAttackIds {
    void* vtable;
    UnkSpitOwner* field_0x8;
};

const UnkStrIdBase* fn_71003f1538(UnkSpitAttackIds* self) {
    UnkSpitOwner* owner = self->field_0x8;
    if (owner) {
        UnkSpitOwnerState* state = owner->field_0x218;
        UnkSpitOwnerTarget* target = owner->field_0x240;
        if (state && target) {
            const UnkStrIdBase* ids = g_unk_71726c2888;
            if (state->field_0x6f8 == 6 || state->field_0x6f8 == 1)
                return target->field_0x14518 == 1 ? &ids[0] : &ids[1];
            return ids;
        }
    }
    return &g_unk_7101d470e0;
}
