// CBreakableTileGroupComponent unit (tools/tu_map.py, the name from reflection): fn_710075da30 clears a
// record: the 0x40-byte polymorphic elements of the array at +0x18 (count +0x20, capacity +0x24) are
// destroyed (vf_0x0), the buffer goes back to the allocator at +0x38 (vf_0x48) and the capacity is
// cleared; then the id at +0x10 is released. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkTileGroupElement_A3_b72 {
    void** vtable;
    u8 pad_0x8[0x38];

    void vf_0x0() { reinterpret_cast<void (*)(UnkTileGroupElement_A3_b72*)>(vtable[0])(this); }
};

struct UnkTileGroupAllocator_A3_b72 {
    void** vtable;

    void vf_0x48(void* p) {
        reinterpret_cast<void (*)(UnkTileGroupAllocator_A3_b72*, void*)>(vtable[0x48 / 8])(this, p);
    }
};

struct UnkTileGroupRecord_A3_b72 {
    u8 pad_0x0[0x10];
    CStrId id;                            // 0x10
    UnkTileGroupElement_A3_b72* data;     // 0x18
    u32 count;                            // 0x20
    u32 capacity;                         // 0x24
    u8 pad_0x28[0x10];
    UnkTileGroupAllocator_A3_b72* alloc;  // 0x38
};

void fn_710075da30(UnkTileGroupRecord_A3_b72* self) {
    if (self->data) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].vf_0x0();
        UnkTileGroupElement_A3_b72* data = self->data;
        self->count = 0;
        self->alloc->vf_0x48(data);
        self->capacity = 0;
    }
    self->id.Reset();
}
