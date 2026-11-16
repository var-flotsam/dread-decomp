// Unit 1115 (tools/tu_map.py, no class name): erasing a record from the registry's keyed table (records
// with a u32 key per record alongside) by moving the last record and key into its place. Names and
// layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

// An inner array of plain values, freed with its owner.
struct UnkIdList_l6fb820 {
    CRntVector<u32> ids;

    ~UnkIdList_l6fb820() { ids.Free(); }
};

// A record of the table: an array of arrays and a word.
struct UnkRecord_l6fb820 {
    CRntVector<UnkIdList_l6fb820> lists;  // 0x0
    u32 field_0x28;                       // 0x28

    UnkRecord_l6fb820(UnkRecord_l6fb820&& other)
        : lists(static_cast<CRntVector<UnkIdList_l6fb820>&&>(other.lists)), field_0x28(other.field_0x28) {}
    ~UnkRecord_l6fb820() { lists.Free(); }
};
static_assert(sizeof(UnkRecord_l6fb820) == 0x30, "layout");

// Records with their keys alongside in a second array.
struct UnkRecordTable_l6fb820 {
    UnkRecord_l6fb820* records;  // 0x0
    u32* keys;                   // 0x8
    u32 count;                   // 0x10
};

// Erases the record `it` points at: the last record and key take its place. Returns the erased record's place.
UnkRecord_l6fb820* fn_71006fb820(UnkRecordTable_l6fb820* self, UnkRecord_l6fb820* it) {
    u32 index = static_cast<u32>(it - self->records);
    self->records[index].~UnkRecord_l6fb820();
    if (self->count - 1 != index) {
        new (&self->records[index])
            UnkRecord_l6fb820(static_cast<UnkRecord_l6fb820&&>(self->records[self->count - 1]));
        self->records[self->count - 1].~UnkRecord_l6fb820();
        self->keys[index] = self->keys[self->count - 1];
    }
    --self->count;
    return &self->records[index];
}
