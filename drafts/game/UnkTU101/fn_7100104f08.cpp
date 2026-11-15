// Unit 101: default construction of a sound source's parameter record (floats at one, the name and file
// path ids copied from the empty ids, the vector defaults, ranges unset). Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/UnkZeroVector3.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::math::CVector3D;

struct UnkSource_l104f08 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    u8 pad_0x20[0x30 - 0x20];
    u64 field_0x30;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    u8 pad_0x48[0x64 - 0x48];
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u8 pad_0x70[0x78 - 0x70];
    u64 field_0x78;
    u64 field_0x80;
    CStrId name;  // 0x88
    CFilePathStrId path;  // 0x90
    CVector3D vec_a;  // 0x98
    CVector3D vec_b;  // 0xa4
    s32 field_0xb0[4];
    s32 field_0xc0;
    s32 field_0xc4;
    s32 field_0xc8;
    s32 field_0xcc;
    s32 field_0xd0;
    s32 field_0xd4;
    u64 field_0xd8;
    u32 field_0xe0;
    u64 field_0xe4;
    u32 field_0xec;
    u32 field_0xf0;
    u32 field_0xf4;
    u32 field_0xf8;
    u64 field_0xfc;
    u32 field_0x104;
    u32 field_0x108;
    u32 field_0x10c;
    u64 field_0x110;
    u64 field_0x118;
    bool field_0x120;
    u8 pad_0x121[3];
    bool field_0x124;

    UnkSource_l104f08()
        : field_0x0(0), field_0x4(0), field_0x8(0), field_0xc(0), field_0x10(0), field_0x14(0), field_0x18(0),
          field_0x1c(0), pad_0x20(), field_0x30(0), field_0x38(0), field_0x3c(0), field_0x40(0), field_0x44(0),
          pad_0x48(), field_0x64(0), field_0x68(0), field_0x6c(0), pad_0x70(), field_0x78(0), field_0x80(0),
          name(g_unk_7101d470e0), path(g_unk_7101d470e8), vec_a(g_unk_710164779c), vec_b(g_unk_710164779c) {
        field_0xb0[0] = field_0xb0[1] = field_0xb0[2] = field_0xb0[3] = 0x7fffffff;
        field_0xc0 = 1;
        field_0xc4 = field_0xc8 = field_0xcc = field_0xd0 = field_0xd4 = -1;
        field_0xd8 = 0;
        field_0xe0 = 0x3e80;
        field_0xe4 = 0;
        field_0xec = field_0xf0 = field_0xf4 = 0x3f800000;
        field_0xf8 = 0;
        field_0xfc = 0;
        field_0x104 = 0xbf800000;
        field_0x108 = 0;
        field_0x10c = 0xbf800000;
        field_0x110 = 0;
        field_0x118 = 0;
        field_0x120 = false;
        field_0x124 = true;
        field_0x0 = field_0x4 = field_0x8 = 1.0f;
        field_0x14 = field_0x18 = field_0x1c = 1.0f;
        field_0x3c = field_0x40 = field_0x44 = 1.0f;
        field_0x64 = field_0x68 = field_0x6c = 0x3e80;
    }
};
