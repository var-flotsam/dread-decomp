// Unit 197 of tools/tu_map.py (0x710016931c-0x710016947c): an object of three 3-float vectors (0x24
// bytes) that builds a 4x4 matrix from them (fn_71000797b4) into a target at +0x20. Names and
// layouts are placeholders.
#include "types.h"

struct UnkVec3_197 {
    f32 x, y, z;
};

struct UnkMatrix44_197 {
    f32 m[16];

    UnkMatrix44_197& operator=(const UnkMatrix44_197& other) {
        for (u32 i = 0; i < 16; ++i)
            m[i] = other.m[i];
        return *this;
    }
};

UnkMatrix44_197 fn_71000797b4(const UnkVec3_197* a, const UnkVec3_197* b, const UnkVec3_197* c);

struct UnkVectors197 {
    UnkVec3_197 a;
    UnkVec3_197 b;
    UnkVec3_197 c;
};

struct UnkMatrixTarget197 {
    u8 pad_0x0[0x20];
    UnkMatrix44_197 matrix;
};

void fn_7100169338(const UnkVectors197* self, UnkMatrixTarget197* target) {
    target->matrix = fn_71000797b4(&self->a, &self->b, &self->c);
}
