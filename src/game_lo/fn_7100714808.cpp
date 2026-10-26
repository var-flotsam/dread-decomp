// Blends another weighted transform into this one. When this one is empty (no weight or no
// flags) it is replaced by the other (fn_7100714604). Otherwise the matrices are multiplied,
// the flags merged, and the position either becomes the weighted average (mode 1, weights
// summed) or the product's translation divided by the larger weight.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector3D;

// The matrix (base/UnkMatrix44.h) with this file's element-by-element copy: with the real type's
// plain copy the function no longer resembles the original.
struct UnkMatrix44_7100714808 : UnkMatrix44 {
    UnkMatrix44_7100714808(const UnkMatrix44& o) {
        for (s32 i = 0; i < 16; ++i)
            m[i] = o.m[i];
    }
    UnkMatrix44_7100714808(const UnkMatrix44_7100714808& o) {
        for (s32 i = 0; i < 16; ++i)
            m[i] = o.m[i];
    }
    UnkMatrix44_7100714808& operator=(const UnkMatrix44_7100714808& o) {
        for (s32 i = 0; i < 16; ++i)
            m[i] = o.m[i];
        return *this;
    }
};

// A transform: its matrix, six cleared words and a kind (8 when built from a matrix).
struct UnkXform_7100714808 {
    UnkMatrix44_7100714808 m;
    f32 field_0x40[6];
    u32 field_0x58;

    explicit UnkXform_7100714808(const UnkMatrix44& mat) : m(mat) {
        for (s32 i = 0; i < 6; ++i)
            field_0x40[i] = 0.0f;
        field_0x58 = 8;
    }
    UnkXform_7100714808& operator=(const UnkXform_7100714808& o) {
        if (&o != this) {
            m = o.m;
            for (s32 i = 0; i < 6; ++i)
                field_0x40[i] = o.field_0x40[i];
            field_0x58 = o.field_0x58;
        }
        return *this;
    }
};

struct UnkWeighted_7100714808 {
    UnkXform_7100714808 xform;
    u32 flags;
    CVector3D pos;
    f32 weight;
};

void fn_7100714604(UnkWeighted_7100714808* self, const UnkWeighted_7100714808* other);  // assigns

// NON_MATCHING(minor): in the weighted average, four of the multiplications by the inverse
// total weight are scheduled in a different order.
void fn_7100714808(UnkWeighted_7100714808* self, const UnkWeighted_7100714808* other, s32 mode) {
    if (!(other->weight > 0.0f) || other->flags == 0)
        return;
    if (!(self->weight > 0.0f) || self->flags == 0) {
        fn_7100714604(self, other);
        return;
    }
    const UnkMatrix44& a = fn_710002614c(&self->xform.m);
    const UnkMatrix44& b = fn_710002614c(&other->xform.m);
    UnkMatrix44 prod = fn_71000127c8(a, b);
    self->xform = UnkXform_7100714808(prod);
    self->flags = self->flags | other->flags;
    f32 w0 = self->weight;
    f32 w1 = other->weight;
    if (mode == 1) {
        f32 total = w0 + w1;
        f32 inv = 1.0f / total;
        f32 sz = w0 * self->pos.z;
        f32 sx = w0 * self->pos.x;
        f32 sy = w0 * self->pos.y;
        f32 oy = w1 * other->pos.y;
        f32 ox = w1 * other->pos.x;
        f32 oz = w1 * other->pos.z;
        f32 x = inv * sx + inv * ox;
        f32 y = inv * sy + inv * oy;
        f32 z = inv * sz + inv * oz;
        self->pos.x = x;
        self->pos.y = y;
        self->pos.z = z;
        self->weight = total;
    } else {
        f32 tx = self->xform.m.m[3];
        f32 ty = self->xform.m.m[7];
        f32 tz = self->xform.m.m[11];
        f32 w = w0 > w1 ? w0 : w1;
        self->weight = w;
        f32 inv = 1.0f / w;
        self->pos.x = inv * tx;
        self->pos.y = inv * ty;
        self->pos.z = inv * tz;
    }
}
