// The owner's bounds (fn_71005c00fc): the base bounds (fn_71011061d0), grown to take in the point
// 1180 units above the owner's position.
#include "types.h"

struct UnkVec3 {
    f32 x, y, z;
};

struct UnkAabb {
    UnkVec3 min;
    UnkVec3 max;
};

struct UnkTransformOwner {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
};

struct UnkBoundsComponent {
    u8 pad_0x0[0x30];
    UnkTransformOwner* field_0x30;
};

void fn_71011061d0(UnkBoundsComponent* self, UnkAabb* out);

static inline f32 Min(f32 a, f32 b) { return a < b ? a : b; }
static inline f32 Max(f32 a, f32 b) { return a > b ? a : b; }

void fn_71005c00fc(UnkBoundsComponent* self, UnkAabb* out) {
    fn_71011061d0(self, out);
    UnkTransformOwner* o = self->field_0x30;
    UnkVec3 p = {o->x, o->y, o->z};
    p.y += 1180.0f;
    out->min = UnkVec3{Min(out->min.x, p.x), Min(out->min.y, p.y), Min(out->min.z, p.z)};
    out->max = UnkVec3{Max(out->max.x, p.x), Max(out->max.y, p.y), Max(out->max.z, p.z)};
}
