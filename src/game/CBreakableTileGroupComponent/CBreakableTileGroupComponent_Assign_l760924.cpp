// CBreakableTileGroupComponent unit (unit 1167): TActorTileStatesMap's copy assignment (parallel arrays of
// 16-byte-element arrays and string-id keys; the rest of its container code is in
// CBreakableTileGroupComponent_Containers_ovb0028.cpp). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTileElem_ovb0028.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

using UnkTileElemVector_ovb0028 = base::global::CRntVector<UnkTileElem_ovb0028>;

struct UnkTileStatesMap_ovb0028 {
    UnkTileElemVector_ovb0028* values;  // 0x0
    CStrId* keys;                       // 0x8
    u32 count;                          // 0x10
    u32 capacity;                       // 0x14
    u32 step;                           // 0x18
    u32 flags;                          // 0x1c
    base::UnkAllocator* allocator;      // 0x20
};

template <class V>
struct UnkIdMap;
void fn_7100760774(UnkIdMap<UnkOwnedList>* self, u32 capacity);
// Releases the keys, frees the arrays and empties the map (a clone of fn_710027e494).
void fn_7100760ae4(UnkTileStatesMap_ovb0028* self);
void fn_7100760bf0(UnkTileStatesMap_ovb0028* self);

static inline void CopyConstructVector(UnkTileElemVector_ovb0028* dst, const UnkTileElemVector_ovb0028* src) {
    dst->data = nullptr;
    dst->count = 0;
    dst->capacity = 0;
    dst->step = src->step;
    dst->field_0x14 = src->field_0x14;
    dst->flags = src->flags;
    dst->allocator = src->allocator;
    dst->Reserve(src->capacity);
    __builtin_memcpy(dst->data, src->data, (u64)src->count * sizeof(UnkTileElem_ovb0028));
    dst->count = src->count;
}

// A new reference to a key, stored bytewise (no alias info: the original reloads the count after it).
static inline void CopyKey(CStrId* dst, const CStrId* src) {
    UnkStringData* d = src->data;
    dst->data = d;
    if (&g_unk_7101d47108->empty != d)
        ++d->ref_count;
}

// Copy assignment: cleared, shrunk and regrown to the other's count when the capacity differs, then copied.
UnkTileStatesMap_ovb0028* fn_7100760924(UnkTileStatesMap_ovb0028* self, const UnkTileStatesMap_ovb0028* other) {
    if (self == other)
        return self;
    fn_7100760ae4(self);
    if (other->count != self->capacity) {
        fn_7100760bf0(self);
        fn_7100760774(reinterpret_cast<UnkIdMap<UnkOwnedList>*>(self), other->count);
    }
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        CopyConstructVector(&self->values[i], &other->values[i]);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        CopyKey(&self->keys[i], &other->keys[i]);
    return self;
}
