// Unit 365 (navmesh::CNavMeshGenerator): the element at an index of a growable array of 0x60-byte
// elements; an index past the end first reserves capacity (to the next multiple of the step) and
// then index + 1 elements and constructs the new ones. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

struct UnkElem_l29c39c {
    u64 id = 0xffffffff;
    CRntVector<u32> list_a{0xf};
    CRntVector<u32> list_b{0xf};
    bool field_0x58 = false;
};
static_assert(sizeof(UnkElem_l29c39c) == 0x60, "layout");

void fn_71002a9960(base::global::CRntVector<UnkElem_l29c39c>* self, u32 capacity);

UnkElem_l29c39c* fn_710029c39c(base::global::CRntVector<UnkElem_l29c39c>* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        u32 capacity = self->capacity;
        if (capacity <= index && self->step != 0)
            capacity = (index / self->step + 1) * self->step;
        fn_71002a9960(self, capacity);
        u32 n = index + 1;
        fn_71002a9960(self, n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkElem_l29c39c();
        self->count = n;
    }
    return &self->data[index];
}
