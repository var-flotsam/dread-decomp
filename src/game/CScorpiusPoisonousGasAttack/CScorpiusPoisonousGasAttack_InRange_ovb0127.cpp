// CScorpiusPoisonousGasAttack's unit (tools/tu_map.py): whether the actor is within the tunable
// radius (g_unk_7172702030, +0x30) of the end of the Scorpius AI's path it faces: its last point when
// the actor's matrix entry m[2] is positive, else its first, in 2D. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkScorpiusAI_ovb0127 {
    u8 pad_0x0[0x68];
    CVector3D* points;  // 0x68
    u32 count;          // 0x70
    u8 pad_0x74[0x20];
    u8 looped;  // 0x94
};

struct UnkScorpiusTunable_ovb0127 {
    u8 pad_0x0[0x30];
    f32 radius;  // 0x30
};
extern UnkScorpiusTunable_ovb0127 g_unk_7172702030;

static inline UnkScorpiusAI_ovb0127* ScorpiusAI(UnkActor* actor) {
    UnkReflObject* ai = static_cast<UnkReflObject*>(actor->field_0x240);
    if (!ai)
        return nullptr;
    return fn_710009d50c(ai->vf_0x0(), fn_7100690c74()) ? reinterpret_cast<UnkScorpiusAI_ovb0127*>(ai) : nullptr;
}

bool fn_7100689c0c(CAttack* self) {
    UnkScorpiusAI_ovb0127* ai = ScorpiusAI(self->field_0x8);
    u32 last = ai->count;
    if (ai->looped == 0 && last != 0)
        --last;
    if (last == 0)
        return false;
    CVector2D first = {ai->points[0].x, ai->points[0].y};
    CVector2D end = {ai->points[last].x, ai->points[last].y};
    f32 facing = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->field_0x8->field_0x38)).m[2];
    CVector2D here;
    f32 radius = g_unk_7172702030.radius;
    UnkActor* actor = self->field_0x8;
    f32 x = actor->field_0x44;
    f32 y = actor->field_0x54;
    f32 z = actor->field_0x64;
    f32 radius_sq = radius * radius;
    CVector3D position;
    position.x = x;
    position.y = y;
    position.z = z;
    fn_710007b430(&here, &position);
    const CVector2D& target = facing > 0.0f ? end : first;
    f32 dx = target.x - here.x;
    f32 dy = target.y - here.y;
    return dx * dx + dy * dy <= radius_sq;
}
