// Unit 93 of tools/tu_map.py: prints three floats as "x,y,z" into the writer's 0x200-byte text
// buffer at +0xc and reports success (as fn_71000efa94 does for four).
// Names and layouts are placeholders.
#include <stdio.h>

#include "types.h"

struct UnkTextWriter_71000ef9cc {
    u8 unk_0x0[0xc];
    char text[0x200];
};

struct UnkFloat3_l0efa58 {
    f32 x, y, z;
};

bool fn_71000efa58(UnkTextWriter_71000ef9cc* self, const UnkFloat3_l0efa58* v) {
    snprintf(self->text, sizeof(self->text), "%f,%f,%f", v->x, v->y, v->z);
    return true;
}
