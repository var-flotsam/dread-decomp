// Unit 1387 (tools/tu_map.py, no name hint): points the light reached through the owner's +0x138 and +0x78
// (as fn_71008e966c in src/game/UnkTU1386/UnkTU1386_Light_ovb0187.cpp) from one position to another: the unit
// direction from `from` to `to` (unchanged when it has no length) into its vector at +0x30, dirty flag
// (+0x1b8), then updates the +0x138 object (fn_71001bdc3c). Names and layouts are placeholders.
#include <math.h>

#include "types.h"

void fn_710020ca64(void* member);
void fn_71001bdc3c(void* object);

struct UnkLight_b0187j {
    u8 pad_0x0[0x30];
    f32 direction[3];  // 0x30
    u8 pad_0x3c[0x17c];
    bool dirty;  // 0x1b8
};
static_assert(__builtin_offsetof(UnkLight_b0187j, dirty) == 0x1b8, "UnkLight_b0187j layout");

struct UnkLightHolder_b0187j {
    u8 pad_0x0[0x78];
    UnkLight_b0187j* light;  // 0x78
};

struct UnkLightOwner_b0187j {
    u8 pad_0x0[0x20];
    u8 member[0x118];  // 0x20
    UnkLightHolder_b0187j* holder;  // 0x138
};

struct UnkLightSelf_b0187j {
    u8 pad_0x0[0x120];
    UnkLightOwner_b0187j* owner;  // 0x120
};

void fn_71008ea140(UnkLightSelf_b0187j* self, const f32* from, const f32* to) {
    fn_710020ca64(self->owner->member);
    f32 dx = to[0] - from[0];
    f32 dy = to[1] - from[1];
    f32 dz = to[2] - from[2];
    f32 lengthSq = dx * dx + dy * dy + dz * dz;
    if (sqrtf(lengthSq) > 0.0f) {
        f32 inv = 1.0f / (sqrtf(lengthSq) + 0.0001f);
        dx *= inv;
        dy *= inv;
        dz *= inv;
    }
    UnkLightHolder_b0187j* holder = self->owner->holder;
    UnkLight_b0187j* light = holder->light;
    light->direction[0] = dx;
    light->direction[1] = dy;
    light->direction[2] = dz;
    light->dirty = true;
    fn_71001bdc3c(holder);
}
