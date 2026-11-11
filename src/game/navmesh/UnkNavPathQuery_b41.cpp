// A path query (tools/tu_map.py unit 1130, no class name). fn_710070699c: sets the target position
// at +0x18; when it moved (more than 0.001 on some axis), it keeps the new one, hands the path at
// +0x12ab8 to the object at +0x25540 when +0x12ac0 is set (fn_71002ad420), clears both paths
// (+0x12ab8 and +0x30, fn_71002adc8c) and clears +0x37fdc. Names and layouts are placeholders.
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

struct UnkVec3_np41 {
    f32 x, y, z;
};

struct UnkSelf_np41 {
    u8 pad_0x0[0x18];
    UnkVec3_np41 target;  // 0x18
    u8 pad_0x24[0xc];
    u8 path_0x30[0x12a88];
    u8 path_0x12ab8[0x8];
    u8 field_0x12ac0;
    u8 pad_0x12ac1[0x12a7f];
    u8 field_0x25540[0x12a9c];
    u8 field_0x37fdc;
};

static inline bool Near_np41(f32 a, f32 b) {
    f32 lo = a - 0.001f;
    f32 hi = a + 0.001f;
    return lo <= b && hi >= b;
}

static inline bool Near_np41(const UnkVec3_np41& a, const UnkVec3_np41& b) {
    return Near_np41(a.x, b.x) && Near_np41(a.y, b.y) && Near_np41(a.z, b.z);
}

void fn_710070699c(UnkSelf_np41* self, const UnkVec3_np41* target) {
    if (!Near_np41(self->target, *target)) {
        self->target.x = target->x;
        asm("" ::: "memory");
        self->target.y = target->y;
        self->target.z = target->z;
        u8& dirty = self->field_0x37fdc;
        u8* path = self->path_0x12ab8;
        if (self->field_0x12ac0)
            fn_71002ad420(self->field_0x25540, path);
        fn_71002adc8c(path);
        fn_71002adc8c(self->path_0x30);
        dirty = 0;
    }
}
