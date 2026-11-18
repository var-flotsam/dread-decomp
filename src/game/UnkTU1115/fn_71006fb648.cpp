// Unit 1115: growing a paired array of 0x30-byte entries (each a list of lists) and their 4-byte
// keys; the entries are moved over and the old ones freed. Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

struct UnkList_l6fb648 {
    CRntVector<u8> items;

    ~UnkList_l6fb648() { items.Free(); }
};
static_assert(sizeof(UnkList_l6fb648) == 0x28, "");

// Two halves of a word, copied together.
struct UnkHalves_l6fb648 {
    u16 field_0x0;
    u16 field_0x2;
};

struct UnkEntry_l6fb648 {
    CRntVector<UnkList_l6fb648> lists;
    UnkHalves_l6fb648 field_0x28;

    UnkEntry_l6fb648(UnkEntry_l6fb648&& other)
        : lists(static_cast<CRntVector<UnkList_l6fb648>&&>(other.lists)), field_0x28(other.field_0x28) {}
    ~UnkEntry_l6fb648() { lists.Free(); }
};
static_assert(sizeof(UnkEntry_l6fb648) == 0x30, "");

// count: 0x10
// capacity: 0x14
// flags: 0x1c
// allocator: 0x20
using UnkEntryPairArray_l6fb648 = base::global::CRntSmallDictionary<u32, UnkEntry_l6fb648>;

void fn_71006fb648(UnkEntryPairArray_l6fb648* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    UnkEntry_l6fb648* entries = static_cast<UnkEntry_l6fb648*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkEntry_l6fb648), 8, self->flags));
    u32* keys = static_cast<u32*>(self->allocator->vf_0x40((u64)capacity * sizeof(u32), 8, self->flags));
    if (UnkEntry_l6fb648* old = self->values) {
        UnkEntry_l6fb648* dst = entries;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
            new (dst) UnkEntry_l6fb648(static_cast<UnkEntry_l6fb648&&>(*old));
            old->~UnkEntry_l6fb648();
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
