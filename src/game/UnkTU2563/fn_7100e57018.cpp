// The CMinimapData unit (unit 2563): erasing a record from an array of array-pair records with a string id per
// record alongside, by moving the last record and id into its place. Names and layouts are placeholders.
#include <new>

#include "base/UnkFreedVectorPair.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

namespace {

// Records with their ids alongside in a second array.
using UnkPairIdArray = base::global::CRntSmallDictionary<CStrId, UnkFreedVectorPair>;

}  // namespace

// Erases the record `it` points at: the last record and id take its place. Returns the erased record's place.
UnkFreedVectorPair* fn_7100e57018(char* array, void* it) {
    UnkPairIdArray* self = reinterpret_cast<UnkPairIdArray*>(array);
    u32 index = static_cast<u32>(static_cast<UnkFreedVectorPair*>(it) - self->values);
    self->values[index].~UnkFreedVectorPair();
    self->keys[index].Reset();
    if (self->count - 1 != index) {
        new (&self->values[index])
            UnkFreedVectorPair(static_cast<UnkFreedVectorPair&&>(self->values[self->count - 1]));
        self->values[self->count - 1].~UnkFreedVectorPair();
        CStrId::MoveConstructReloading(&self->keys[index], self->keys[self->count - 1]);
        self->keys[self->count - 1].Reset();
    }
    --self->count;
    return &self->values[index];
}
