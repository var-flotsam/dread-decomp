// CReserveTankInfo's unit (tools/tu_map.py unit 2593): constructors of objects holding two
// qualified ids (game/UnkGameBlackboard.h) copied from two property definitions, each id built on
// first use before the object is: the base constructor (fn_7101043904), the base vtable
// 0x7101caa4d0, the two ids, then the class's vtable. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkGameBlackboard.h"
#include "types.h"

void fn_7101043904(void*);
extern char g_unk_7101caa4d0[];
extern char g_unk_7101caa520[];
extern char g_unk_7101caa5c0[];

struct UnkDef_7100fc4b34;  // src/game_s7/fn_7100fc4b34.cpp
extern UnkDef_7100fc4b34 g_unk_71729dbc40;
extern UnkDef_7100fc4b34 g_unk_71729dbc80;
extern UnkDef_7100fc4b34 g_unk_71729dbd40;
extern UnkDef_7100fc4b34 g_unk_71729dbd80;

// The view of a definition this file uses.
struct UnkDefView_b55 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedId qid;
};

static inline const UnkQualifiedId& Qid_b55(UnkDef_7100fc4b34& def_) {
    UnkDefView_b55& def = reinterpret_cast<UnkDefView_b55&>(def_);
    if (def.qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&def.qid, &def, g_unk_7101d470e0);
    return def.qid;
}

struct UnkQidPairBase_b55 {
    void** vtable;
    u8 pad_0x8[0x8];

    __attribute__((always_inline)) UnkQidPairBase_b55() {
        fn_7101043904(this);
        vtable = reinterpret_cast<void**>(g_unk_7101caa4d0) + 2;
    }
};

template <char* Vtable>
struct UnkQidPair_b55 : UnkQidPairBase_b55 {
    UnkQualifiedId first;
    UnkQualifiedId second;

    __attribute__((always_inline)) UnkQidPair_b55(const UnkQualifiedId& a, const UnkQualifiedId& b)
        : first(a), second(b) {
        vtable = reinterpret_cast<void**>(Vtable) + 2;
    }
};

void fn_7100f10c4c(UnkQidPair_b55<g_unk_7101caa520>* self) {
    new (self) UnkQidPair_b55<g_unk_7101caa520>(Qid_b55(g_unk_71729dbc40), Qid_b55(g_unk_71729dbc80));
}
void fn_7100f10f20(UnkQidPair_b55<g_unk_7101caa5c0>* self) {
    new (self) UnkQidPair_b55<g_unk_7101caa5c0>(Qid_b55(g_unk_71729dbd40), Qid_b55(g_unk_71729dbd80));
}
