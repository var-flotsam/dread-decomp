// CLightsDef's unit (tools/tu_map.py): the copy constructor and copy assignment of the unit's array
// (base::UnkVector's layout) of 0x60-byte elements (src/game/CLightsDef/CLightsDef_b42.cpp has its
// setters): reset (the assignment resizes to nothing, fn_7100da0870, instead), reserve the other's
// capacity (fn_7100da1508), then append a copy of each element (fn_7100d9bffc) with the push-back
// inlined, as base::UnkVector::PushBack. The shape of fn_7100c8b688
// (src/game/CCutSceneDef/CCutSceneDef_CopyA0_b43.cpp). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CLightsDef.h"
#include "types.h"

// As src/game/CLightsDef/CLightsDef_b42.cpp declares them.
struct UnkLightsElem_b42a3 {
    u8 unk_0x0[0x60];
};

struct UnkLightsArray_b42a3 {
    UnkLightsElem_b42a3* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

// As src/game/CLightManager/fn_7100e0ccf8.cpp declares it.
struct UnkClearedArray;

void fn_7100d9bffc(UnkLightsElem_b42a3* self, const UnkLightsElem_b42a3* other);  // the element's copy constructor

static inline __attribute__((always_inline)) void AppendAll_b43a3(UnkLightsArray_b42a3* self,
                                                                  const UnkLightsArray_b42a3* other) {
    fn_7100da1508(self, other->capacity);
    for (const UnkLightsElem_b42a3 *it = other->data, *end = it + other->count; it != end; ++it) {
        u32 n = self->count;
        u32 capacity = self->capacity;
        if (capacity <= n && self->step != 0)
            capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)capacity)
            for (;;) {
            }
        fn_7100da1508(self, capacity);
        fn_7100d9bffc(&self->data[self->count], it);
        ++self->count;
    }
}

void fn_7100da1438(UnkLightsArray_b42a3* self, const UnkLightsArray_b42a3* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    AppendAll_b43a3(self, other);
}

UnkLightsArray_b42a3* fn_7100da1668(UnkLightsArray_b42a3* self, const UnkLightsArray_b42a3* other) {
    if (self != other) {
        fn_7100da0870(reinterpret_cast<UnkClearedArray*>(self), 0);
        AppendAll_b43a3(self, other);
    }
    return self;
}
