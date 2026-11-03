// SCameraRailNode unit (unit 1934 of tools/tu_map.py): two identical helpers that add an offset to the
// position at +0x8 (a camera rail node's, by their neighbours). Names and layouts are placeholders.
#include "types.h"

struct UnkRailVec3 {
    f32 x, y, z;
};

struct UnkRailPositioned {
    u8 pad_0x0[0x8];
    UnkRailVec3 position;
};

// All three sums before the stores, in this order, as the original allocates its registers.
#define RAIL_TRANSLATE(self, offset)                                         do {                                                                         f32 x = (self)->position.x + (offset)->x;                                f32 y = (self)->position.y + (offset)->y;                                f32 z = (self)->position.z + (offset)->z;                                (self)->position = {x, y, z};                                        } while (0)

void fn_7100c03868(UnkRailPositioned* self, const UnkRailVec3* offset) { RAIL_TRANSLATE(self, offset); }
void fn_7100c03890(UnkRailPositioned* self, const UnkRailVec3* offset) { RAIL_TRANSLATE(self, offset); }
