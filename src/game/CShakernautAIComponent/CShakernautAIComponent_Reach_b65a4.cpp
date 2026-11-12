// CShakernautAIComponent's unit (tools/tu_map.py): whether the target can be reached. False when
// fn_71006b1f04 says so, when there is no target actor (fn_7100d7c990 of the owner at +0x30), or when
// its helper (+0x208) is missing or not ready (fn_71009e12d0); otherwise the helper's position
// (fn_71009ebe68), projected to 2D (fn_710007b430), is looked up within 250 in the area at +0x68
// (fn_71001176e8). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkTU108.h"
#include "game/UnkXformNode.h"
#include "types.h"

// As src/game/CBreakableTileGroup.cpp declares them.
struct UnkHelper;
bool fn_71009e12d0(void*);
UnkVec3f fn_71009ebe68(UnkHelper* helper);

struct UnkShakernautArea_b65a4;

struct UnkShakernautAI_b65a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x30];
    UnkShakernautArea_b65a4* area[1];  // 0x68 (the area object starts here)
};

bool fn_71006b1f04(UnkShakernautAI_b65a4* self);

bool fn_71006af848(UnkShakernautAI_b65a4* self) {
    if (!fn_71006b1f04(self)) {
        UnkActor* target = fn_7100d7c990(self->owner);
        if (target && target->field_0x208 && fn_71009e12d0(static_cast<UnkHelper*>(target->field_0x208))) {
            u32 found;
            base::math::CVector2D pos2;
            UnkShakernautArea_b65a4* area = reinterpret_cast<UnkShakernautArea_b65a4*>(self->area);
            UnkVec3f pos = fn_71009ebe68(static_cast<UnkHelper*>(target->field_0x208));
            fn_710007b430(&pos2, reinterpret_cast<const base::math::CVector3D*>(&pos));
            if (fn_71001176e8(area, &pos2, &found, nullptr, 0, 250.0f))
                return true;
        }
    }
    return false;
}
