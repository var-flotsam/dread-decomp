#pragma once

#include "base/UnkDestructors.h"

// A material colour change as fn_71009272c4 builds it (kind at +0, a string id at +0x38).
struct UnkMagnetColorChange_b0086 {
    u32 kind;
    u8 pad_0x4[0x34];
    union {
        base::UnkStrIdMember id;  // 0x38
    };
    u8 pad_0x40[0x18];

    UnkMagnetColorChange_b0086() {}  // built by fn_71009272c4
    ~UnkMagnetColorChange_b0086() { id.~UnkStrIdMemberOf(); }
};
