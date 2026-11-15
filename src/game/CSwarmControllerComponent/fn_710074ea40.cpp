// CSwarmControllerComponent's unit: grows an array of 0x40-byte polymorphic entries and its parallel
// array of words to a capacity, moving the entries over one by one. Names and layouts are placeholders.
#include <string.h>

#include "base/global/CRntVector.h"
#include "game/UnkTU341.h"
#include "types.h"

extern char g_unk_7101a6dee0[];

struct UnkSwarmEntry_l74ea40 {
    char* vtable;  // 0x0
    u32 field_0x8;
    u32 pad_0xc;
    u64 field_0x10;
    void* buffer;  // 0x18
    u32 size;      // 0x20
    u32 capacity;  // 0x24
    u32 field_0x28;
    u8 field_0x2c;
    u8 pad_0x2d[3];
    u32 field_0x30;
    u8 pad_0x34[4];
    base::UnkAllocator* allocator;  // 0x38

    // The move constructor.
    UnkSwarmEntry_l74ea40(UnkSwarmEntry_l74ea40& other) {
        field_0x8 = 0;
        vtable = g_unk_7101a6dee0 + 0x10;
        field_0x10 = other.field_0x10;
        buffer = other.buffer;
        size = other.size;
        capacity = other.capacity;
        field_0x28 = other.field_0x28;
        field_0x2c = other.field_0x2c;
        field_0x30 = other.field_0x30;
        allocator = other.allocator;
        other.buffer = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    // The destructor: releases the buffer.
    ~UnkSwarmEntry_l74ea40() {
        if (buffer) {
            size = 0;
            allocator->vf_0x48(buffer);
            capacity = 0;
        }
        vtable = g_unk_7101a20a08 + 0x10;
    }
};
static_assert(sizeof(UnkSwarmEntry_l74ea40) == 0x40, "layout");

struct UnkSwarmEntryArray_l74ea40 {
    UnkSwarmEntry_l74ea40* entries;  // 0x0
    void** items;                    // 0x8
    u32 count;                       // 0x10
    u32 capacity;                    // 0x14
    u32 step;                        // 0x18
    u32 flags;                       // 0x1c
    base::UnkAllocator* allocator;   // 0x20
};

// Reserves room for n entries in both arrays.
// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710074ea40(UnkSwarmEntryArray_l74ea40* self, u32 n) {
    if (self->capacity >= n)
        return;
    auto* entries = static_cast<UnkSwarmEntry_l74ea40*>(
        self->allocator->vf_0x40((u64)n * sizeof(UnkSwarmEntry_l74ea40), 8, self->flags));
    auto* items = static_cast<void**>(self->allocator->vf_0x40((u64)n * sizeof(void*), 8, self->flags));
    if (UnkSwarmEntry_l74ea40* old = self->entries) {
        UnkSwarmEntry_l74ea40* dst = entries;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
            new (dst) UnkSwarmEntry_l74ea40(*old);
            old->~UnkSwarmEntry_l74ea40();
        }
        self->allocator->vf_0x48(self->entries);
    }
    if (self->items) {
        memcpy(items, self->items, (u64)self->count * sizeof(void*));
        self->allocator->vf_0x48(self->items);
    }
    self->entries = entries;
    self->items = items;
    self->capacity = n;
}
