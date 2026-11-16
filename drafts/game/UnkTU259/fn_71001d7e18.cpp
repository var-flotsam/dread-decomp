// Unit 259 (tools/tu_map.py): two parallel arrays (0x10-byte records that start with a counted
// resource reference, and a string id per record) sharing a count, a capacity and allocation flags.
// Grows both to a new capacity. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A counted reference to a resource; assigning one takes the new reference before dropping the old.
struct UnkResHandle_l1d7e18 {
    UnkCountedResource* ptr;

    UnkResHandle_l1d7e18() : ptr(nullptr) {}
    UnkResHandle_l1d7e18& operator=(const UnkResHandle_l1d7e18& other) {
        UnkCountedResource* p = other.ptr;
        if (p)
            ++p->ref_count;
        UnkCountedResource* old = ptr;
        if (old && --old->ref_count == 0)
            fn_710001737c(old);
        ptr = p;
        return *this;
    }
    ~UnkResHandle_l1d7e18() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

struct UnkResRecord_l1d7e18 {
    UnkResHandle_l1d7e18 ref;
    void* field_0x8;

    UnkResRecord_l1d7e18(const UnkResRecord_l1d7e18& other) {
        ref = other.ref;
        field_0x8 = other.field_0x8;
    }
};
static_assert(sizeof(UnkResRecord_l1d7e18) == 0x10, "UnkResRecord_l1d7e18 layout");

using UnkResRecordArrays_l1d7e18 = base::global::CRntSmallDictionary<CStrId, UnkResRecord_l1d7e18>;

// Grows both arrays to `new_capacity`: the records are copied and the old ones destroyed, the
// names moved.
void fn_71001d7e18(UnkResRecordArrays_l1d7e18* self, u32 new_capacity) {
    if (self->capacity >= new_capacity)
        return;
    UnkResRecord_l1d7e18* records = static_cast<UnkResRecord_l1d7e18*>(
        self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkResRecord_l1d7e18), 8, self->flags));
    CStrId* names = static_cast<CStrId*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(CStrId), 8, self->flags));
    if (UnkResRecord_l1d7e18* old = self->values) {
        UnkResRecord_l1d7e18* dst = records;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
            new (dst) UnkResRecord_l1d7e18(*old);
            old->~UnkResRecord_l1d7e18();
        }
        self->allocator->vf_0x48(self->values);
    }
    if (CStrId* old = self->keys) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
            names[j].data = old->data;
            old->data = &g_unk_7101d47108->empty;
            old->Reset();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = records;
    self->keys = names;
    self->capacity = new_capacity;
}
