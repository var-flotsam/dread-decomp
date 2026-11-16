// Unit 2640: erases the 0x78-byte group at an index of an engine array (a word, an array of file-path
// records, a block of settings and an array of ids): destroys it and moves the later groups down by one.
// Names and layouts are placeholders.
#include <new>

#include "../UnkUnit2601/UnkUnit2601_VecCopy_b56.h"
#include "base/global/CRntVector.h"
#include "types.h"

// The file-path records; a move takes the other's array.
struct UnkPathRecords_l00208c {
    UnkPathRecordVec_b56 vec;

    UnkPathRecords_l00208c(UnkPathRecords_l00208c&& other) {
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
    ~UnkPathRecords_l00208c() { vec.Free(); }
};

// The ids; a move takes the other's array.
struct UnkIds_l00208c {
    base::global::CRntVector<u64> vec;

    UnkIds_l00208c(UnkIds_l00208c&& other) : vec(static_cast<base::global::CRntVector<u64>&&>(other.vec)) {}
    ~UnkIds_l00208c() { vec.Free(); }
};

struct UnkGroup_l00208c {
    u32 field_0x0;
    UnkPathRecords_l00208c entries;  // 0x8
    u64 field_0x30;
    f32 field_0x38;
    f32 field_0x3c;
    u64 field_0x40;
    u32 field_0x48;
    bool field_0x4c;
    UnkIds_l00208c ids;  // 0x50
};
static_assert(sizeof(UnkGroup_l00208c) == 0x78);

void fn_710100208c(base::global::CRntVector<UnkGroup_l00208c>* self, u32 index) {
    if (index < self->count) {
        u64 i = index;
        self->data[i].~UnkGroup_l00208c();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkGroup_l00208c* cur = &self->data[i];
            new (cur) UnkGroup_l00208c(static_cast<UnkGroup_l00208c&&>(cur[1]));
            (&self->data[i])[1].~UnkGroup_l00208c();
        }
        self->count = self->count - 1;
    }
}
