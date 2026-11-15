// CAutclastChargeAttack unit: picks one of three table values from a 2D direction.
// Names and layouts are placeholders.
#include "types.h"

extern f32 g_unk_71726bd2c8[];

f32 fn_71003b5d1c(const f32* dir) {
    if (__builtin_fabsf(dir[0]) > 0.9f)
        return g_unk_71726bd2c8[0x34 / 4];
    return !(dir[1] < -0.9f) ? g_unk_71726bd2c8[0x30 / 4] : g_unk_71726bd2c8[0x38 / 4];
}
