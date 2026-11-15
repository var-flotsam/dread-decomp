// Unit 54: the transpose of a 4x4 matrix, returned through x8. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "types.h"

UnkMatrix44 fn_71000782c4(const UnkMatrix44* src) {
    UnkMatrix44 r;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            r.m[col * 4 + row] = src->m[row * 4 + col];
        }
    }
    return r;
}
