// The CEmmyAIComponent unit: fn_71005436e8, a copy of the string-id test in
// CEmmyAIComponent_IsStateId_b56.cpp (fn_710054aecc) against the id g_unk_71726debd0. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU811.h"
#include "game/behaviortree/CEmmyChaseAction.h"
#include "types.h"

using base::global::CStrId;

bool fn_710051d464(CBlackboard::CSection* section, const void* name, CStrId* out);

struct UnkEmmyAIComponent_b46a1;

struct UnkEmmyStateIdTest_b63a3 {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;
};

namespace {

struct UnkEmptyStrId_b63a3 {
    union {
        CStrId id;
    };

    UnkEmptyStrId_b63a3() {
        id.data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        id.data = &g_unk_7101d47108->empty;
    }
    ~UnkEmptyStrId_b63a3() { id.~CStrId(); }
};

}  // namespace

bool fn_71005436e8(UnkEmmyAIComponent_b46a1* owner) {
    UnkEmmyStateIdTest_b63a3* self = reinterpret_cast<UnkEmmyStateIdTest_b63a3*>(owner);
    const CStrId& want = *reinterpret_cast<const CStrId*>(g_unk_71726debd0);
    UnkEmptyStrId_b63a3 value;
    CBlackboard* blackboard = &self->blackboard;
    asm("" : "+r"(blackboard));
    const CStrId& section = *reinterpret_cast<const CStrId*>(reinterpret_cast<char*>(&GameManagerIdTable()) + 0x1b58);
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        fn_710051d464(s, g_unk_71726e20a8, &value.id);
    return value.id.data == want.data;
}
