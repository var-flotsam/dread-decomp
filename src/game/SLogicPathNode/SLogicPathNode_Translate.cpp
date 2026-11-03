// SLogicPathNode unit (unit 2657 of tools/tu_map.py): adds an offset to the position at +0x8 (the same code as
// src/game/SCameraRailNode/SCameraRailNode_Translate.cpp). Names and layouts are placeholders.
#include "types.h"

struct UnkLogicPathVec3 {
    f32 x, y, z;
};

struct UnkLogicPathPositioned {
    u8 pad_0x0[0x8];
    UnkLogicPathVec3 position;
};

// All three sums before the stores, in this order, as the original allocates its registers.
void fn_7101059098(UnkLogicPathPositioned* self, const UnkLogicPathVec3* offset) {
    f32 x = self->position.x + offset->x;
    f32 y = self->position.y + offset->y;
    f32 z = self->position.z + offset->z;
    self->position = {x, y, z};
}
