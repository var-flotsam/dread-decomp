// Unit 208: adds a per-index or constant offset to a vec3 array, storing the sum and the applied delta.
// Names and layouts are placeholders.
#include "types.h"

struct UnkVec3Elem {
    f32 x, y, z;
};

struct UnkVec3Array {
    u8 pad_0x00[0x20];
    UnkVec3Elem* data;
};

struct UnkOffsetSource {
    u8 pad_0x00[0x10];
    UnkVec3Elem constant;
    u8 pad_0x1c[4];
    UnkVec3Elem* data;
};

struct UnkOffsetApplier {
    u8 pad_0x00[8];
    UnkVec3Array* target;
    UnkOffsetSource* source;
    UnkVec3Array* delta;
};

static inline void ApplyOffset(UnkOffsetApplier* self, u32 i, const UnkVec3Elem& off) {
    UnkVec3Elem cur = self->target->data[i];
    UnkVec3Elem sum;
    sum.x = cur.x + off.x;
    sum.y = cur.y + off.y;
    sum.z = cur.z + off.z;
    UnkVec3Elem* d = &self->delta->data[i];
    d->x = sum.x - cur.x;
    d->y = sum.y - cur.y;
    d->z = sum.z - cur.z;
    self->target->data[i] = sum;
}

void fn_710016bf28(UnkOffsetApplier* self, const u32* indices, u32 count) {
    if (self->source->data) {
        for (u32 n = 0; n < count; n++) {
            u32 i = indices[n];
            ApplyOffset(self, i, self->source->data[i]);
        }
    } else {
        for (u32 n = 0; n < count; n++) {
            ApplyOffset(self, indices[n], self->source->constant);
        }
    }
}
