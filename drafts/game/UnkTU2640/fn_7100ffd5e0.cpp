// Unit 2640: inserts a copy of a 0x50-byte record (a word, an array of file-path records and a block of
// settings) at an index of an engine array, moving the later records up. Names and layouts are placeholders.
#include <new>

#include "../UnkUnit2601/UnkUnit2601_VecCopy_b56.h"
#include "base/global/CRntVector.h"
#include "types.h"

// The file-path records, copied out of line; a move takes the other's array.
struct UnkPathRecords_lffd5e0 {
    UnkPathRecordVec_b56 vec;

    UnkPathRecords_lffd5e0(const UnkPathRecords_lffd5e0& other) { fn_7100f85f00(&vec, &other.vec); }
    UnkPathRecords_lffd5e0(UnkPathRecords_lffd5e0&& other) : vec(other.vec) {
        other.vec.data = nullptr;
        other.vec.count = 0;
        other.vec.capacity = 0;
    }
    ~UnkPathRecords_lffd5e0() { vec.Free(); }
};

struct UnkRecord50_lffd5e0 {
    u32 field_0x0;
    UnkPathRecords_lffd5e0 records;
    u64 field_0x30;
    f32 field_0x38;
    f32 field_0x3c;
    u64 field_0x40;
    u32 field_0x48;
    bool field_0x4c;
};
static_assert(sizeof(UnkRecord50_lffd5e0) == 0x50);

using UnkRecord50Array_lffd5e0 = base::global::CRntVector<UnkRecord50_lffd5e0>;

// The array's reserve, out of line (the same code as unit 2601's).
static inline void Reserve_lffd5e0(UnkRecord50Array_lffd5e0* self, u32 n) {
    fn_7100f85d24(reinterpret_cast<UnkRecord50Vec_b56*>(self), n);
}

void fn_7100ffd5e0(UnkRecord50Array_lffd5e0* self, const UnkRecord50_lffd5e0* value, u32 index) {
    u32 n = self->count;
    u32 capacity = self->capacity;
    if (n == index) {
        if (capacity <= n && self->step != 0)
            capacity = (index / self->step + 1) * self->step;
        // A capacity that does not grow past the index hangs, as in the original.
        if ((s32)capacity <= (s32)index)
            for (;;) {
            }
        Reserve_lffd5e0(self, capacity);
        new (&self->data[self->count]) UnkRecord50_lffd5e0(*value);
    } else {
        if (capacity <= n && self->step != 0)
            capacity = (n / self->step + 1) * self->step;
        Reserve_lffd5e0(self, capacity);
        for (u32 i = self->count - 1; i >= index; --i) {
            new (&self->data[i + 1]) UnkRecord50_lffd5e0(static_cast<UnkRecord50_lffd5e0&&>(self->data[i]));
            self->data[i].~UnkRecord50_lffd5e0();
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkRecord50_lffd5e0(*value);
    }
    ++self->count;
}
