// CQuetzoaEnergyWaveAttack: offsets a position (fn_7100645568) by 211 units against the sign of the
// facing value and 78 units up. Names and layouts are placeholders.
#include "types.h"

struct UnkVec3 {
    f32 x, y, z;
};

struct UnkEnergyWaveAttack;

void fn_7100645568(UnkEnergyWaveAttack* self, UnkVec3* pos, const f32* facing) {
    f32 f = *facing;
    f32 sign = (f32)((s32)(f > 0.0f) - (s32)(f < 0.0f));
    UnkVec3 offset = {sign * -211.0f, 78.0f, 0.0f};
    pos->x += offset.x;
    pos->y += offset.y;
    pos->z += offset.z;
}
