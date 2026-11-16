// Unit 365 (navmesh::CNavMeshGenerator): the element at an index of an array of 0x30-byte records (a
// word and an array of their own), growing the array up to it with empty records: CRntVector::GrowAt.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkNavRecord_l2a07a4 {
    u64 field_0x0 = 0;
    base::global::CRntVector<u32> items{15};
};
static_assert(sizeof(UnkNavRecord_l2a07a4) == 0x30, "layout");

UnkNavRecord_l2a07a4* fn_71002a07a4(base::global::CRntVector<UnkNavRecord_l2a07a4>* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        // First to the next multiple of the step, then to exactly index + 1 records.
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0) {
            u32 grown = (index / self->step + 1) * self->step;
            if (cap < grown) {
                self->Reserve(grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            self->Reserve(n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkNavRecord_l2a07a4;
        self->count = n;
    }
    return &self->data[index];
}
