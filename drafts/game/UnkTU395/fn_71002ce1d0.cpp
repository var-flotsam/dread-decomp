// Unit 395: the element at an index of a growable array of 0x48-byte elements (a list, a string id and
// three words); an index past the end first reserves capacity (to the next multiple of the step) and then
// index + 1 elements and constructs the new ones. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkTail_l2ce1d0 {
    f32 min;
    f32 max;
    u64 field_0x8;
    u64 field_0x10;
    UnkTail_l2ce1d0() : min(-1.0f), max(-1.0f), field_0x8(0), field_0x10(0) {}
};

struct UnkElem_l2ce1d0 {
    base::global::CRntVector<void*> list;  // 0x0
    base::global::UnkStringData* id;       // 0x28
    UnkTail_l2ce1d0 field_0x30;

    // Takes a reference to the id with an int-typed increment.
    void RefId() {
        if (&g_unk_7101d47108->empty != id) {
            s32* count = &id->ref_count;
            ++*count;
        }
    }

    UnkElem_l2ce1d0() : list(0xf), id(g_unk_7101d470e0.data) {
        RefId();
    }
};
static_assert(sizeof(UnkElem_l2ce1d0) == 0x48, "layout");

void fn_71002cd854(base::global::CRntVector<UnkElem_l2ce1d0>* self, u32 capacity);

UnkElem_l2ce1d0* fn_71002ce1d0(base::global::CRntVector<UnkElem_l2ce1d0>* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        u32 capacity = self->capacity;
        if (capacity <= index && self->step != 0)
            capacity = (index / self->step + 1) * self->step;
        fn_71002cd854(self, capacity);
        u32 n = index + 1;
        fn_71002cd854(self, n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkElem_l2ce1d0();
        self->count = n;
    }
    return &self->data[index];
}
