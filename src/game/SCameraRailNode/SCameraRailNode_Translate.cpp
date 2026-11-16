// SCameraRailNode unit (unit 1934 of tools/tu_map.py): two identical helpers that add an offset to the
// position at +0x8 (a camera rail node's, by their neighbours). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector3D;

struct UnkRailPositioned {
    u8 pad_0x0[0x8];
    CVector3D position;
};

// All three sums before the stores, in this order, as the original allocates its registers.
#define RAIL_TRANSLATE(self, offset)                                         do {                                                                         f32 x = (self)->position.x + (offset)->x;                                f32 y = (self)->position.y + (offset)->y;                                f32 z = (self)->position.z + (offset)->z;                                (self)->position = {x, y, z};                                        } while (0)

void fn_7100c03868(UnkRailPositioned* self, const CVector3D* offset) { RAIL_TRANSLATE(self, offset); }
void fn_7100c03890(UnkRailPositioned* self, const CVector3D* offset) { RAIL_TRANSLATE(self, offset); }
