#pragma once

// The object of unit 51 (tools/tu_map.py: 0x7100072564-0x7100075db8, whose reflected class is
// base::input::CRumbleManager::CTunableRumbleManager; the unit's own class name base::input::
// CRumbleManager is the hint, the exact type of this object is unverified). It owns the
// vibration graph of the controllers (nn::hid mixers, targets and node connections, declared in
// base/input/UnkHidVibration.h) and
// two hash maps. Only what the constructor fn_71000733e4 initialises is spelled out; names and
// layout are placeholders.

#include "base/UnkBlockFreeList.h"
#include "base/UnkVector.h"
#include "base/input/UnkHidVibration.h"
#include "types.h"

namespace base::input {

// A chained hash map's head (base/UnkHashMap.h layout; fn_7100075958 sets up its storage): an
// allocator at +0, flags +0x8, the entry count at +0xc, the bucket array at +0x10 and its counts
// and growth step at +0x18-0x24, the buckets' flags and allocator at +0x28/+0x30, the node list
// at +0x38/+0x40, the node pool (vtable +0x48, allocator +0x50, free list +0x58).
struct UnkPooledChainedMap {
    base::UnkAllocator* alloc;  // +0x0
    u32 flags;                  // +0x8, 0xe
    u8 words_0xc[0x18];         // +0xc, cleared (and the step at +0x20 set) as words
    bool field_0x24;            // true
    u8 pad_0x25[3];
    u32 field_0x28;  // 0xe
    u8 pad_0x2c[4];
    base::UnkAllocator* alloc_0x30;
    u64 field_0x38;
    u64 field_0x40;
    void* pool_vtable;  // +0x48
    u64 field_0x50;
    UnkBlockFreeList pool;  // +0x58
    u32 field_0x80;
    bool field_0x84;
    u8 pad_0x85[3];
};
static_assert(sizeof(UnkPooledChainedMap) == 0x88, "UnkPooledChainedMap placeholder layout");

struct UnkRumbleManager {
    void* vtable;  // +0x0: set twice (base, then this class)
    u8 pad_0x8[0x1e4];
    u8 field_0x1ec[8];  // zeroed as words (unaligned)
    u8 field_0x1f4[8];
    u32 field_0x1fc;
    nn::hid::VibrationMixer mixers[2];                // +0x200
    nn::hid::VibrationTarget targets[2];              // +0x290
    nn::hid::VibrationNodeConnection connections[2];  // +0x330
    u8 sub_0x3a0[0x1d00];                             // built by fn_710007533c
    u64 field_0x20a0;
    u64 field_0x20a8;
    u64 field_0x20b0;
    u32 field_0x20b8;                // 10
    u32 field_0x20bc;                // 0xe
    base::UnkAllocator* alloc_0x20c0;
    u8 pad_0x20c8[8];
    UnkPooledChainedMap map;  // +0x20d0
    u64 field_0x2158;
    u64 field_0x2160;
    u32 field_0x2168;  // 10
    bool field_0x216c;
    u8 pad_0x216d[3];
    u32 field_0x2170;  // 0xf
    u8 pad_0x2174[4];
    base::UnkAllocator* alloc_0x2178;
    u64 field_0x2180;
    u8 pad_0x2188[8];
    UnkBlockFreeList pool_0x2190;
    u64 field_0x21b8;
    u64 field_0x21c0;
    u32 field_0x21c8;  // 10
    bool field_0x21cc;
    u8 pad_0x21cd[3];
    u32 field_0x21d0;  // 0xf
    u8 pad_0x21d4[4];
    base::UnkAllocator* alloc_0x21d8;
    u8 pad_0x21e0[8];
    u32 field_0x21e8;
    u32 field_0x21ec;
    u32 field_0x21f0;
    u8 pad_0x21f4[4];
    u64 field_0x21f8;
    u32 field_0x2200;
    u32 field_0x2204;
    u16 field_0x2208;  // 0x100
    u8 pad_0x220a[6];
};
static_assert(sizeof(UnkRumbleManager) == 0x2210, "UnkRumbleManager placeholder layout");

}  // namespace base::input

// Its constructor (a plain function; callers pass the storage). Defined in
// src/base/input/CRumbleManager.cpp.
void fn_71000733e4(base::input::UnkRumbleManager* self);
