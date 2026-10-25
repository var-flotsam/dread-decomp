// Computes a position taken from the object found through the owner's +0x8: when the object at +0x208
// reports the first state, the position is moved by -75 on y if the owner is below it on y and the
// object's matrix points toward it, or by 50 along the matrix's x column when the two matrices
// differ; otherwise y is raised by 6. Returns false when there is no object.
#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkVec3 {
    u32 x;
    u32 y;
    u32 z;
};

struct UnkHelper {
    void** vtable;
};

struct UnkTarget {
    u8 pad_0x0[0x38];
    u8 field_0x38[0xc];
    u32 field_0x44;
    u8 pad_0x48[0xc];
    u32 field_0x54;
    u8 pad_0x58[0xc];
    u32 field_0x64;
    u8 pad_0x68[0x1a0];
    UnkHelper* field_0x208;
};

struct UnkOwner {
    u8 pad_0x0[0x38];
    u8 field_0x38[0xc];
    u32 field_0x44;
    u8 pad_0x48[0xc];
    u32 field_0x54;
    u8 pad_0x58[0xc];
    u32 field_0x64;
};

struct UnkSelf {
    u8 pad_0x0[0x8];
    UnkOwner* field_0x8;
};

bool fn_71009e12d0(UnkHelper* helper);
bool fn_71009e6434(UnkHelper* helper);
// The matrix of the transform at +0x38 of an actor (fn_710002614c), as its 16 floats.
static inline const f32* Matrix(const u8* xform) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(xform)).m;
}

static inline f32 AsF32(u32 bits) {
    f32 value;
    __builtin_memcpy(&value, &bits, 4);
    return value;
}

static inline u32 AsU32(f32 value) {
    u32 bits;
    __builtin_memcpy(&bits, &value, 4);
    return bits;
}

bool fn_710046514c(UnkSelf* self, UnkVec3* out) {
    UnkTarget* target = reinterpret_cast<UnkTarget*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(self->field_0x8)));
    if (!target)
        return false;
    u32 x = target->field_0x44;
    u32 y = target->field_0x54;
    u32 z = target->field_0x64;
    if (target->field_0x208) {
        if (fn_71009e12d0(target->field_0x208)) {
            if (AsF32(self->field_0x8->field_0x54) < AsF32(y)) {
                f32 oy = AsF32(self->field_0x8->field_0x54);
                f32 oz = AsF32(self->field_0x8->field_0x64);
                f32 ox = AsF32(self->field_0x8->field_0x44);
                const f32* m = Matrix(target->field_0x38);
                f32 m0 = m[2];
                f32 m1 = m[6];
                f32 m2 = m[10];
                f32 dx = AsF32(x) - ox;
                f32 dy = AsF32(y) - oy;
                f32 dz = AsF32(z) - oz;
                f32 dot = dx * m0 + dy * m1 + dz * m2;
                if (dot > 0.0f)
                    y = AsU32(AsF32(y) + -75.0f);
            } else if (!(reinterpret_cast<bool (*)(UnkHelper*)>(target->field_0x208->vtable[0x3b8 / 8]))(
                           target->field_0x208) &&
                       !fn_71009e6434(target->field_0x208)) {
                const f32* a = Matrix(target->field_0x38);
                f32 a0 = a[2];
                f32 a1 = a[6];
                f32 a2 = a[10];
                const f32* b = Matrix(self->field_0x8->field_0x38);
                f32 b0;
                f32 b1;
                f32 b2;
                if (!((b0 = b[2], a0 + 0.001f >= b0 && a0 + -0.001f <= b0) &&
                      (b1 = b[6], a1 + 0.001f >= b1 && a1 + -0.001f <= b1) &&
                      (b2 = b[10], a2 + 0.001f >= b2 && a2 + -0.001f <= b2)))
                    x = AsU32(Matrix(target->field_0x38)[2] * 50.0f + AsF32(x));
            }
        } else {
            y = AsU32(AsF32(y) + 6.0f);
        }
    }
    out->x = x;
    out->y = y;
    out->z = z;
    return true;
}
