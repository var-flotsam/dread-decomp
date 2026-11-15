#pragma once

// CMissionLog's unit (tools/tu_map.py): a 0x38-byte array element, a word and a string-like member
// destroyed out of line (fn_7100b5a8f0) and moved by taking its buffer. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

void fn_7100b5a8f0(char*);

struct UnkMissionLogStr {
    char* field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    bool field_0x1c;
    u32 field_0x20;
    u64 field_0x28;

    UnkMissionLogStr(UnkMissionLogStr&& other) {
        field_0x0 = other.field_0x0;
        other.field_0x0 = reinterpret_cast<char*>(g_unk_7101d47108) + 0x10;
        field_0x8 = other.field_0x8;
        field_0x10 = other.field_0x10;
        field_0x14 = other.field_0x14;
        field_0x18 = other.field_0x18;
        field_0x1c = other.field_0x1c;
        field_0x20 = other.field_0x20;
        field_0x28 = other.field_0x28;
        other.field_0x8 = 0;
        other.field_0x10 = 0;
        other.field_0x14 = 0;
    }
};

struct UnkMissionLogElem {
    u32 field_0x0;
    UnkMissionLogStr field_0x8;

    UnkMissionLogElem(UnkMissionLogElem&& other) : field_0x0(other.field_0x0), field_0x8(static_cast<UnkMissionLogStr&&>(other.field_0x8)) {}
    // fn_7100b5a8f0 destroys the member 0x10 after the address it is given: the element's own string is at +0x8.
    ~UnkMissionLogElem() { fn_7100b5a8f0(reinterpret_cast<char*>(this) - 0x8); }
};
