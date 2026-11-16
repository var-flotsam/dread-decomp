// Minimap manager unit (UnkTU2566): grows the keyed cell list (0x90-byte cells and their string-id
// keys, appended to by fn_7100e97f8c) to a new capacity, moving the old elements over.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkMmNamed_b0199;
void fn_7100ea9eb8(UnkMmNamed_b0199* self);

// The cell types are this unit's own.
namespace {

// An array that owns its storage: elements, a second block, count, capacity, growth step,
// allocation flags and the allocator. Moving it leaves the other empty.
template <typename T>
struct UnkMmArray_le97c60 {
    T* data;
    void* extra;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkMmArray_le97c60(UnkMmArray_le97c60&& other)
        : data(other.data), extra(other.extra), count(other.count), capacity(other.capacity), step(other.step),
          flags(other.flags), allocator(other.allocator) {
        other.count = 0;
        other.data = nullptr;
        other.extra = nullptr;
    }
    ~UnkMmArray_le97c60() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~T();
        }
        count = 0;
        if (extra)
            allocator->vf_0x48(extra);
        if (data)
            allocator->vf_0x48(data);
    }
};

// A string id that is moved, leaving the other the empty id.
struct UnkMmMovedId_le97c60 {
    base::global::UnkStringData* data;

    UnkMmMovedId_le97c60(UnkMmMovedId_le97c60&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

struct UnkMmItem_le97c60 {
    u8 pad_0x0[0x20];
};

// A named record's items: the same fields as UnkMmArray_le97c60, released by the record's
// destructor (fn_7100ea9eb8) rather than by a destructor of their own.
struct UnkMmItems_le97c60 {
    UnkMmItem_le97c60* data;
    void* extra;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkMmItems_le97c60(UnkMmItems_le97c60&& other)
        : data(other.data), extra(other.extra), count(other.count), capacity(other.capacity), step(other.step),
          flags(other.flags), allocator(other.allocator) {
        other.count = 0;
        other.data = nullptr;
        other.extra = nullptr;
    }
};

// A named record (destroyed by fn_7100ea9eb8): an id, its items and eight words.
struct UnkMmNamed_le97c60 {
    UnkMmMovedId_le97c60 name;
    UnkMmItems_le97c60 items;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;

    UnkMmNamed_le97c60(UnkMmNamed_le97c60&& other)
        : name(static_cast<UnkMmMovedId_le97c60&&>(other.name)),
          items(static_cast<UnkMmItems_le97c60&&>(other.items)), field_0x30(other.field_0x30),
          field_0x34(other.field_0x34), field_0x38(other.field_0x38), field_0x3c(other.field_0x3c),
          field_0x40(other.field_0x40), field_0x44(other.field_0x44), field_0x48(other.field_0x48),
          field_0x4c(other.field_0x4c) {}
    ~UnkMmNamed_le97c60() { fn_7100ea9eb8(reinterpret_cast<UnkMmNamed_b0199*>(this)); }
};
static_assert(sizeof(UnkMmNamed_le97c60) == 0x50, "");

// A cell of the list: its named records, four words, one named record of its own and a flag.
struct UnkMmCell_le97c60 {
    UnkMmArray_le97c60<UnkMmNamed_le97c60> records;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    UnkMmNamed_le97c60 sub;
    u8 field_0x88;

    UnkMmCell_le97c60(UnkMmCell_le97c60&& other)
        : records(static_cast<UnkMmArray_le97c60<UnkMmNamed_le97c60>&&>(other.records)),
          field_0x28(other.field_0x28), field_0x2c(other.field_0x2c), field_0x30(other.field_0x30),
          field_0x34(other.field_0x34), sub(static_cast<UnkMmNamed_le97c60&&>(other.sub)),
          field_0x88(other.field_0x88) {}
};
static_assert(sizeof(UnkMmCell_le97c60) == 0x90, "");

struct UnkMmCellList_le97c60 {
    UnkMmCell_le97c60* cells;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

}  // namespace

void fn_7100e97c60(void* self_, u32 new_capacity) {
    auto* self = static_cast<UnkMmCellList_le97c60*>(self_);
    if (self->capacity < new_capacity) {
        UnkMmCell_le97c60* cells = static_cast<UnkMmCell_le97c60*>(
            self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkMmCell_le97c60), 8, self->flags));
        CStrId* keys = static_cast<CStrId*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(CStrId), 8, self->flags));
        if (UnkMmCell_le97c60* old = self->cells) {
            UnkMmCell_le97c60* dst = cells;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
                new (dst) UnkMmCell_le97c60(static_cast<UnkMmCell_le97c60&&>(*old));
                old->~UnkMmCell_le97c60();
            }
            self->allocator->vf_0x48(self->cells);
        }
        if (CStrId* old = self->keys) {
            CStrId* dst = keys;
            for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
                dst->data = old->data;
                old->data = &g_unk_7101d47108->empty;
                old->Reset();
            }
            self->allocator->vf_0x48(self->keys);
        }
        self->cells = cells;
        self->keys = keys;
        self->capacity = new_capacity;
    }
}
