// CAnimationNavMeshItemStage's unit (tools/tu_map.py unit 1474): the stage's teardown: back to its own
// vtable (g_unk_7101a88a20), every id in its inline table (+0x4c entries from +0x50) and its name (+0x40)
// released to the empty id, the table emptied, and the buffer at +0x10 handed back to its allocator
// (+0x30). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a88a20[];

struct UnkNavMeshStageEntry_ovb0122 {
    CStrId id;
    u8 pad_0x8[0x8];
};

struct UnkNavMeshItemStage_ovb0122 {
    void** vtable;
    u8 pad_0x8[0x8];
    void* data;     // 0x10
    u32 count;      // 0x18
    u32 capacity;   // 0x1c
    u8 pad_0x20[0x10];
    base::UnkAllocator* allocator;  // 0x30
    u8 pad_0x38[0x8];
    CStrId name;  // 0x40
    u8 pad_0x48[0x4];
    u32 entry_count;                          // 0x4c
    UnkNavMeshStageEntry_ovb0122 entries[1];  // 0x50
};

void fn_71009b8d70(UnkNavMeshItemStage_ovb0122* self) {
    self->vtable = g_unk_7101a88a20 + 2;
    for (u64 i = 0; i < self->entry_count; ++i)
        self->entries[i].id.Reset();
    self->entry_count = 0;
    self->name.Reset();
    if (void* data = self->data) {
        self->count = 0;
        self->allocator->vf_0x48(data);
        self->capacity = 0;
    }
}
