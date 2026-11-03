// Unit 2530 of tools/tu_map.py (no reflected class): appends a counted reference to an engine array of
// counted references (base::UnkVector's layout): grows the capacity to the next multiple of the
// step when it is full (hanging when there is no room, as base::UnkVector::PushBack does), has
// fn_7100da2900 make room, then stores the reference in a cleared slot and counts it. Names are
// placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkRefPtrArray_b46a3 {
    UnkRefObject** data;
    // A union member: the original reloads the count after the slot stores.
    union {
        u32 count;
    };
    u32 capacity;
    u32 step;
};

void fn_7100da2900(UnkRefPtrArray_b46a3* self, u32 capacity);

void fn_7100da27f4(UnkRefPtrArray_b46a3* self, UnkRefObject* const* value) {
    u32 n = self->count;
    u32 capacity = self->capacity;
    if (capacity <= n && self->step != 0)
        capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    fn_7100da2900(self, capacity);
    UnkRefObject** slot = &self->data[self->count];
    *slot = nullptr;
    if (UnkRefObject* p = *value) {
        ++p->ref_count;
        if (UnkRefObject* old = *slot) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        *slot = p;
    }
    ++self->count;
}
