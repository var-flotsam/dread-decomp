// CObsydomithonAIComponent's unit (tools/tu_map.py): whether the target is within reach. False
// without an AI component on the owner (+0x30, its +0x240) or without a target (fn_7100d7c990);
// otherwise the target's position, projected to 2D (fn_710007b430), is looked up in the AI
// component's area at +0x68 (fn_71001176e8, which writes a distance back into the position's first
// word), and the result holds when that distance is at most the owner's float at +0x380.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkTU108.h"
#include "types.h"

// As src/game/CShakernautAIComponent/CShakernautAIComponent_Reach_b65a4.cpp declares it.
struct UnkShakernautArea_b65a4;

struct UnkObsydomithonAI_b66a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

static inline __attribute__((always_inline)) base::math::CVector3D Position_b66a4(const UnkActor* a) {
    base::math::CVector3D v;
    v.x = a->field_0x44;
    v.y = a->field_0x54;
    v.z = a->field_0x64;
    return v;
}

bool fn_710062ea78(UnkObsydomithonAI_b66a4* self) {
    if (!self->owner->field_0x240)
        return false;
    if (!fn_7100d7c990(self->owner))
        return false;
    base::math::CVector2D pos2;
    UnkActor* target = fn_7100d7c990(self->owner);
    base::math::CVector3D pos = Position_b66a4(target);
    fn_710007b430(&pos2, &pos);
    UnkActor* owner = self->owner;
    u32 found;
    UnkShakernautArea_b65a4* area =
        reinterpret_cast<UnkShakernautArea_b65a4*>(static_cast<char*>(owner->field_0x240) + 0x68);
    f32 limit = *reinterpret_cast<f32*>(reinterpret_cast<char*>(owner) + 0x380);
    bool hit = fn_71001176e8(area, &pos2, reinterpret_cast<u32*>(&pos), &found, 1, 0.0f);
    return hit & (pos.x <= limit);
}
