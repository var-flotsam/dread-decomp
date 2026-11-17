// Unit 2566 (CMinimapManager tunables): grows a pair of parallel arrays (0x90-byte records and
// their string ids) to a new capacity, moving the old elements into the new storage.
// Names and layouts are placeholders.

#include <new>

#include "base/global/CRntSmallDictionary.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A string id that can be moved: takes the other's string and leaves it the empty one.
struct UnkMovableId {
    base::global::UnkStringData* data;

    UnkMovableId(UnkMovableId&& other) {
        CStrId::MoveConstructReloading(reinterpret_cast<CStrId*>(this), reinterpret_cast<CStrId&>(other));
    }
    ~UnkMovableId() { reinterpret_cast<CStrId*>(this)->Reset(); }
};

// An inner array: storage, a second block, count and capacity, and its allocator.
template <typename T>
struct UnkArray {
    T* data;
    void* field_0x8;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 field_0x1c;
    base::UnkAllocator* allocator;

    UnkArray(UnkArray&& other)
        : data(other.data), field_0x8(other.field_0x8), count(other.count),
          capacity(other.capacity), field_0x18(other.field_0x18), field_0x1c(other.field_0x1c),
          allocator(other.allocator) {
        other.count = 0;
        other.data = nullptr;
        other.field_0x8 = nullptr;
    }
    ~UnkArray() {
        if (data && count) {
            for (u64 i = 0; i < count; ++i)
                data[i].~T();
        }
        count = 0;
        if (field_0x8)
            allocator->vf_0x48(field_0x8);
        if (data)
            allocator->vf_0x48(data);
    }
};

struct UnkElem {
    u8 pad_0x0[0x50];
};

// A 0x50-byte record: an id, an array and eight words. Its destructor is out of line.
struct UnkRecord {
    UnkMovableId id;
    UnkArray<UnkElem> items;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;

    UnkRecord(UnkRecord&& other)
        : id(static_cast<UnkMovableId&&>(other.id)), items(static_cast<UnkArray<UnkElem>&&>(other.items)),
          field_0x30(other.field_0x30), field_0x34(other.field_0x34), field_0x38(other.field_0x38),
          field_0x3c(other.field_0x3c), field_0x40(other.field_0x40), field_0x44(other.field_0x44),
          field_0x48(other.field_0x48), field_0x4c(other.field_0x4c) {}
    ~UnkRecord() asm("fn_7100ea9eb8");
};

struct UnkEntry {
    UnkArray<UnkRecord> records;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    UnkRecord main;
    u8 field_0x88;

    UnkEntry(UnkEntry&& other)
        : records(static_cast<UnkArray<UnkRecord>&&>(other.records)), field_0x28(other.field_0x28),
          field_0x2c(other.field_0x2c), field_0x30(other.field_0x30), field_0x34(other.field_0x34),
          main(static_cast<UnkRecord&&>(other.main)), field_0x88(other.field_0x88) {}
};
static_assert(sizeof(UnkEntry) == 0x90, "");

using UnkEntryTable = base::global::CRntSmallDictionary<UnkMovableId, UnkEntry>;

void fn_7100e97c60(UnkEntryTable* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    UnkEntry* entries = static_cast<UnkEntry*>(
        self->allocator->vf_0x40(static_cast<u64>(capacity) * sizeof(UnkEntry), 8, self->flags));
    UnkMovableId* ids = static_cast<UnkMovableId*>(
        self->allocator->vf_0x40(static_cast<u64>(capacity) * sizeof(UnkMovableId), 8, self->flags));
    if (self->values) {
        UnkEntry* src = self->values;
        UnkEntry* dst = entries;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkEntry(static_cast<UnkEntry&&>(*src));
            src->~UnkEntry();
        }
        self->allocator->vf_0x48(self->values);
    }
    if (self->keys) {
        UnkMovableId* src = self->keys;
        UnkMovableId* dst = ids;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            new (dst) UnkMovableId(static_cast<UnkMovableId&&>(*src));
            src->~UnkMovableId();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = entries;
    self->keys = ids;
    self->capacity = capacity;
}
