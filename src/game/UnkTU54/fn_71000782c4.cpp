// Unit 54: the transpose of a 4x4 matrix, returned through x8. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "types.h"

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
UnkMatrix44 fn_71000782c4(const UnkMatrix44* src) {
    UnkMatrix44 r;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            r.m[col * 4 + row] = src->m[row * 4 + col];
        }
    }
    return r;
}
