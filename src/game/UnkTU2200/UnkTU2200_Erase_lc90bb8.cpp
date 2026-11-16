// CCutSceneDef's unit (2200): erases a record (two string ids and a value) from an array with a string id
// per record alongside, by moving the last record and id into its place. Returns the erased record's place.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkIdPairRecord_lc90bb8 {
    CStrId first;
    CStrId second;
    u32 field_0x10;

    // Takes over the other's ids; the other is left with empty ones.
    void MoveFrom(UnkIdPairRecord_lc90bb8& other) {
        CStrId::MoveConstructReloading(&first, other.first);
        CStrId::MoveConstructReloading(&second, other.second);
        field_0x10 = other.field_0x10;
    }
};
static_assert(sizeof(UnkIdPairRecord_lc90bb8) == 0x18, "");

struct UnkIdPairArrays_lc90bb8 {
    UnkIdPairRecord_lc90bb8* records;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

UnkIdPairRecord_lc90bb8* fn_7100c90bb8(UnkIdPairArrays_lc90bb8* self, UnkIdPairRecord_lc90bb8* it) {
    u32 index = static_cast<u32>(it - self->records);
    self->records[index].~UnkIdPairRecord_lc90bb8();
    self->ids[index].Reset();
    if (self->count - 1 != index) {
        self->records[index].MoveFrom(self->records[self->count - 1]);
        self->records[self->count - 1].~UnkIdPairRecord_lc90bb8();
        CStrId::MoveConstructReloading(&self->ids[index], self->ids[self->count - 1]);
        self->ids[self->count - 1].Reset();
    }
    --self->count;
    return &self->records[index];
}
