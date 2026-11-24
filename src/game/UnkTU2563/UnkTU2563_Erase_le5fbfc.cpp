// CMinimapData's unit (2563): erasing a record from an array of 0xa8-byte records (three arrays and their
// bounds) with a string id per record alongside, by moving the last record and id into its place. Names and
// layouts are placeholders.
#include <new>

#include "base/UnkFreedVectorPair.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// Three arrays with a bounding box; a moved record gets a fresh, empty box marked dirty.
struct UnkTripleArrayRecord_le5fbfc {
    UnkFreedVector<u32> first;
    u32 field_0x28;
    u8 field_0x2c;
    bool dirty = true;
    f32 min[2] = {__FLT_MAX__, __FLT_MAX__};
    f32 max[2] = {-__FLT_MAX__, -__FLT_MAX__};
    u32 field_0x40;
    UnkFreedVector<u32> second;
    UnkFreedVector<u32> third;
    u32 field_0x98;
    u32 field_0x9c;
    u32 field_0xa0;
    u32 field_0xa4;

    UnkTripleArrayRecord_le5fbfc(UnkTripleArrayRecord_le5fbfc&& other)
        : first(static_cast<UnkFreedVector<u32>&&>(other.first)), field_0x28(other.field_0x28),
          field_0x2c(other.field_0x2c), field_0x40(other.field_0x40),
          second(static_cast<UnkFreedVector<u32>&&>(other.second)),
          third(static_cast<UnkFreedVector<u32>&&>(other.third)), field_0x98(other.field_0x98),
          field_0x9c(other.field_0x9c), field_0xa0(other.field_0xa0), field_0xa4(other.field_0xa4) {}
};
static_assert(sizeof(UnkTripleArrayRecord_le5fbfc) == 0xa8, "UnkTripleArrayRecord_le5fbfc layout");

// Records with their ids alongside in a second array.
struct UnkTripleArrayIdArray_le5fbfc {
    UnkTripleArrayRecord_le5fbfc* records;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::IAllocator* allocator;
};

// Erases the record `it` points at: the last record and id take its place. Returns the erased record's place.
UnkTripleArrayRecord_le5fbfc* fn_7100e5fbfc(char* array, void* it) {
    UnkTripleArrayIdArray_le5fbfc* self = reinterpret_cast<UnkTripleArrayIdArray_le5fbfc*>(array);
    u32 index = static_cast<u32>(static_cast<UnkTripleArrayRecord_le5fbfc*>(it) - self->records);
    self->records[index].~UnkTripleArrayRecord_le5fbfc();
    self->ids[index].Reset();
    if (self->count - 1 != index) {
        new (&self->records[index]) UnkTripleArrayRecord_le5fbfc(
            static_cast<UnkTripleArrayRecord_le5fbfc&&>(self->records[self->count - 1]));
        self->records[self->count - 1].~UnkTripleArrayRecord_le5fbfc();
        CStrId::MoveConstructReloading(&self->ids[index], self->ids[self->count - 1]);
        self->ids[self->count - 1].Reset();
    }
    --self->count;
    return &self->records[index];
}
