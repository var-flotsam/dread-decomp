#pragma once

// Unit 2506's group array (tools/tu_map.py unit 2506): groups of up to 128 records of 0x70 bytes (five
// refcounted pointers, an id, cleared words and a float), each group paired with a refcounted handle, and
// the array header (groups, handles, count, capacity, step, flags, allocator) that
// src/game/UnkTU2506/UnkTU2506_Groups_ovb0244.cpp reserves, pops and clears. Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "types.h"

#pragma pack(push, 4)
struct UnkSlotRecord_ovb0244 {
    base::UnkRefPtrMember refs[5];
    s32 id;
    u64 words[6];
    u32 pad_0x5c;
    u64 field_0x60;
    f32 value;
    u32 pad_0x6c;
    UnkSlotRecord_ovb0244() : refs{}, id(-1), words{}, field_0x60(0), value(-1.0f) {}
};
struct UnkSlotGroup_ovb0244 {
    u32 count;
    UnkSlotRecord_ovb0244 items[128];
};
#pragma pack(pop)

// The handle's count is a union member: the original reloads the pointers around count updates here.
struct UnkRefCountedU_ovb0244 {
    virtual void vf0();
    virtual void vf8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

struct UnkGroupArray_ovb0244 {
    UnkSlotGroup_ovb0244* groups;
    UnkRefCountedU_ovb0244** handles;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::IAllocator* allocator;
};

