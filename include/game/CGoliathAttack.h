#pragma once

#include "base/global/CStrId.h"
#include "game/CAttack.h"

struct CGoliathAttack : CAttack {
    ~CGoliathAttack() override;
    u64 field_0x28;
    u64 field_0x30;
    u32 field_0x38;
    base::global::CStrId field_0x40;
    UnkAttackListener field_0x48;
    u64 field_0x68;
    // Reflection's value operations (CType+0x28..+0x58), defined out of line.
    static void refl_CopyConstruct(CGoliathAttack* self, const CGoliathAttack* other);
    static void refl_MoveConstruct(CGoliathAttack* self, const CGoliathAttack* other);
};
