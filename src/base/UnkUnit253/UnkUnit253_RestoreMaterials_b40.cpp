// Unit 253 of tools/tu_map.py (0x71001c071c-0x71001c33cc, no reflected class): clears the word at 0x350
// and resets every material of the model at 0x78 with all flags (0x7ff) through fn_71001a6ec8
// (src/game/msapi/material/CMaterial.cpp). Names and layouts are placeholders.
#include "types.h"

struct UnkMaterial;
void fn_71001a6ec8(UnkMaterial* self, u32 flags);

struct UnkModel253 {
    u8 pad_0x0[0x2c];
    u32 material_count;
    u8 pad_0x30[0x18];
    UnkMaterial** materials;  // 0x48
};

struct UnkObject253 {
    u8 pad_0x0[0x78];
    UnkModel253* model;
    u8 pad_0x80[0x2d0];
    u32 field_0x350;
};

void fn_71001c3200(UnkObject253* self) {
    self->field_0x350 = 0;
    u32 count = self->model->material_count;
    for (u64 i = 0; i < count; ++i)
        fn_71001a6ec8(self->model->materials[i], 0x7ff);
}
