// Unit 365 (navmesh::CNavMeshGenerator): the element at an index of a growable array of 0x48-byte
// elements (a list and an empty 2D box); an index past the end first reserves capacity (to the next
// multiple of the step) and then index + 1 elements and constructs the new ones. Names and layouts
// are placeholders.
#include <float.h>

#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

struct UnkElem_l295550 {
    CRntVector<u32> list{7};
    u32 field_0x28 = 0;
    bool field_0x2c = false;
    bool field_0x2d = true;
    f32 min[2] = {FLT_MAX, FLT_MAX};   // 0x30
    f32 max[2] = {-FLT_MAX, -FLT_MAX};  // 0x38
    u32 field_0x40 = 0;
    u8 pad_0x44[4];
};
static_assert(sizeof(UnkElem_l295550) == 0x48, "layout");

struct UnkArray_l295550 {
    UnkElem_l295550* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_710011f1ec(UnkArray_l295550* self, u32 capacity);

UnkElem_l295550* fn_7100295550(UnkArray_l295550* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        u32 capacity = self->capacity;
        if (capacity <= index && self->step != 0)
            capacity = (index / self->step + 1) * self->step;
        fn_710011f1ec(self, capacity);
        u32 n = index + 1;
        fn_710011f1ec(self, n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkElem_l295550;
        self->count = n;
    }
    return &self->data[index];
}
