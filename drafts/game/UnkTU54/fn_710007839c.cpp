// Unit 54: inverts a 4x4 matrix in place by Cramer's rule: the cofactors of the transpose, built from
// products of pairs of its elements, over the determinant. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "types.h"

void fn_710007839c(UnkMatrix44* m) {
    f32* dst = m->m;
    // The transpose.
    f32 src[16] = {
        dst[0], dst[4], dst[8], dst[12],
        dst[1], dst[5], dst[9], dst[13],
        dst[2], dst[6], dst[10], dst[14],
        dst[3], dst[7], dst[11], dst[15],
    };

    // Products of pairs from the last two rows, for the first eight cofactors.
    f32 pair[12];
    pair[0] = src[10] * src[15];
    pair[1] = src[11] * src[14];
    pair[2] = src[9] * src[15];
    pair[3] = src[11] * src[13];
    pair[4] = src[9] * src[14];
    pair[5] = src[10] * src[13];
    pair[6] = src[8] * src[15];
    pair[7] = src[11] * src[12];
    pair[8] = src[8] * src[14];
    pair[9] = src[10] * src[12];
    pair[10] = src[8] * src[13];
    pair[11] = src[9] * src[12];

    dst[0] = pair[0] * src[5] + pair[3] * src[6] + pair[4] * src[7];
    dst[0] -= pair[1] * src[5] + pair[2] * src[6] + pair[5] * src[7];
    dst[1] = pair[1] * src[4] + pair[6] * src[6] + pair[9] * src[7];
    dst[1] -= pair[0] * src[4] + pair[7] * src[6] + pair[8] * src[7];
    dst[2] = pair[2] * src[4] + pair[7] * src[5] + pair[10] * src[7];
    dst[2] -= pair[3] * src[4] + pair[6] * src[5] + pair[11] * src[7];
    dst[3] = pair[5] * src[4] + pair[8] * src[5] + pair[11] * src[6];
    dst[3] -= pair[4] * src[4] + pair[9] * src[5] + pair[10] * src[6];
    dst[4] = pair[1] * src[1] + pair[2] * src[2] + pair[5] * src[3];
    dst[4] -= pair[0] * src[1] + pair[3] * src[2] + pair[4] * src[3];
    dst[5] = pair[0] * src[0] + pair[7] * src[2] + pair[8] * src[3];
    dst[5] -= pair[1] * src[0] + pair[6] * src[2] + pair[9] * src[3];
    dst[6] = pair[3] * src[0] + pair[6] * src[1] + pair[11] * src[3];
    dst[6] -= pair[2] * src[0] + pair[7] * src[1] + pair[10] * src[3];
    dst[7] = pair[4] * src[0] + pair[9] * src[1] + pair[10] * src[2];
    dst[7] -= pair[5] * src[0] + pair[8] * src[1] + pair[11] * src[2];

    // Products of pairs from the first two rows, for the last eight.
    pair[0] = src[2] * src[7];
    pair[1] = src[3] * src[6];
    pair[2] = src[1] * src[7];
    pair[3] = src[3] * src[5];
    pair[4] = src[1] * src[6];
    pair[5] = src[2] * src[5];
    pair[6] = src[0] * src[7];
    pair[7] = src[3] * src[4];
    pair[8] = src[0] * src[6];
    pair[9] = src[2] * src[4];
    pair[10] = src[0] * src[5];
    pair[11] = src[1] * src[4];

    dst[8] = pair[0] * src[13] + pair[3] * src[14] + pair[4] * src[15];
    dst[8] -= pair[1] * src[13] + pair[2] * src[14] + pair[5] * src[15];
    dst[9] = pair[1] * src[12] + pair[6] * src[14] + pair[9] * src[15];
    dst[9] -= pair[0] * src[12] + pair[7] * src[14] + pair[8] * src[15];
    dst[10] = pair[2] * src[12] + pair[7] * src[13] + pair[10] * src[15];
    dst[10] -= pair[3] * src[12] + pair[6] * src[13] + pair[11] * src[15];
    dst[11] = pair[5] * src[12] + pair[8] * src[13] + pair[11] * src[14];
    dst[11] -= pair[4] * src[12] + pair[9] * src[13] + pair[10] * src[14];
    dst[12] = pair[2] * src[10] + pair[5] * src[11] + pair[1] * src[9];
    dst[12] -= pair[4] * src[11] + pair[0] * src[9] + pair[3] * src[10];
    dst[13] = pair[8] * src[11] + pair[0] * src[8] + pair[7] * src[10];
    dst[13] -= pair[6] * src[10] + pair[9] * src[11] + pair[1] * src[8];
    dst[14] = pair[6] * src[9] + pair[11] * src[11] + pair[3] * src[8];
    dst[14] -= pair[10] * src[11] + pair[2] * src[8] + pair[7] * src[9];
    dst[15] = pair[10] * src[10] + pair[4] * src[8] + pair[9] * src[9];
    dst[15] -= pair[8] * src[9] + pair[11] * src[10] + pair[5] * src[8];

    f32 det = src[0] * dst[0] + src[1] * dst[1] + src[2] * dst[2] + src[3] * dst[3];
    det = 1.0f / det;
    for (int i = 0; i < 16; i++) dst[i] *= det;
}
