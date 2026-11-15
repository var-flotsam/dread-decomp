// CSwarmControllerComponent's unit: removes an element from an array of 0x40-byte polymorphic
// entries by moving the last one into its place (and its index entry with it); returns the slot.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

extern char g_unk_7101a20a08[];
extern char g_unk_7101a6dee0[];

struct UnkSwarmEntry_l74a568 {
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

    // The destructor: releases the buffer.
    void Destroy() {
        if (buffer) {
            size = 0;
            allocator->vf_0x48(buffer);
            capacity = 0;
        }
        vtable = g_unk_7101a20a08 + 0x10;
    }

    // The move constructor, into the slot at this address.
    void MoveFrom(UnkSwarmEntry_l74a568& other) {
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
};
static_assert(sizeof(UnkSwarmEntry_l74a568) == 0x40, "layout");

struct UnkSwarmEntries_l74a568 {
    UnkSwarmEntry_l74a568* entries;  // 0x0
    u64* order;                      // 0x8
    u32 count;                       // 0x10
};

UnkSwarmEntry_l74a568* fn_710074a568(UnkSwarmEntries_l74a568* self, UnkSwarmEntry_l74a568* entry) {
    UnkSwarmEntry_l74a568* first = self->entries;
    u32 index = (u32)(((char*)entry - (char*)first) >> 6);
    first[index].Destroy();
    u32 last = self->count - 1;
    if (last != index) {
        self->entries[index].MoveFrom(self->entries[last]);
        self->entries[self->count - 1].Destroy();
        self->order[index] = self->order[self->count - 1];
    }
    self->count = self->count - 1;
    return &self->entries[index];
}
