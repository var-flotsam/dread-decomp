// CReserveTankInfo's unit (tools/tu_map.py unit 2593): a flag stored in the global blackboard, and
// another constructor of an object holding two qualified ids (game/UnkGameBlackboard.h), as the pairs
// of CReserveTankInfo_QidPairCtor_b55.cpp, with a flag after them (+0x30, cleared). Names and layouts
// are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CGameObject.h"
#include "game/CReserveTankInfo.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

extern char g_unk_7101caa570[];

struct UnkDef_7100fc4b34;  // src/game_s7/fn_7100fc4b34.cpp

// The view of a definition this file uses.
struct UnkDefView_ov110 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedId qid;
};

static inline const UnkQualifiedId& Qid_ov110(UnkDef_7100fc4b34& def_) {
    UnkDefView_ov110& def = reinterpret_cast<UnkDefView_ov110&>(def_);
    if (def.qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&def.qid, &def, g_unk_7101d470e0);
    return def.qid;
}

struct UnkQidPairBase_ov110 {
    void** vtable;
    u8 pad_0x8[0x8];

    __attribute__((always_inline)) UnkQidPairBase_ov110() {
        fn_7101043904(this);
        vtable = reinterpret_cast<void**>(g_unk_7101caa4d0) + 2;
    }
};

struct UnkQidPairFlag_ov110 : UnkQidPairBase_ov110 {
    UnkQualifiedId first;
    UnkQualifiedId second;
    bool flag;

    __attribute__((always_inline)) UnkQidPairFlag_ov110(const UnkQualifiedId& a, const UnkQualifiedId& b)
        : first(a), second(b), flag(false) {
        vtable = reinterpret_cast<void**>(g_unk_7101caa570) + 2;
    }
};

void fn_7100f10e18(void* self) {
    new (self) UnkQidPairFlag_ov110(Qid_ov110(g_unk_71729dbcc0), Qid_ov110(g_unk_71729dbd00));
}

struct UnkReserveTankFlag_ov110 {
    u8 pad_0x0[0x20];
    UnkPropertyName name;  // 0x20
};

// Stores `value` under the object's property (+0x20) in the global manager's blackboard section named
// by the string table's id at 0x1148, unless the blackboard is locked.
void fn_7100f10b00(UnkReserveTankFlag_ov110* self, bool value) {
    UnkTypedValue typed;
    bool v = value;
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    const CStrId& id = base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x229];
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, id)) {
        if (!blackboard->field_0xe8) {
            UnkPropertyName* name = &self->name;
            typed.type = fn_710000d228();
            typed.value = &v;
            fn_7100332c80(section, name, &typed);
        }
    }
}
