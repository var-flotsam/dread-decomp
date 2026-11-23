#pragma once

// CEmmyMovement: the movement of an E.M.M.I., as far as its search actions use it, and the functions of its
// unit (tools/tu_map.py unit 814, 0x7100571348-0x7100571b18) that other files call. Names and layouts are
// placeholders.
#include "base/reflection/ReflectedAttributes.h"
#include "game/UnkNavMeshCell.h"
#include "types.h"

struct UnkEmmyMovement {
    void** vtable;
    u8 pad_0x8[0x9dc];
    u8 field_0x9e4[0x4];

    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkEmmyMovement*)>(vtable[0])(this);
    }
    UnkNavMeshCell* vf_0x2d0() {
        return reinterpret_cast<UnkNavMeshCell* (*)(UnkEmmyMovement*)>(vtable[0x2d0 / 8])(this);
    }
};

// The index of the animation the movement's direction picks.
s32 fn_71005717d4(UnkEmmyMovement* self, void* arg);
