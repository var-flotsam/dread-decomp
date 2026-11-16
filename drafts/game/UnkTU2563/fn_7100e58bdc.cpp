// CMinimapData's unit (2563): erases an entry of a map of record arrays keyed by string ids by moving the
// last entry (value and key) into its slot. Names and layouts are placeholders.
#include <new>

#include "base/UnkFreedVectorPair.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

typedef UnkSecondPtrArray<UnkFreedVectorPair> UnkPairRecords_le58bdc;

// Record arrays keyed by string ids, in two parallel arrays.
struct UnkPairRecordsMap_le58bdc {
    UnkPairRecords_le58bdc* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

UnkPairRecords_le58bdc* fn_7100e58bdc(UnkPairRecordsMap_le58bdc* self, UnkPairRecords_le58bdc* it) {
    u32 index = (u32)(it - self->values);
    fn_7100e493f4(&self->values[index]);
    self->keys[index].Reset();
    u32 last = self->count - 1;
    if (last != index) {
        new (&self->values[index]) UnkPairRecords_le58bdc(static_cast<UnkPairRecords_le58bdc&&>(self->values[last]));
        fn_7100e493f4(&self->values[self->count - 1]);
        // The key is moved: copied, and the last slot left holding the empty id.
        CStrId& moved = self->keys[self->count - 1];
        static_cast<base::global::UnkStrIdBase&>(self->keys[index]) = moved;
        moved.data = &g_unk_7101d47108->empty;
        self->keys[self->count - 1].Reset();
    }
    self->count = self->count - 1;
    return &self->values[index];
}
