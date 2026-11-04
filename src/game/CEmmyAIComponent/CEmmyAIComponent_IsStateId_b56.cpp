// Three tests in the CEmmyAIComponent unit, one code with a different id: whether the string id
// the blackboard at +0x1260 holds under the property g_unk_71726e20a8 (in the section the id table
// names at +0x1b58) equals a fixed id (fn_710051d464 reads it; an empty id when the section or the
// property is missing). The read follows src/game/CScenario.cpp's fn_71011a2500. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "types.h"

using base::global::CStrId;

// The id table (fn_7100031894 builds it; the definition of src/game/CScenario.cpp).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x17e0];
    u8 field_0x17e0[8];
    u8 pad_0x17e8[0x2578 - 0x17e8];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

bool fn_710051d464(CBlackboard::CSection* section, const void* name, CStrId* out);

extern char g_unk_71726e20a8[] __attribute__((visibility("hidden")));
// The ids compared with, declared as raw storage (as CEmmyStateOverrideLogicAction_SetUp_b55.cpp does
// g_unk_71726ded60).
extern char g_unk_71726df1a0[];
extern char g_unk_71726ded60[];
extern char g_unk_71726df038[];

struct UnkEmmyStateIdTest_b56a3 {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;
};

namespace {

// A string id built empty inline (as CScenario.cpp's UnkScenarioEmptyStrId).
struct UnkEmptyStrId_b56a3 {
    union {
        CStrId id;
    };

    UnkEmptyStrId_b56a3() {
        id.data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        id.data = &g_unk_7101d47108->empty;
    }
    ~UnkEmptyStrId_b56a3() { id.~CStrId(); }
};

}  // namespace

static inline __attribute__((always_inline)) bool IsStateId_b56a3(UnkEmmyStateIdTest_b56a3* self, const CStrId& want) {
    UnkEmptyStrId_b56a3 value;
    CBlackboard* blackboard = &self->blackboard;
    asm("" : "+r"(blackboard));
    const CStrId& section = *reinterpret_cast<const CStrId*>(reinterpret_cast<char*>(&GameManagerIdTable()) + 0x1b58);
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        fn_710051d464(s, g_unk_71726e20a8, &value.id);
    return value.id.data == want.data;
}

bool fn_7100543b30(UnkEmmyStateIdTest_b56a3* self) { return IsStateId_b56a3(self, *reinterpret_cast<const CStrId*>(g_unk_71726df1a0)); }
bool fn_710054ad2c(UnkEmmyStateIdTest_b56a3* self) { return IsStateId_b56a3(self, *reinterpret_cast<const CStrId*>(g_unk_71726ded60)); }
bool fn_710054aecc(UnkEmmyStateIdTest_b56a3* self) { return IsStateId_b56a3(self, *reinterpret_cast<const CStrId*>(g_unk_71726df038)); }
