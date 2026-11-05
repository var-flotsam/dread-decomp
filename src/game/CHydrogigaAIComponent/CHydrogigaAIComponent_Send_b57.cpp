// The CHydrogigaAIComponent unit's event handler: for a kind-1 event whose id hash (+0x8 -> +0x10 ->
// +0x38 -> +0x30) is 0x455ffe40f48e5f6d and whose second id (+0x10 -> +0x30) is 0x2b145cf4efb3d2c6,
// sends 0x252f517d6a8cfb08 under that kind with mode 1 to the owner's sub-object (+0x220 of the owner
// at +0x30; descriptors as UnkSpitSend, src/game_s2/UnkSpitSend.h), then unregisters the link at
// +0x15040 from the target of the weak handle at +0x15058 and drops the handle. Always returns
// true. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkHydrogigaIdNode_b57a3 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkHydrogigaIdHolder_b57a3 {
    u8 pad_0x0[0x38];
    UnkHydrogigaIdNode_b57a3* node;
};

struct UnkHydrogigaIdRef_b57a3 {
    u8 pad_0x0[0x10];
    UnkHydrogigaIdHolder_b57a3* holder;
};

struct UnkHydrogigaEvent_b57a3 {
    s32 kind;
    u8 pad_0x4[4];
    UnkHydrogigaIdRef_b57a3* ref;
    UnkHydrogigaIdNode_b57a3* field_0x10;
};

struct UnkHydrogigaOwner_b57a3 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkHydrogigaAIEv_b57a3 {
    u8 pad_0x0[0x30];
    UnkHydrogigaOwner_b57a3* owner;
    u8 pad_0x38[0x15040 - 0x38];
    u8 field_0x15040[0x18];
    UnkWeakHandle* field_0x15058;
};

bool fn_71005c3ac0(UnkHydrogigaAIEv_b57a3* self, UnkHydrogigaEvent_b57a3* event) {
    if (event->kind != 1 || event->ref->holder->node->hash != 0x455ffe40f48e5f6d ||
        event->field_0x10->hash != 0x2b145cf4efb3d2c6)
        return true;
    void* target = self->owner->field_0x220;
    if (!target)
        return true;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 1;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, 0x252f517d6a8cfb08, 0x2b145cf4efb3d2c6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                  &mask);
    UnkWeakHandle** slot = &self->field_0x15058;
    if (UnkWeakHandle* h = *slot) {
        if (h->target)
            fn_710117b3f0(h->target, self->field_0x15040);
        ResetRef(slot);
    }
    return true;
}
