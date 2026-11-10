// Unit 214 (tools/tu_map.py): a camera-like object's lazily built projection matrix (+0xf0).
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkXformNode.h"
#include "types.h"

UnkMatrix44 fn_71000791a8(const UnkVec3f& translation, const UnkVec3f& angle, const UnkVec3f& scale);
void fn_710015bf40(UnkMatrix44*, f32);

struct UnkViewport_b0249 {
    u8 pad_0x0[8];
    u32 width;   // 0x8
    u32 height;  // 0xc
};

struct UnkProjCamera_b0249 {
    u8 pad_0x0[0x30];
    UnkVec3f field_0x30;
    UnkVec3f field_0x3c;
    u8 pad_0x48[0xa8];
    UnkMatrix44 matrix;  // 0xf0
    u8 pad_0x130[4];
    bool built;  // 0x134
};

UnkMatrix44* fn_7100196248(UnkProjCamera_b0249* self, UnkViewport_b0249* view) {
    if (!self->built) {
        self->built = true;
        UnkCopyMatrix(&self->matrix, fn_71000791a8(self->field_0x30, UnkVec3f(0.0f, 0.0f, 0.0f), self->field_0x3c));
        fn_710015bf40(&self->matrix, (f32)view->width / (f32)view->height);
    }
    return &self->matrix;
}

struct UnkGrid_b0249 {
    u8 pad_0x0[0x20];
    u32 rows;       // 0x20
    u32 columns;    // 0x24
    u8 pad_0x28[0x48];
    f32* cells;     // 0x70
    u8 pad_0x78[0x20];
    f32* widths;    // 0x98
    u8 pad_0xa0[0x2c];
    u32 filled;     // 0xcc
    u8 pad_0xd0[0x65];
    bool dirty;     // 0x135
};

f32 fn_71000768f0(f32 v);

void fn_7100196054(UnkGrid_b0249* self) {
    if (!self->cells || self->filled >= self->columns)
        return;
    f32 edge = 0.0f;
    u64 i = 0;
    for (;;) {
        f32 next = edge - self->widths[i];
        if (i >= self->filled) {
            for (u32 j = 0; j < self->rows; j++) {
                u32 k = (self->columns * j + (u32)i) * 8 + 1;
                f32* c = &self->cells[k];
                c[2] = edge;
                c[0] = edge;
                c[6] = next;
                c[4] = next;
                c[0] = fn_71000768f0(c[0]);
                c[2] = fn_71000768f0(c[2]);
                c[4] = fn_71000768f0(c[4]);
                c[6] = fn_71000768f0(c[6]);
            }
        }
        i++;
        if (i >= self->columns)
            break;
        edge = next;
    }
    self->filled = self->columns;
    self->dirty = true;
}
