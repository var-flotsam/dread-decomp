// Unit 93 of tools/tu_map.py (0x71000ef534-0x71000efdb0): writers that print a value as text into
// a 0x200-byte buffer at +0xc of the writer and report success: a bool as "true" or "false"
// (fn_71000ef9cc), four floats (fn_71000efa94, fn_71000efb10) and an 8-bit colour stored blue
// first, printed red first (fn_71000efb50).
// Names and layouts are placeholders.
#include <stdio.h>

#include "types.h"

struct UnkTextWriter_71000ef9cc {
    u8 unk_0x0[0xc];
    char text[0x200];
};

struct UnkFloat4_71000efa94 {
    f32 x, y, z, w;
};

struct UnkColorBGRA_71000efb50 {
    u8 b, g, r, a;
};

bool fn_71000ef9cc(UnkTextWriter_71000ef9cc* self, const bool* value) {
    snprintf(self->text, sizeof(self->text), "%s", *value ? "true" : "false");
    return true;
}

static inline bool WriteFloat4(UnkTextWriter_71000ef9cc* self, const UnkFloat4_71000efa94* v) {
    snprintf(self->text, sizeof(self->text), "%f,%f,%f,%f", v->x, v->y, v->z, v->w);
    return true;
}

bool fn_71000efa94(UnkTextWriter_71000ef9cc* self, const UnkFloat4_71000efa94* v) { return WriteFloat4(self, v); }

bool fn_71000efb10(UnkTextWriter_71000ef9cc* self, const UnkFloat4_71000efa94* v) { return WriteFloat4(self, v); }

bool fn_71000efb50(UnkTextWriter_71000ef9cc* self, const UnkColorBGRA_71000efb50* c) {
    snprintf(self->text, sizeof(self->text), "%d,%d,%d,%d", c->r, c->g, c->b, c->a);
    return true;
}
