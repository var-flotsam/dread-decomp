// CLightsDef's unit (tools/tu_map.py): the "set element at index" functions (slots 0x140 and 0x148 of
// the reflected collection type) of an array (base::UnkVector's layout) of 0x60-byte elements. An
// index past the end of a growable array first reserves its capacity (grown to the next multiple of
// the step when the index reaches it, fn_7100da1508) and then index + 1 elements, constructs the new
// ones (fn_7100d9b128) and extends the count. The copying setter stops there (the element's copy
// assignment does nothing); the moving one then move-assigns the element (fn_7100d9b27c). The shape of
// fn_7100c92080 (src/game/CCutSceneDef/CCutSceneDef_b42.cpp). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

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

void fn_7100da1508(UnkLightsArray_b42a3* self, u32 capacity);
void fn_7100d9b128(UnkLightsElem_b42a3* self);
void fn_7100d9b27c(UnkLightsElem_b42a3* self, UnkLightsElem_b42a3* other);

static inline void Grow_b42a3(UnkLightsArray_b42a3* self, u32 index) {
    u32 capacity = self->capacity;
    if (capacity <= index && self->step != 0)
        capacity = (index / self->step + 1) * self->step;
    fn_7100da1508(self, capacity);
    u32 n = index + 1;
    fn_7100da1508(self, n);
    for (u32 i = self->count; i < n; ++i)
        fn_7100d9b128(&self->data[i]);
    self->count = n;
}

void fn_7100da1138(UnkLightsArray_b42a3* self, const u32* index, const UnkLightsElem_b42a3*) {
    if (self->field_0x14) {
        u32 i = *index;
        if (self->count <= i)
            Grow_b42a3(self, i);
    }
}

void fn_7100da11e4(UnkLightsArray_b42a3* self, const u32* index, UnkLightsElem_b42a3* value) {
    u32 i = *index;
    if (self->field_0x14 && self->count <= i)
        Grow_b42a3(self, i);
    fn_7100d9b27c(&self->data[i], value);
}
