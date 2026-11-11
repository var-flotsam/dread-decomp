// The CHydrogigaBraidAttack unit's event handler: when the owner's AI component (+0x240 of the owner
// at +0x8) is a CHydrogigaAIComponent (fn_71005ba098), the owner has a sub-object at +0x220 and a
// kind-1 event's id hash (+0x8 -> +0x10 -> +0x38 -> +0x30) is 0x9f0ae64d14118a66, sends a fixed
// request with mode 1 to that sub-object unless the target's helper (+0x208) is busy
// (fn_71009e6f9c) or fn_71009e6434 holds; descriptors as UnkSpitSend (src/game_s2/UnkSpitSend.h).
// Always returns true. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU1501.h"
#include "types.h"

struct UnkHelper;

struct UnkBraidAIComponent_b57a3 {
    void** vtable;
    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkBraidAIComponent_b57a3*)>(vtable[0])(this);
    }
};

struct UnkBraidIdNode_b57a3 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkBraidIdHolder_b57a3 {
    u8 pad_0x0[0x38];
    UnkBraidIdNode_b57a3* node;
};

struct UnkBraidIdRef_b57a3 {
    u8 pad_0x0[0x10];
    UnkBraidIdHolder_b57a3* holder;
};

struct UnkBraidEvent_b57a3 {
    s32 kind;
    u8 pad_0x4[4];
    UnkBraidIdRef_b57a3* ref;
};

struct UnkBraidAttack_b57a3 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
};

bool fn_71005b3e60(UnkBraidAttack_b57a3* self, UnkBraidEvent_b57a3* event) {
    auto* ai = static_cast<UnkBraidAIComponent_b57a3*>(self->owner->field_0x240);
    if (!ai)
        return true;
    if (!fn_710009d50c(ai->vf_0x0(), fn_71005ba098()))
        return true;
    UnkActor* owner = self->owner;
    void* target = owner->field_0x220;
    if (!target || event->kind != 1)
        return true;
    if (event->ref->holder->node->hash != 0x9f0ae64d14118a66)
        return true;
    UnkActor* other = fn_7100d7c990(owner);
    if (other) {
        auto* helper = static_cast<UnkHelper*>(other->field_0x208);
        if (helper && (fn_71009e6f9c(helper) || fn_71009e6434(helper)))
            return true;
    }
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 1;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, 0x4e12b70d37e92f0b, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                  &mask);
    return true;
}
