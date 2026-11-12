// CGrappleBeamComponent unit (tools/tu_map.py, unit 1277): whether a target position is in reach of the beam:
// from the owner's position (the translation column of the transform at +0x38), or the point fn_710081b920 answers
// when it is not zero, within fMaxLength, and beyond fMaxLengthToStartRetract only where the render manager
// projects it (fn_7100219588). Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "types.h"

using base::reflection::UnkTunable;

// The answer of fn_710081b920 (src/game/CGrappleBeamComponent/CGrappleBeamComponent_b50.cpp), here read as
// a position.
struct UnkGrappleAnswer_b50 {
    void* field_0x0;
    u32 field_0x8;

    UnkGrappleAnswer_b50() : field_0x0(nullptr), field_0x8(0) {}
    UnkGrappleAnswer_b50(const UnkGrappleAnswer_b50& other);
};
struct UnkGrappleBeam_b50;
UnkGrappleAnswer_b50 fn_710081b920(UnkGrappleBeam_b50* self);

extern UnkTunable g_unk_71727232d8;

struct UnkGrappleTunable_ovb0047 {
    u8 pad_0x0[0x2c];
    f32 max_length;  // 0x2c, fMaxLength
    f32 max_length_to_start_retract;  // 0x30, fMaxLengthToStartRetract
};

struct UnkGrappleVec3_ovb0047 {
    f32 x, y, z;
    bool IsZero() const { return x == 0.0f && y == 0.0f && z == 0.0f; }
};

struct UnkGrappleTransform_ovb0047 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38
};

bool fn_710081c3b0(UnkGrappleBeam_b50* self, const UnkGrappleTransform_ovb0047* origin,
                   const UnkGrappleVec3_ovb0047* target) {
    UnkGrappleVec3_ovb0047 pos = {origin->transform[0][3], origin->transform[1][3], origin->transform[2][3]};
    UnkGrappleAnswer_b50 answer = fn_710081b920(self);
    const UnkGrappleVec3_ovb0047& point = *reinterpret_cast<const UnkGrappleVec3_ovb0047*>(&answer);
    if (!point.IsZero())
        pos = point;
    const UnkGrappleTunable_ovb0047* tunable = reinterpret_cast<const UnkGrappleTunable_ovb0047*>(&g_unk_71727232d8);
    f32 max = tunable->max_length;
    f32 max2 = max * max;
    f32 dx = target->x - pos.x;
    f32 dy = target->y - pos.y;
    f32 dz = target->z - pos.z;
    f32 dist2 = dx * dx + dy * dy + dz * dz;
    if (dist2 > max2)
        return false;
    f32 near = tunable->max_length_to_start_retract;
    if (dist2 > near * near && !fn_7100219588(g_unk_7101d47300, target, 0, 0))
        return false;
    return true;
}
