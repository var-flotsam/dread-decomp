// CNavMeshItemStage's unit (tools/tu_map.py unit 1483): appends an id to the stage's entries (fn_71009c5ab8:
// the id, taken through a copy of the empty id, goes into the next 16-byte entry at +0x50 with its flag
// cleared, and the count at +0x4c grows; the flag is cleared through a char store, which makes the
// string pool reload as the original does). Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkNavMeshStageEntry_ovb0090 {
    CStrId id;
    bool field_0x8;
};

struct UnkNavMeshItemStageIds_ovb0090 {
    u8 pad_0x0[0x4c];
    u32 count;  // 0x4c
    u8 entries[0x10];  // 0x50, UnkNavMeshStageEntry_ovb0090 elements
};

void fn_71009c5ab8(UnkNavMeshItemStageIds_ovb0090* self, const CStrId* name) {
    CStrId id(g_unk_7101d470e0);
    id = *name;
    UnkNavMeshStageEntry_ovb0090* entry = reinterpret_cast<UnkNavMeshStageEntry_ovb0090*>(self->entries) + self->count;
    new (&entry->id) CStrId(id);
    *reinterpret_cast<char*>(&entry->field_0x8) = 0;
    ++self->count;
}
