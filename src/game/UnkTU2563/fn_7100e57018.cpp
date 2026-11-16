// The CMinimapData unit (unit 2563): erasing a record from an array of array-pair records with a string id per
// record alongside, by moving the last record and id into its place. Names and layouts are placeholders.
#include <new>

#include "base/UnkFreedVectorPair.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

namespace {

// Records with their ids alongside in a second array.
struct UnkPairIdArray {
    UnkFreedVectorPair* records;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

}  // namespace

// Erases the record `it` points at: the last record and id take its place. Returns the erased record's place.
UnkFreedVectorPair* fn_7100e57018(char* array, void* it) {
    UnkPairIdArray* self = reinterpret_cast<UnkPairIdArray*>(array);
    u32 index = static_cast<u32>(static_cast<UnkFreedVectorPair*>(it) - self->records);
    self->records[index].~UnkFreedVectorPair();
    self->ids[index].Reset();
    if (self->count - 1 != index) {
        new (&self->records[index])
            UnkFreedVectorPair(static_cast<UnkFreedVectorPair&&>(self->records[self->count - 1]));
        self->records[self->count - 1].~UnkFreedVectorPair();
        CStrId::MoveConstructReloading(&self->ids[index], self->ids[self->count - 1]);
        self->ids[self->count - 1].Reset();
    }
    --self->count;
    return &self->records[index];
}
