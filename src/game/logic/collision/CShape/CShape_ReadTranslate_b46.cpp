// game::logic::collision::CShape's unit (tools/tu_map.py): reads an offset (three floats) from a
// file when there is data left (zero otherwise) and moves the shape's matrix translation by it;
// always succeeds. Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "types.h"

using base::global::CRntFile;

struct UnkShapeOffset_b46a3 {
    f32 x, y, z;
};

struct UnkShapeXform_b46a3 {
    u8 pad_0x0[0x8];
    f32 m[4][4];  // 0x8, row-major; the translation is column 3
};

bool fn_7100d6c288(UnkShapeXform_b46a3* self, CRntFile* file) {
    UnkShapeOffset_b46a3 v;
    v.x = 0.0f;
    v.y = 0.0f;
    v.z = 0.0f;
    if (file->position < file->size)
        fn_710007c714(file, &v, sizeof(v), 1);
    self->m[0][3] += v.x;
    self->m[1][3] += v.y;
    self->m[2][3] += v.z;
    return true;
}
