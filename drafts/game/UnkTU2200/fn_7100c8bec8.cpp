// CCutSceneDef's unit (tools/tu_map.py unit 2200): grows two parallel arrays (0x18-byte records of two
// string ids and a word, and a string id per record) that share a count, a capacity and allocation
// flags, moving the old elements over. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkAllocator;
using base::global::CStrId;

// The record's last word: two halves, copied as a whole.
struct UnkCutSceneRecordTail_lc8bec8 {
    u16 field_0x0;
    u16 field_0x2;
};

struct UnkCutSceneRecord_lc8bec8 {
    CStrId id;
    CStrId name;
    UnkCutSceneRecordTail_lc8bec8 field_0x10;
};
static_assert(sizeof(UnkCutSceneRecord_lc8bec8) == 0x18, "UnkCutSceneRecord_lc8bec8 size");

struct UnkCutSceneRecordArrays_lc8bec8 {
    UnkCutSceneRecord_lc8bec8* records;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    UnkAllocator* allocator;
};

// Takes the string `from` holds into an id slot that holds none yet, leaving `from` empty.
static inline void MoveInto(CStrId* slot, CStrId& from) {
    slot->data = from.data;
    from.data = &g_unk_7101d47108->empty;
}

void fn_7100c8bec8(UnkCutSceneRecordArrays_lc8bec8* self, u32 new_capacity) {
    if (self->capacity < new_capacity) {
        UnkCutSceneRecord_lc8bec8* records = static_cast<UnkCutSceneRecord_lc8bec8*>(self->allocator->vf_0x40(
            (u64)new_capacity * sizeof(UnkCutSceneRecord_lc8bec8), 8, self->flags));
        CStrId* keys = static_cast<CStrId*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(CStrId), 8, self->flags));
        if (UnkCutSceneRecord_lc8bec8* old = self->records) {
            UnkCutSceneRecord_lc8bec8* dst = records;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
                MoveInto(&dst->id, old->id);
                MoveInto(&dst->name, old->name);
                dst->field_0x10 = old->field_0x10;
                old->~UnkCutSceneRecord_lc8bec8();
            }
            self->allocator->vf_0x48(self->records);
        }
        if (CStrId* old = self->keys) {
            CStrId* dst = keys;
            for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
                CStrId::MoveConstructReloading(dst, *old);
                old->Reset();
            }
            self->allocator->vf_0x48(self->keys);
        }
        self->records = records;
        self->keys = keys;
        self->capacity = new_capacity;
    }
}
