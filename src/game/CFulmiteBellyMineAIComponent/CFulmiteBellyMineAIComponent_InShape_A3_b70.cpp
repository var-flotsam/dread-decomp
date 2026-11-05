// CFulmiteBellyMineAIComponent unit (tools/tu_map.py unit 826): whether the owner's position lies in the
// shape of the central unit's tracked component (UnkCentralShapeTest_A3_b70.h). Names and layouts are
// placeholders.
#include "UnkCentralShapeTest_A3_b70.h"

struct UnkMineAI_A3_b70 {
    u8 pad_0x0[0x30];
    UnkShapeOwner_A3_b70* owner;
};

bool fn_710057c778(UnkMineAI_A3_b70* self) {
    return CentralShapeTest_A3_b70([self] {
        base::math::CVector3D r;
        r.x = self->owner->x;
        r.y = self->owner->y;
        r.z = self->owner->z;
        return r;
    });
}
