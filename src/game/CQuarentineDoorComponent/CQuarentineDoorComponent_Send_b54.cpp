// CQuarentineDoorComponent's unit (tools/tu_map.py): when the owner (+0x30) has its sub-object at
// +0x220, sends it the hash of "opened" with mode 2 and, in place of UnkSpitSend's first
// descriptor (src/game_s2/UnkSpitSend.h), a local block {-1, 1.0f, 1}; then fn_71007df12c(1).
// fn_71007df384 (same unit) does nothing while the flag at +0x7a or +0x2a0 is set; it sends a
// fixed hash (as UnkSpitSend), gives the owner's weak target (fn_71002b3b84) the hash of "opened"
// (fn_71009c0f1c), then sets the flag at +0x2a1. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CQuarentineDoorComponent.h"
#include "game/UnkTU372.h"
#include "types.h"

struct UnkQuarentineDoor_b54;

struct UnkWeakTarget;

struct UnkQuarentineOwner_b54 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkQuarentineDoor_b54 {
    u8 pad_0x0[0x30];
    union {
        UnkQuarentineOwner_b54* owner;
        UnkWeakTarget* weak_owner;
    };
    u8 pad_0x38[0x42];
    bool field_0x7a;
    u8 pad_0x7b[0x2a0 - 0x7b];
    bool field_0x2a0;
    bool field_0x2a1;
};

struct UnkSendParams_b54 {
    s64 field_0x0;
    f32 field_0x8;
    u32 field_0xc;
};

static inline void SendParams_b54(void* target, u64 hash, u64 kind, u32 m) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = m;
    UnkSendParams_b54 params;
    params.field_0x0 = -1;
    params.field_0x8 = 1.0f;
    params.field_0xc = 1;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, kind, &mode, &params, &g_unk_7172769740, &mask);
}

void fn_71007defe0(UnkQuarentineDoor_b54* self) {
    if (self->owner)
        if (void* target = self->owner->field_0x220)
            SendParams_b54(target, base::global::CName("opened").hash, 0xf4061ff25a2163d6, 2);
    fn_71007df12c(self, true);
}

void fn_71007df384(UnkQuarentineDoor_b54* self) {
    if (self->field_0x7a || self->field_0x2a0)
        return;
    if (self->owner)
        if (void* target = self->owner->field_0x220)
            UnkSpitSend(target, 0xa0e33be014f44bd7, 0xf4061ff25a2163d6);
    if (void* target = fn_71002b3b84(self->weak_owner))
        fn_71009c0f1c(target, base::global::CName("opened").hash);
    self->field_0x2a1 = true;
}
