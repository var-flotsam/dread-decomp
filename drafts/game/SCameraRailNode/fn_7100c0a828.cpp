// SCameraRailNode's unit (tools/tu_map.py): the element (0x50 bytes: vtable, an array, a flag, six words)
// at an index of an array of them, growing the array up to it with default elements when it is past the
// end: CRntVector::GrowAt written out. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/SCameraRailNode.h"
#include "types.h"

using base::UnkAllocator;

// As src/game/SCameraRailNode/fn_7100c073f0.cpp defines them.
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

// A default element as it is built in place (value-initialised, so the words are cleared): its vtable,
// an empty array with allocator flags 0xf and the flag set.
struct UnkElem50New_lc0a828 {
    void* vtable = g_unk_7101aa28d8 + 2;
    base::global::CRntVector<void*> field_0x8{0xf};
    bool field_0x30 = true;
    u32 field_0x34[6];
};
static_assert(sizeof(UnkElem50New_lc0a828) == 0x50, "UnkElem50New_lc0a828 layout");

struct UnkIndexedEntry;
struct UnkIndexedTable;

// Signature as src/game/SCameraRailNode/fn_7100c0a1d4.cpp declares it.
UnkIndexedEntry* fn_7100c0a828(UnkIndexedTable* table, u32 index) {
    auto* self = reinterpret_cast<base::global::CRntVector<UnkElem50>*>(table);
    if (self->field_0x14 && self->count <= index) {
        // First to the next multiple of the step, then to exactly index + 1 elements.
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0) {
            u32 grown = (index / self->step + 1) * self->step;
            if (cap < grown) {
                self->Reserve(grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            self->Reserve(n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkElem50New_lc0a828();
        self->count = n;
    }
    return reinterpret_cast<UnkIndexedEntry*>(&self->data[index]);
}
