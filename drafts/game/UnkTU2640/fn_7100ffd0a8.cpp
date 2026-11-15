// Unit 2640: assigns a copy of a record to element `index` of an array of 0x50-byte records (a word, an array
// of path records and a run of plain fields), growing the array to the next multiple of its step and
// value-initialising the new records up to index + 1 first. Names and layouts are placeholders.
#include <new>

#include "../UnkUnit2601/UnkUnit2601_VecCopy_b56.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU2640.h"
#include "types.h"

struct UnkTU2640Rec50 {
    u32 field_0x0;
    u32 pad_0x4;
    base::global::CRntVector<u8> entries{0xf};  // 0x8
    u64 field_0x30 = 0;
    f32 scale_x = 1.0f;
    f32 scale_y = 1.0f;
    u64 field_0x40 = 0;
    u32 field_0x48;
    u8 field_0x4c = 0;

    // The word, the entries (fn_7100ffbad8) and the run of plain fields.
    UnkTU2640Rec50& operator=(const UnkTU2640Rec50& other) {
        field_0x0 = other.field_0x0;
        fn_7100ffbad8(reinterpret_cast<UnkPartFfca00*>(&entries), reinterpret_cast<const UnkPartFfca00*>(&other.entries));
        __builtin_memcpy(&field_0x30, &other.field_0x30, 0x1d);
        return *this;
    }
};
static_assert(sizeof(UnkTU2640Rec50) == 0x50, "UnkTU2640Rec50 layout");

struct UnkTU2640Rec50Array {
    UnkTU2640Rec50* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100ffd0a8(UnkTU2640Rec50Array* self, const u32* index, const UnkTU2640Rec50* value) {
    u32 i = *index;
    if (self->field_0x14 && self->count <= i) {
        u32 grown = self->capacity;
        if (grown <= i && self->step != 0)
            grown = (i / self->step + 1) * self->step;
        fn_7100f85d24(reinterpret_cast<UnkRecord50Vec_b56*>(self), grown);
        u32 n = i + 1;
        fn_7100f85d24(reinterpret_cast<UnkRecord50Vec_b56*>(self), n);
        for (u32 j = self->count; j < n; ++j)
            new (&self->data[j]) UnkTU2640Rec50();
        self->count = n;
    }
    self->data[i] = *value;
}
