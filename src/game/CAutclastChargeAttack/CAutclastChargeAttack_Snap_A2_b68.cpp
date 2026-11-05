// CAutclastChargeAttack's unit (tools/tu_map.py, the name from reflection): snaps one coordinate of a position to a
// grid of 100 offset by 50 away from zero. With a mostly horizontal direction (|x| > 0.95) it snaps y, with a
// mostly vertical one it snaps x. Names are placeholders.
#include "types.h"

static inline f32 UnkFabs_A2_b68(f32 v) {
    return __builtin_fabsf(v);
}

static inline __attribute__((always_inline)) void UnkSnap_A2_b68(f32* v) {
    s32 q = (s32)*v / 100 * 100;
    s32 sign = (q > 0) - (q < 0);
    *v = (f32)(q + sign * 50);
}

void fn_71003b50c4(f32* pos, const f32* dir) {
    f32* v;
    if (UnkFabs_A2_b68(dir[0]) > 0.95f)
        v = &pos[1];
    else if (UnkFabs_A2_b68(dir[1]) > 0.95f)
        v = &pos[0];
    else
        return;
    UnkSnap_A2_b68(v);
}
