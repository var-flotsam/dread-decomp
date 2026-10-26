// Updates the flag byte at 0x1a of `other`: a clear flag stays clear, and it is cleared while the
// owner's byte at 0x14b54 is set. Otherwise it becomes the inverse of a flag in the section that the
// blackboard at +0x968 of the global manager names by the string table's id at 0x1150 (the table is a
// function-local static); the named id at 0x717275ac18+0x10 is set to the default id on first use.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkOwner {
    u8 pad_0x0[0x14b54];
    u8 field_0x14b54;
};

struct UnkOther {
    u8 pad_0x0[0x1a];
    u8 field_0x1a;
};

// A blackboard property definition (game/UnkGameBlackboard.h): its qualified id is built on
// first use.
struct UnkNamedId : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedId qid;
};

extern UnkNamedId g_unk_7172759178;

void fn_71003e7bf8(UnkOwner* self, UnkOther* other) {
    u32 value = other->field_0x1a;
    if (value) {
        if (self->field_0x14b54) {
            value = 0;
        } else {
            bool flag = false;
            CBlackboard* blackboard = reinterpret_cast<CBlackboard*>(g_unk_7101d47310->field_0x968);
            const CStrId& id = base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x22a];
            if (g_unk_7172759178.qid.id.data == &g_unk_7101d47108->empty)
                fn_7100ae9a10(&g_unk_7172759178.qid, &g_unk_7172759178, g_unk_7101d470e0);
            if (CBlackboard::CSection* target = fn_7101047270(blackboard, id))
                fn_710006d11c(target, &g_unk_7172759178.qid.id, &flag);
            value = flag ^ 1;
        }
    }
    other->field_0x1a = value;
}
