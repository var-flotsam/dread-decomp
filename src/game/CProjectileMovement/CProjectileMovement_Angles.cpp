// A function in the CProjectileMovement unit: fn_7100ac437c turns the direction at +0x4b8 (x, y; z 0,
// x nudged to 1e-5 when zero) into rotation angles (fn_710007a7f0). Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

UnkVec3f fn_710007a7f0(UnkVec3f dir);  // direction to rotation angles (src/game_s1/fn_71001bc8a4.cpp)

struct UnkProjectileMovement {
    u8 pad_0x0[0x4b8];
    u32 dir_0x4b8[2];
};

void fn_7100ac437c(UnkProjectileMovement* self, UnkVec3f* out) {
    UnkVec3f dir;
    struct Words { u32 x, y, z; };
    Words w = {self->dir_0x4b8[0], self->dir_0x4b8[1], 0};
    __builtin_memcpy(&dir, &w, sizeof(w));
    if (dir.x == 0.0f)
        dir.x = 1e-5f;
    *out = ((UnkVec3f(*)(const UnkVec3f&))fn_710007a7f0)(dir);
}
