// Unit 2640: the "set by move at index" slot of an array of 0x50-byte records (a word, an array of file-path
// records and a block of settings). An index past the end of a growable array grows it first. Names and
// layouts are placeholders.
#include <new>

#include "../UnkUnit2601/UnkUnit2601_VecCopy_b56.h"
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkRecord50_lffd210 {
    u32 field_0x0;
    base::global::CRntVector<UnkPathRecord18_b56> records{0xf};
    u64 field_0x30 = 0;
    f32 field_0x38 = 1.0f;
    f32 field_0x3c = 1.0f;
    u64 field_0x40 = 0;
    u32 field_0x48;
    bool field_0x4c = false;
};
static_assert(sizeof(UnkRecord50_lffd210) == 0x50);

struct UnkRecord50Array_lffd210 {
    UnkRecord50_lffd210* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;
};

static inline void Grow_lffd210(UnkRecord50Array_lffd210* self, u32 index) {
    u32 capacity = self->capacity;
    if (capacity <= index && self->step != 0)
        capacity = (index / self->step + 1) * self->step;
    fn_7100f85d24(reinterpret_cast<UnkRecord50Vec_b56*>(self), capacity);
    u32 n = index + 1;
    fn_7100f85d24(reinterpret_cast<UnkRecord50Vec_b56*>(self), n);
    for (u32 i = self->count; i < n; ++i)
        new (&self->data[i]) UnkRecord50_lffd210();
    self->count = n;
}

void fn_7100ffd210(UnkRecord50Array_lffd210* self, const u32* index, UnkRecord50_lffd210* value) {
    u32 i = *index;
    if (self->autoGrow && self->count <= i)
        Grow_lffd210(self, i);
    self->data[i] = static_cast<UnkRecord50_lffd210&&>(*value);
}
