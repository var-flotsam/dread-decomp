// Copies of fn_7100c073f0 (src/game_aca57c/fn_7100c073f0.cpp) for other types, with their own callees and data (the names, strings and comments below are the original's): fn_7101059bfc.
// Constructor of an array of 0x50-byte polymorphic elements with the given allocator flags,
// reserving `n` elements up front.
#include "base/UnkVector.h"
#include "types.h"

using base::UnkAllocator;

// An array moved by taking over its storage (the source keeps its step and allocator).
struct UnkMovedArray {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    UnkAllocator* allocator;

    UnkMovedArray(UnkMovedArray&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};

extern void* g_unk_7101aa28d8[];

struct UnkElem50 {
    virtual void vf_0x0();
    virtual ~UnkElem50();

    UnkMovedArray field_0x8;
    bool field_0x30;
    u32 field_0x34[6];

    UnkElem50(UnkElem50&& other)
        : field_0x8(static_cast<UnkMovedArray&&>(other.field_0x8)), field_0x30(other.field_0x30), field_0x34() {}
};
static_assert(sizeof(UnkElem50) == 0x50, "UnkElem50 layout");

void fn_7101059bfc(base::UnkVector<UnkElem50>* self, u32 n, u32 flags) {
    new (self) base::UnkVector<UnkElem50>(flags);
    self->Reserve(n);
}

// An owner of such an array at 0x10: reserves `n` elements and sets the flag at 0x44.
struct UnkElem50Owner {
    u8 unk_0x0[0x10];
    base::UnkVector<UnkElem50> elems;
    u8 unk_0x38[0xc];
    bool field_0x44;
};

