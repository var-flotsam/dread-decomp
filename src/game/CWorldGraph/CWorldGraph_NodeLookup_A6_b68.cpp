// CWorldGraph's unit: for a node (null gives null), project its position (x, y, z at +0x44/+0x54/+0x64) to 2D
// with fn_710007b430 and look it up with fn_7100acc650 together with the node's data at +0x30. Names are
// placeholders.
#include "base/math/CVector.h"

struct UnkWorldGraphNode_A6_b68 {
    u8 unk_0x0[0x30];
    u8 field_0x30[0x14];
    f32 x;
    u8 unk_0x48[0xc];
    f32 y;
    u8 unk_0x58[0xc];
    f32 z;
};

void* fn_7100acc650(void* self, void* data, const base::math::CVector2D* pos);

void* fn_7100acc5ec(void* self, UnkWorldGraphNode_A6_b68* node) {
    if (!node)
        return nullptr;
    f32 x = node->x;
    f32 y = node->y;
    f32 z = node->z;
    void* data = node->field_0x30;
    base::math::CVector2D pos;
    base::math::CVector3D pos3 = {x, y, z};
    fn_710007b430(&pos, &pos3);
    return fn_7100acc650(self, data, &pos);
}
