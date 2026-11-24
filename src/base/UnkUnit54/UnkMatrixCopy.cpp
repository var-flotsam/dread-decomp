// From unit 54 of tools/tu_map.py (0x71000769f0-0x710007b430, no reflected class): returns a copy of
// a 4x4 float matrix (as words), with the fourth word of each of the first three rows cleared and
// the last row kept. The matrix type has a user-provided copy, so it is returned in memory (x8).
// Names are placeholders.
#include "base/UnkMatrix44Words.h"
#include "types.h"

UnkMatrix44Words CopyMatrixWithoutTranslation(const UnkMatrix44Words* src) {
    UnkMatrix44Words r;
    UnkMatrix44Words* dst = &r;
    dst->m[0] = src->m[0];
    dst->m[1] = src->m[1];
    dst->m[2] = src->m[2];
    u32 m4 = src->m[4];
    asm("" ::: "memory");  // the original reads the second row before clearing the first row's last word
    dst->m[3] = 0;
    dst->m[4] = m4;
    dst->m[5] = src->m[5];
    dst->m[6] = src->m[6];
    dst->m[7] = 0;
    dst->m[8] = src->m[8];
    dst->m[9] = src->m[9];
    dst->m[10] = src->m[10];
    dst->m[11] = 0;
    dst->m[12] = src->m[12];
    dst->m[13] = src->m[13];
    dst->m[14] = src->m[14];
    dst->m[15] = src->m[15];
    return r;
}
