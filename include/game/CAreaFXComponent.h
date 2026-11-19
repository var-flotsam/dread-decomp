#pragma once

// CAreaFXComponent: functions of this translation unit that other files call (tools/tu_map.py unit 1263,
// 0x71007fdc30-0x7100803c40). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

// The rectangle the area effect covers: its low corner, then its high corner. Copied field by field (a
// user-written copy), so passed by value in memory.
struct UnkAreaFXRecord_A5_b66 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;

    UnkAreaFXRecord_A5_b66(const UnkAreaFXRecord_A5_b66& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0xc(other.field_0xc) {}
};

struct UnkAreaFXResult_A5_b66;
struct UnkSelf_af41;
UnkAreaFXResult_A5_b66 fn_71007fdc30(UnkAreaFXRecord_A5_b66 record, void* a, void* b);
void fn_7100802108(UnkSelf_af41* self);
