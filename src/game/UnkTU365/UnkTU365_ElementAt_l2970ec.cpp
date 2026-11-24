// Unit 365 (navmesh::CNavMeshGenerator): the element at an index of a growable array of 0x158-byte
// elements; an index past the end first reserves capacity (to the next multiple of the step) and
// then index + 1 elements and constructs the new ones. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkTU109.h"
#include "game/UnkTU365.h"
#include "types.h"

struct UnkElem_l2970ec {
    u8 outline_a[0x40];  // 0x0, constructed by fn_7100123eb4
    u64 field_0x40;
    char* vtable_a;  // 0x48
    u32 field_0x50;
    u8 pad_0x54[4];
    u64 field_0x58;
    u64 field_0x60;
    u32 field_0x68;
    u8 pad_0x6c[4];
    base::IAllocator* allocator_a;  // 0x70
    char* vtable_b;                   // 0x78
    u32 field_0x80;
    u8 pad_0x84[4];
    u64 field_0x88;
    u64 field_0x90;
    base::IAllocator* allocator_b;  // 0x98
    u32 field_0xa0;
    u8 pad_0xa4[4];
    u8 outline_b[0x40];  // 0xa8
    char* vtable_c;      // 0xe8
    u32 field_0xf0;
    u8 pad_0xf4[4];
    u64 field_0xf8;
    u64 field_0x100;
    base::IAllocator* allocator_c;  // 0x108
    u32 field_0x110;
    u8 pad_0x114[4];
    u8 outline_c[0x40];  // 0x118

    UnkElem_l2970ec() {
        fn_7100123eb4(reinterpret_cast<UnkTileOutlineList*>(outline_a));
        field_0x40 = 0;
        vtable_a = g_unk_7101a20f48 + 0x10;
        field_0x50 = 0;
        field_0x68 = 0x10;
        field_0x58 = 0;
        field_0x60 = 0;
        allocator_a = GetDefaultAllocator();
        vtable_b = g_unk_7101a20f68 + 0x10;
        field_0x80 = 0;
        field_0x88 = 0;
        field_0x90 = 0;
        allocator_b = GetDefaultAllocator();
        field_0xa0 = 0x13;
        fn_7100123eb4(reinterpret_cast<UnkTileOutlineList*>(outline_b));
        vtable_c = g_unk_7101a20f88 + 0x10;
        field_0xf0 = 0;
        field_0xf8 = 0;
        field_0x100 = 0;
        allocator_c = GetDefaultAllocator();
        field_0x110 = 0x13;
        fn_7100123eb4(reinterpret_cast<UnkTileOutlineList*>(outline_c));
    }
};
static_assert(sizeof(UnkElem_l2970ec) == 0x158, "layout");

struct UnkArray_l2970ec {
    UnkElem_l2970ec* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;
};

// As src/game/UnkTU365/UnkTU365_Misc_ovb0228.cpp and UnkTU365_Containers_ovb0229.cpp declare them.
struct UnkBigSlot_b0228;
struct UnkNavRegions_ovb0229;
void fn_71002a9558(base::global::CRntVector<UnkBigSlot_b0228>* self, u32 capacity);

u8* fn_71002970ec(UnkNavRegions_ovb0229* regions, u32 index) {
    auto* self = reinterpret_cast<UnkArray_l2970ec*>(regions);
    auto* list = reinterpret_cast<base::global::CRntVector<UnkBigSlot_b0228>*>(self);
    if (self->field_0x14 && self->count <= index) {
        u32 capacity = self->capacity;
        if (capacity <= index && self->step != 0)
            capacity = (index / self->step + 1) * self->step;
        fn_71002a9558(list, capacity);
        u32 n = index + 1;
        fn_71002a9558(list, n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkElem_l2970ec();
        self->count = n;
    }
    return reinterpret_cast<u8*>(&self->data[index]);
}
