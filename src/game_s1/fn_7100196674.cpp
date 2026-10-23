// A lazily computed matrix: on first use, the matrix of the UnkXform at +0x30
// (include/game/UnkXformNode.h) with its y axis scaled by the aspect ratio of `size`
// (fn_710015bf40), cached at +0xbc.
#include "game/UnkXformNode.h"

struct UnkSize_7100196674 {
    u8 pad_0x0[0x8];
    u32 width;
    u32 height;
};

struct UnkCachedMatrix_7100196674 {
    u8 pad_0x0[0x30];
    UnkXform xform;
    u8 pad_0x8c[0x30];
    UnkMatrix44 matrix;  // 0xbc
    u8 pad_0xfc[0x38];
    bool cached;  // 0x134
};

void fn_710015bf40(UnkMatrix44* m, f32 scale);

UnkMatrix44* fn_7100196674(UnkCachedMatrix_7100196674* self, const UnkSize_7100196674* size) {
    if (!self->cached) {
        self->cached = true;
        const UnkMatrix44& m = self->xform.GetMatrix();
        for (int i = 0; i < 16; ++i)
            reinterpret_cast<a_u32*>(self->matrix.m)[i] = reinterpret_cast<const a_u32*>(m.m)[i];
        fn_710015bf40(&self->matrix, (f32)size->width / (f32)size->height);
    }
    return &self->matrix;
}
