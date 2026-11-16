// Unit 2640: moves a 0x50-byte record (a word, an array of file-path records and a block of settings) in at
// an index of an engine array, moving the later records up. Names and layouts are placeholders.
#include <new>

#include "../UnkUnit2601/UnkUnit2601_VecCopy_b56.h"
#include "types.h"

// The file-path records; a move takes the other's array.
struct UnkPathRecords_lffd848 {
    UnkPathRecordVec_b56 vec;

    UnkPathRecords_lffd848(UnkPathRecords_lffd848&& other) {
        vec.data = other.vec.data;
        vec.count = other.vec.count;
        vec.capacity = other.vec.capacity;
        vec.step = other.vec.step;
        vec.field_0x14 = other.vec.field_0x14;
        vec.flags = other.vec.flags;
        vec.allocator = other.vec.allocator;
        other.vec.data = nullptr;
        other.vec.count = 0;
        other.vec.capacity = 0;
    }
    ~UnkPathRecords_lffd848() { vec.Free(); }
};

struct UnkRecord50_lffd848 {
    u32 field_0x0;
    UnkPathRecords_lffd848 records;
    u64 field_0x30;
    f32 field_0x38;
    f32 field_0x3c;
    u64 field_0x40;
    u32 field_0x48;
    bool field_0x4c;
};
static_assert(sizeof(UnkRecord50_lffd848) == 0x50);

struct UnkRecord50Array_lffd848 {
    UnkRecord50_lffd848* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;
};

// The array's reserve, out of line (the same code as unit 2601's).
static inline void Reserve_lffd848(UnkRecord50Array_lffd848* self, u32 n) {
    fn_7100f85d24(reinterpret_cast<UnkRecord50Vec_b56*>(self), n);
}

void fn_7100ffd848(UnkRecord50Array_lffd848* self, UnkRecord50_lffd848* value, u32 index) {
    u32 n = self->count;
    u32 capacity = self->capacity;
    if (n == index) {
        if (capacity <= n && self->step != 0)
            capacity = (index / self->step + 1) * self->step;
        // A capacity that does not grow past the index hangs, as in the original.
        if ((s32)capacity <= (s32)index)
            for (;;) {
            }
        Reserve_lffd848(self, capacity);
        new (&self->data[self->count]) UnkRecord50_lffd848(static_cast<UnkRecord50_lffd848&&>(*value));
    } else {
        if (capacity <= n && self->step != 0)
            capacity = (n / self->step + 1) * self->step;
        Reserve_lffd848(self, capacity);
        for (u32 i = self->count - 1; i >= index; --i) {
            UnkRecord50_lffd848* q = &self->data[i];
            new (q + 1) UnkRecord50_lffd848(static_cast<UnkRecord50_lffd848&&>(*q));
            self->data[i].~UnkRecord50_lffd848();
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkRecord50_lffd848(static_cast<UnkRecord50_lffd848&&>(*value));
    }
    ++self->count;
}
