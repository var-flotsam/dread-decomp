#pragma once

// An engine file device object of unit 66 (tools/tu_map.py: 0x7100089284-0x710008b6d4, whose
// reflected class is base::file::CPackManagerDevice; the object here is over 0x1330 bytes, far
// larger than that class's reflected size 0x88, so which class it is stays unverified). Its
// vtable is at GOT 0x7101cbc488. Only what the constructor fn_71000893a8 initialises is spelled
// out; names and layout are placeholders and the real size is unknown.

#include <new>

#include <nn/os.h>

#include "base/global/CFilePathStrId.h"
#include "types.h"

namespace base::file {

// A path id with a flag byte, a constant 9 and two zeroed words (0x20 bytes).
struct UnkPathSlot {
    base::global::CFilePathStrId path;  // +0x0, built from the empty string
    u8 field_0x8;
    u8 pad_0x9[3];
    u32 field_0xc;  // 9
    u64 field_0x10;
    u64 field_0x18;
};
static_assert(sizeof(UnkPathSlot) == 0x20, "UnkPathSlot placeholder layout");

struct UnkFileDevice {
    void* vtable;       // +0x0
    u8 base_0x8[0xb0];  // a base object built by fn_710008b88c
    u8 field_0xb8;
    u8 pad_0xb9[7];
    UnkPathSlot slots[128];  // +0xc0
    u64 field_0x10c0;
    u32 field_0x10c8;
    u8 pad_0x10cc[4];
    nn::os::MutexType mutex;  // +0x10d0
    u8 pad_0x10f0[0x20];
    u64 field_0x1110;
    u16 field_0x1118;
    u8 pad_0x111a[0x20a];
    u8 tail_0x1324[0x14];  // zeroed by overlapping word stores
};
static_assert(sizeof(UnkFileDevice) == 0x1338, "UnkFileDevice placeholder layout");

}  // namespace base::file

// Its constructor (a plain function; callers pass the storage). Defined in
// src/base/file/UnkFileDevice.cpp.
void fn_71000893a8(base::file::UnkFileDevice* self);
