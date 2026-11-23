#pragma once

#include "game/CCooldownXBossFireBallDef.h"
#include "game/UnkFireBallDefData_ovb0072.h"
#include "game/UnkFireBallDefVtable_ovb0072.h"
#include "types.h"

struct UnkFireBallDef_ovb0072;

struct UnkFireBallDef_ovb0072 {
    const UnkFireBallDefVtable_ovb0072* vtable;
    UnkFireBallDefData_ovb0072 data;  // 0x8

    // The vtable is stored untyped: the original reloads the array after it.
    UnkFireBallDef_ovb0072() {
        const char* vt = g_unk_7101a3e2b0 + 0x10;
        __builtin_memcpy(&vtable, &vt, sizeof(vt));
        data.field_0x0 = true;
        data.field_0x4 = 0.0f;
        data.field_0x8 = false;
    }
    UnkFireBallDef_ovb0072(const UnkFireBallDef_ovb0072& other)
        : vtable(reinterpret_cast<const UnkFireBallDefVtable_ovb0072*>(g_unk_7101a3e2b0 + 0x10)), data(other.data) {}
    UnkFireBallDef_ovb0072(UnkFireBallDef_ovb0072&& other)
        : vtable(reinterpret_cast<const UnkFireBallDefVtable_ovb0072*>(g_unk_7101a3e2b0 + 0x10)), data(other.data) {}
    ~UnkFireBallDef_ovb0072() { vtable->dtor(this); }
};
