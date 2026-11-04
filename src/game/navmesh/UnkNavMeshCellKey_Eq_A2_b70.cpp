// Unit 362 (tools/tu_map.py; no class hint): equality and inequality of a navmesh key (a u16, a byte and two
// floats compared within 0.001). Names are placeholders.
#include "types.h"

struct UnkNavMeshKey_A2_b70 {
    u16 field_0x0;
    u8 field_0x2;
    f32 x;
    f32 y;
};

static inline bool Near_A2_b70(f32 a, f32 b) {
    return a + -0.001f <= b && a + 0.001f >= b;
}

bool fn_7100287610(const UnkNavMeshKey_A2_b70* a, const UnkNavMeshKey_A2_b70* b) {
    return a->field_0x0 == b->field_0x0 && a->field_0x2 == b->field_0x2 && Near_A2_b70(a->x, b->x) &&
           Near_A2_b70(a->y, b->y);
}

bool fn_71002876ac(const UnkNavMeshKey_A2_b70* a, const UnkNavMeshKey_A2_b70* b) {
    return !(a->field_0x0 == b->field_0x0 && a->field_0x2 == b->field_0x2 && Near_A2_b70(a->x, b->x) &&
             Near_A2_b70(a->y, b->y));
}
