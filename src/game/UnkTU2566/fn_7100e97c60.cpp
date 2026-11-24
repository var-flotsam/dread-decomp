// Unit 2566 (CMinimapManager tunables): grows the minimap's cell list, a pair of parallel arrays (the
// 0x90-byte cells and their string ids), to a new capacity, moving the old elements into the new storage.
// The cell is the one fn_7100e97f8c (src/game/UnkTU2566/UnkTU2566_Minimap_ovb0199.cpp) appends and
// fn_7100eb3648 (UnkTU2566_Minimap_ovb0200.cpp) clears. Names and layouts are placeholders.

#include <new>

#include "base/global/CStrId.h"
#include "game/UnkMinimapRecord.h"
#include "game/UnkTU2566.h"
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

// A cell's records: a small dictionary (records and their 4-byte keys) that destroys its records and frees
// both arrays when it is destroyed.
struct UnkMinimapRecords {
    UnkMinimapRecord* data;
    u32* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 field_0x1c;
    base::IAllocator* allocator;

    UnkMinimapRecords(UnkMinimapRecords&& other)
        : data(other.data), keys(other.keys), count(other.count), capacity(other.capacity),
          field_0x18(other.field_0x18), field_0x1c(other.field_0x1c), allocator(other.allocator) {
        other.count = 0;
        other.data = nullptr;
        other.keys = nullptr;
    }
    ~UnkMinimapRecords() {
        if (data && count) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkMinimapRecord();
        }
        count = 0;
        if (keys)
            allocator->vf_0x48(keys);
        if (data)
            allocator->vf_0x48(data);
    }
};

struct UnkMinimapRecordCell {
    UnkMinimapRecords records;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    UnkMinimapRecord main;
    u8 field_0x88;

    UnkMinimapRecordCell(UnkMinimapRecordCell&& other)
        : records(static_cast<UnkMinimapRecords&&>(other.records)), field_0x28(other.field_0x28),
          field_0x2c(other.field_0x2c), field_0x30(other.field_0x30), field_0x34(other.field_0x34),
          main(static_cast<UnkMinimapRecord&&>(other.main)), field_0x88(other.field_0x88) {}
};
static_assert(sizeof(UnkMinimapRecordCell) == 0x90, "UnkMinimapRecordCell layout");

struct UnkMinimapRecordCellList {
    UnkMinimapRecordCell* cells;
    UnkMovableId* ids;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 field_0x1c;
    base::IAllocator* allocator;
};

// The caller (fn_7100e97f8c) has its own view of the list, so the declaration (game/UnkTU2566.h) takes it
// untyped.
void fn_7100e97c60(void* self_, u32 capacity) {
    UnkMinimapRecordCellList* self = static_cast<UnkMinimapRecordCellList*>(self_);
    if (self->capacity >= capacity)
        return;
    UnkMinimapRecordCell* cells = static_cast<UnkMinimapRecordCell*>(
        self->allocator->vf_0x40(static_cast<u64>(capacity) * sizeof(UnkMinimapRecordCell), 8, self->field_0x1c));
    UnkMovableId* ids = static_cast<UnkMovableId*>(
        self->allocator->vf_0x40(static_cast<u64>(capacity) * sizeof(UnkMovableId), 8, self->field_0x1c));
    if (self->cells) {
        UnkMinimapRecordCell* src = self->cells;
        UnkMinimapRecordCell* dst = cells;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkMinimapRecordCell(static_cast<UnkMinimapRecordCell&&>(*src));
            src->~UnkMinimapRecordCell();
        }
        self->allocator->vf_0x48(self->cells);
    }
    if (self->ids) {
        UnkMovableId* src = self->ids;
        UnkMovableId* dst = ids;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            new (dst) UnkMovableId(static_cast<UnkMovableId&&>(*src));
            src->~UnkMovableId();
        }
        self->allocator->vf_0x48(self->ids);
    }
    self->cells = cells;
    self->ids = ids;
    self->capacity = capacity;
}
