#pragma once

#include "base/UnkDestructors.h"
#include "types.h"

// A report record (0x20 bytes, as src/game/CInventoryComponent/fn_7101147868.cpp lays it out): the key, a string
// value, an integer value and the value's kind. Built out of line from a key and a string id
// (fn_7100ec4240) or an integer (fn_71003afa88); both take the ids by value.
struct UnkRecord_7101147868 {
    base::global::CStrId field_0x0;
    base::UnkStrIdMember field_0x8;
    u64 field_0x10;
    u32 field_0x18;

    UnkRecord_7101147868(base::global::CStrId name, base::global::CStrId value) asm("fn_7100ec4240");
    UnkRecord_7101147868(base::global::CStrId name, s64 value) asm("fn_71003afa88");
};
