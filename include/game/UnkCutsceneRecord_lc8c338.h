#pragma once

// The cutscene record of unit 2200 (hint CCutSceneDef) that fn_7100c8c7b4 assigns: ids, four arrays
// (takes, shots, path ids, path entries) and a count; its destructor is fn_7100c8c338
// (src/game/UnkTU2200/UnkTU2200_Destroy_lc8c338.cpp). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2200.h"
#include "types.h"

// A 0x40-byte take, destroyed out of line.
struct UnkTake_lc8c338 {
    u8 unk_0x0[0x40];

    ~UnkTake_lc8c338() { fn_7100c8a490(this); }
};

// A 0xa0-byte shot, destroyed out of line.
struct UnkShot_lc8c338 {
    u8 unk_0x0[0xa0];

    ~UnkShot_lc8c338() { fn_7100c886c0(this); }
};

struct UnkPathEntry_lc8c338 {
    base::global::CFilePathStrId path;
    u64 value;
};

struct UnkRecord_lc8c338 {
    u32 field_0x0;
    base::global::CStrId field_0x8;
    base::global::CFilePathStrId field_0x10;
    f32 field_0x18[3];
    base::global::CStrId field_0x28;
    base::global::CStrId field_0x30;
    base::global::CStrId field_0x38;
    base::global::CStrId field_0x40;
    base::global::CStrId field_0x48;
    base::global::CStrId field_0x50;
    u64 field_0x58;
    u32 field_0x60;
    bool field_0x64;
    base::global::CRntVector<UnkTake_lc8c338> takes;  // 0x68
    base::global::CRntVector<UnkShot_lc8c338> shots;  // 0x90
    base::global::CRntVector<base::global::CFilePathStrId> paths;  // 0xb8
    base::global::CRntVector<UnkPathEntry_lc8c338> entries;  // 0xe0
    u32 field_0x108;

    ~UnkRecord_lc8c338() asm("fn_7100c8c338");  // fn_7100c8c338
};
static_assert(__builtin_offsetof(UnkRecord_lc8c338, takes) == 0x68, "takes");
static_assert(__builtin_offsetof(UnkRecord_lc8c338, field_0x108) == 0x108, "field_0x108");
