// CCutSceneDef's unit (tools/tu_map.py): the copy constructor and copy assignment of the unit's array of 0xa0-byte
// elements (base::UnkVector's layout; src/game/CCutSceneDef/CCutSceneDef_b42.cpp has its reserve and
// element assignments): reset, copy the step, flags and allocator, reserve the other's capacity
// (fn_7100c8b9c8), then append a copy of each element (fn_7100c8bba4) with the push-back inlined: the
// capacity grows to the next multiple of the step, and a count that can't grow hangs, as in
// base::UnkVector::PushBack. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU2200.h"
#include "types.h"

#include <new>

// As src/game/CCutSceneDef/CCutSceneDef_b42.cpp declares them.
struct UnkElemA0_b42a3 {
    u8 unk_0x0[0xa0];
};

struct UnkElemA0Array_b42a3 {
    UnkElemA0_b42a3* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100c8bba4(UnkElemA0_b42a3* self, const UnkElemA0_b42a3* other);  // the element's copy constructor

// Appends a copy of each of the other's elements after reserving its capacity.
static inline __attribute__((always_inline)) void AppendAll_b43a3(UnkElemA0Array_b42a3* self,
                                                                  const UnkElemA0Array_b42a3* other) {
    fn_7100c8b9c8(self, other->capacity);
    for (const UnkElemA0_b42a3 *it = other->data, *end = it + other->count; it != end; ++it) {
        u32 n = self->count;
        u32 capacity = self->capacity;
        if (capacity <= n && self->step != 0)
            capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)capacity)
            for (;;) {
            }
        fn_7100c8b9c8(self, capacity);
        fn_7100c8bba4(&self->data[self->count], it);
        ++self->count;
    }
}

void fn_7100c8b688(UnkElemA0Array_b42a3* self, const UnkElemA0Array_b42a3* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    AppendAll_b43a3(self, other);
}

// The copy assignment: the old elements destroyed one by one, then the other's appended.
UnkElemA0Array_b42a3* fn_7100c8cb88(UnkElemA0Array_b42a3* self, const UnkElemA0Array_b42a3* other) {
    if (self != other) {
        for (u64 i = 0; i < self->count; ++i)
            fn_7100c886c0(&self->data[i]);
        self->count = 0;
        AppendAll_b43a3(self, other);
    }
    return self;
}
