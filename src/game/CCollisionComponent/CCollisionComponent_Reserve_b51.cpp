// CCollisionComponent (unit 1319): growing a paired array of 0x44-byte entries (a count and up to
// eight 8-byte items) and their 4-byte keys. Names and layouts are placeholders.
#include <cstring>

#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"

struct UnkCollisionEntry {
    u32 count;
    u8 items[0x40];
};

// count: 0x10
// capacity: 0x14
// flags: 0x1c
// allocator: 0x20
using UnkCollisionPairArray = base::global::CRntSmallDictionary<u32, UnkCollisionEntry>;

void fn_7100866c74(UnkCollisionPairArray* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    UnkCollisionEntry* entries =
        static_cast<UnkCollisionEntry*>(self->allocator->vf_0x40((u64)capacity * sizeof(UnkCollisionEntry), 8, self->flags));
    u32* keys = static_cast<u32*>(self->allocator->vf_0x40((u64)capacity * sizeof(u32), 8, self->flags));
    if (UnkCollisionEntry* old = self->values) {
        UnkCollisionEntry* dst = entries;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
            memcpy(dst->items, old->items, (u64)old->count * 8);
            dst->count = old->count;
            old->count = 0;
        }
        self->allocator->vf_0x48(self->values);
    }
    if (self->keys) {
        memcpy(keys, self->keys, (u64)self->count * 4);
        self->allocator->vf_0x48(self->keys);
    }
    self->values = entries;
    self->keys = keys;
    self->capacity = capacity;
}
