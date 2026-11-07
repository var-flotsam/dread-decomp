// Copies of fn_7100c073f0 (src/game/SCameraRailNode/fn_7100c073f0.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_7101059bfc.
// The element class here is the copy's own (vtable 0x7101cac938), named apart from the original's.
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

struct UnkElem50_7101cac938 {
    virtual void vf_0x0();
    virtual ~UnkElem50_7101cac938();

    UnkMovedArray field_0x8;
    bool field_0x30;
    u32 field_0x34[6];

    UnkElem50_7101cac938(UnkElem50_7101cac938&& other)
        : field_0x8(static_cast<UnkMovedArray&&>(other.field_0x8)), field_0x30(other.field_0x30), field_0x34() {}
};
static_assert(sizeof(UnkElem50_7101cac938) == 0x50, "UnkElem50_7101cac938 layout");

void fn_7101059bfc(base::UnkVector<UnkElem50_7101cac938>* self, u32 n, u32 flags) {
    new (self) base::UnkVector<UnkElem50_7101cac938>(flags);
    self->Reserve(n);
}

// An owner of such an array at 0x10 whose flag follows it (at 0x38): reserves `n` elements and
// sets the flag (fn_7101059e5c, a copy of fn_7100c04134, which keeps its flag at 0x44).
struct UnkElem50Owner38 {
    u8 unk_0x0[0x10];
    base::UnkVector<UnkElem50_7101cac938> elems;
    bool field_0x38;
};

void fn_7101059e5c(UnkElem50Owner38* self, u32 n) {
    self->elems.Reserve(n);
    self->field_0x38 = true;
}
