// Unit 83 (base::reflection): compares two arrays of 3-float vectors for equality within a small tolerance.
// Names and layouts are placeholders.
#include "types.h"

struct UnkVec83 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
};

struct UnkArray83 {
    UnkVec83* data;
    u32 pad_0x8_unused;
};

struct UnkVecArray83 {
    UnkVec83* data;
    u32 count;  // 0x8
};

static inline bool Near(f32 a, f32 b) {
    return a - 0.001f <= b && !(a + 0.001f < b);
}

bool fn_71000cb8d4(const UnkVecArray83* self, const UnkVecArray83* other) {
    if (self->count != other->count)
        return false;
    for (u32 i = 0; i < self->count; ++i) {
        if (!Near(other->data[i].field_0x0, self->data[i].field_0x0))
            return false;
        if (!Near(other->data[i].field_0x4, self->data[i].field_0x4))
            return false;
        if (!Near(other->data[i].field_0x8, self->data[i].field_0x8))
            return false;
    }
    return true;
}
