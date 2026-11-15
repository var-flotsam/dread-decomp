// Unit 2563 (CMinimapData hint): the element at an index of a growable array of 0x60-byte records (two
// vectors and an empty 2D bounds box); an index past the end first reserves the next multiple of the
// step, then index + 1 records (fn_7100e52b58), and constructs the new ones. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkFreedVectorPair.h"
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkBoundsRecord_e536b8 {
    base::global::CRntVector<void*> first{0xf};   // 0x0
    base::global::CRntVector<void*> second{0xf};  // 0x28
    f32 min_x = 3.4028235e+38f;          // 0x50
    f32 min_y = 3.4028235e+38f;
    f32 max_x = -3.4028235e+38f;         // 0x58
    f32 max_y = -3.4028235e+38f;
};
static_assert(sizeof(UnkBoundsRecord_e536b8) == 0x60, "placeholder layout");

struct UnkBoundsRecords_e536b8 {
    UnkBoundsRecord_e536b8* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
};

UnkBoundsRecord_e536b8* fn_7100e536b8(UnkBoundsRecords_e536b8* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        u32 capacity = self->capacity;
        if (capacity <= index && self->step != 0)
            capacity = (index / self->step + 1) * self->step;
        fn_7100e52b58(reinterpret_cast<UnkPairArray*>(self), capacity);
        u32 n = index + 1;
        fn_7100e52b58(reinterpret_cast<UnkPairArray*>(self), n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkBoundsRecord_e536b8();
        self->count = n;
    }
    return &self->data[index];
}
