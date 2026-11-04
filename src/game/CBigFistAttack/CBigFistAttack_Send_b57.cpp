// CBigFistAttack's unit (tools/tu_map.py): sends the owner's (+0x8) sub-object at +0x220 the value 1 under
// the hash at +0x30 of that sub-object's +0x2b8 (refl_EmitU32, base/reflection/UnkReflStatics.h), then
// clears the word at +0x30. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkBigFistHashed_b57 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkBigFistTarget_b57 {
    u8 pad_0x0[0x2b8];
    UnkBigFistHashed_b57* field_0x2b8;
};

struct UnkBigFistSendOwner_b57 {
    u8 pad_0x0[0x220];
    UnkBigFistTarget_b57* field_0x220;
};

struct UnkBigFistSend_b57 {
    u8 pad_0x0[0x8];
    UnkBigFistSendOwner_b57* owner;
    u8 pad_0x10[0x20];
    u32 field_0x30;
};

void fn_71003e0d50(UnkBigFistSend_b57* self) {
    UnkBigFistTarget_b57* target = self->owner->field_0x220;
    base::reflection::refl_EmitU32(target, target->field_0x2b8->hash, 0xf4061ff25a2163d6, 1, &g_unk_710164b958,
                                   &g_unk_7172769740);
    self->field_0x30 = 0;
}

struct UnkBigFistTunable_b57 {
    u8 pad_0x0[0x2c];
    u32 field_0x2c;
};
// The BigFist tunable (registered in src/reflection/CCharClassBigFistAttack.cpp).
struct UnkTunable;
extern UnkTunable g_unk_71726c06a8;

struct UnkBigFistState_b57 {
    u8 pad_0x0[0x8];
    UnkBigFistSendOwner_b57* owner;
    u8 pad_0x10[0x20];
    u32 field_0x30;
    u32 field_0x34;
};

// Sends a zero under a fixed hash; on success sets the state to 3, otherwise falls back to fn_71003e0d50.
void fn_71003e0e70(UnkBigFistState_b57* self) {
    if (base::reflection::refl_EmitU32(self->owner->field_0x220, 0x064e7414e91ab8a4, 0xf4061ff25a2163d6, 0,
                                       &g_unk_710164b958, &g_unk_7172769740)) {
        self->field_0x30 = 3;
    } else {
        fn_71003e0d50(reinterpret_cast<UnkBigFistSend_b57*>(self));
    }
}

// The same with another hash; on success sets the state to 2 and copies a tunable word.
void fn_71003e1134(UnkBigFistState_b57* self) {
    if (base::reflection::refl_EmitU32(self->owner->field_0x220, 0x8d045f916fcc75ad, 0xf4061ff25a2163d6, 0,
                                       &g_unk_710164b958, &g_unk_7172769740)) {
        u32 value = reinterpret_cast<UnkBigFistTunable_b57*>(&g_unk_71726c06a8)->field_0x2c;
        self->field_0x30 = 2;
        self->field_0x34 = value;
    } else {
        fn_71003e0d50(reinterpret_cast<UnkBigFistSend_b57*>(self));
    }
}
