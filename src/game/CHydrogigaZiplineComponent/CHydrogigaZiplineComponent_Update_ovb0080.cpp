// CHydrogigaZiplineComponent's unit (tools/tu_map.py unit 1410): a new value for the zipline. When it
// moves past the transform's height (+0x64) in either direction, vf_0x178 is told (with 1.0, or the value
// at +0x520 the first time the flag at +0x530 is set). Names and layouts are placeholders.
#include "types.h"

struct UnkZiplineComponent_b0080;

struct UnkZiplineVtable_b0080 {
    u8 pad_0x0[0x178];
    void (*vf_0x178)(UnkZiplineComponent_b0080* self, f32 value);
};

struct UnkZiplineTransform_b0080 {
    u8 pad_0x0[0x64];
    f32 z;  // 0x64
};

struct UnkZiplineComponent_b0080 {
    UnkZiplineVtable_b0080* vtable;
    u8 pad_0x8[0x4e8];
    UnkZiplineTransform_b0080* transform;  // 0x4f0
    u8 pad_0x4f8[0x28];
    f32 field_0x520;
    u8 pad_0x524[0x8];
    f32 lastValue;  // 0x52c
    u8 flag;  // 0x530
};
static_assert(__builtin_offsetof(UnkZiplineComponent_b0080, flag) == 0x530, "layout");

static inline bool Near(f32 current, f32 target) {
    return target >= current - 0.001f && target <= current + 0.001f;
}

void fn_710091c8c0(UnkZiplineComponent_b0080* self, f32 value) {
    f32 last = self->lastValue;
    if (Near(last, value))
        return;
    f32 z = self->transform->z;
    if (last < value) {
        if (!(z <= value)) {
            self->vtable->vf_0x178(self, 1.0f);
        } else {
            u8 prev = self->flag;
            self->flag = 1;
            if (prev != 1)
                self->vtable->vf_0x178(self, self->field_0x520);
        }
    } else if (z > value) {
        u8 prev = self->flag;
        self->flag = 0;
        if (prev)
            self->vtable->vf_0x178(self, 1.0f);
    }
    self->lastValue = value;
}
