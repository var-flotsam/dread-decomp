// Three sends in the CGoliathAttack unit to the owner's sub-object (+0x220 of the owner at +0x8),
// descriptors as UnkSpitSend (src/game_s2/UnkSpitSend.h). fn_710058e94c sends the hash of the
// sub-object's id at +0x2b8 with mode 1 and clears the state at +0x28; the other two try a fixed
// request with mode 0 first and fall back to it. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkGoliathTarget_b56a3 {
    u8 pad_0x0[0x2b8];
    CStrId field_0x2b8;
};

struct UnkGoliathOwner_b56a3 {
    u8 pad_0x0[0x220];
    UnkGoliathTarget_b56a3* field_0x220;
};

struct UnkGoliathData_b56a3 {
    u8 pad_0x0[0xec];
    u32 field_0xec;
};

struct UnkGoliathAttack_b56a3 {
    u8 pad_0x0[0x8];
    UnkGoliathOwner_b56a3* owner;
    u8 pad_0x10[0x18];
    u32 field_0x28;
    u8 pad_0x2c[4];
    UnkGoliathData_b56a3* field_0x30;
    u32 field_0x38;
};

void fn_710058e94c(UnkGoliathAttack_b56a3* self) {
    UnkGoliathTarget_b56a3* target = self->owner->field_0x220;
    u64 hash = *reinterpret_cast<u64*>(reinterpret_cast<char*>(target->field_0x2b8.data) + 0x30);
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 1;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
    self->field_0x28 = 0;
}

static inline __attribute__((always_inline)) bool SendMode0_b56a3(UnkGoliathAttack_b56a3* self, u64 hash) {
    UnkGoliathTarget_b56a3* target = self->owner->field_0x220;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    return fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

// Sends 0x64e7414e91ab8a4 with mode 0; state 3 when it was taken, else the send above.
void fn_710058ea6c(UnkGoliathAttack_b56a3* self) {
    if (SendMode0_b56a3(self, 0x64e7414e91ab8a4)) {
        self->field_0x28 = 3;
        return;
    }
    fn_710058e94c(self);
}

// Sends 0x8d045f916fcc75ad with mode 0; when it was taken, copies the value at +0xec of the object
// at +0x30 to +0x38 and enters state 2, else the send above.
void fn_710058ebb0(UnkGoliathAttack_b56a3* self) {
    if (SendMode0_b56a3(self, 0x8d045f916fcc75ad)) {
        self->field_0x38 = self->field_0x30->field_0xec;
        self->field_0x28 = 2;
        return;
    }
    fn_710058e94c(self);
}
