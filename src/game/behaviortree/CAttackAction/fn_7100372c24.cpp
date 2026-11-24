// Deleting destructors of objects with a name string and a string id after the base (as in
// fn_710116baf4, src/game/CCharClassStartPointComponent/fn_710116baf4.cpp), over other layouts, vtables and base
// destructors: fn_7100372c24 (0x30/0x60, base fn_7100393ea0), fn_71003a3bfc (0x38/0x68, base
// fn_7100394530).
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/behaviortree/CAttackAction.h"
#include "game/behaviortree/CStringEqualsCondition.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkBase_7100393ea0 {
    void** vtable;
    u8 unk_0x8[0x28];
    ~UnkBase_7100393ea0();  // fn_7100393ea0
};

struct UnkBase_7100394530 {
    void** vtable;
    u8 unk_0x8[0x30];
    ~UnkBase_7100394530();  // fn_7100394530
};

struct UnkObject_7100372c24 : UnkBase_7100393ea0 {
    CRntString sAttack;
    UnkStrIdTwoStep field_0x60;

    ~UnkObject_7100372c24() { vtable = &g_unk_7101a25010[2]; }
};

struct UnkObject_71003a3bfc : UnkBase_7100394530 {
    CRntString field_0x38;
    UnkStrIdTwoStep field_0x68;

    ~UnkObject_71003a3bfc() { vtable = &g_unk_7101a28ac0[2]; }
};

void fn_7100372c24(UnkObject_7100372c24& self) { delete &self; }

void fn_71003a3bfc(UnkObject_71003a3bfc& self) { delete &self; }
