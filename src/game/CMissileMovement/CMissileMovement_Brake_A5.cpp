// CMissileMovement's unit (tools/tu_map.py): CProjectileMovement's brake vector (fn_7100ac419c,
// src/game/CProjectileMovement/CProjectileMovement_Brake_b46.cpp) with its z cleared while the sound
// player (g_unk_71726af0b0) has an object from fn_7100ed77a0. Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkProjectileMovement4_b46a3;

UnkVec3f fn_7100ac419c(UnkProjectileMovement4_b46a3* self, f32 t);
void* fn_7100ed77a0(void* player);
extern void* g_unk_71726af0b0;

UnkVec3f fn_7100abac88(UnkProjectileMovement4_b46a3* self, f32 t) {
    UnkVec3f v = fn_7100ac419c(self, t);
    if (fn_7100ed77a0(g_unk_71726af0b0))
        v.z = 0.0f;
    return v;
}
