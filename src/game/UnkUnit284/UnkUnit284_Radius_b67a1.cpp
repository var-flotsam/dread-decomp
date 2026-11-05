// tools/tu_map.py unit 284 (no class hint): looks a float up by its name hash in the part held at
// +0x108 (fn_71001c32bc, as fn_710020dbd8 does), keeps it at +0x110 with a positive flag at +0xfd
// (FLT_MAX and on when it is missing), then calls fn_71001c3200 on the part (fn_7100207920).
// Names and layouts are placeholders.
#include <float.h>

#include "types.h"

struct UnkSource_710020c780;
bool fn_71001c32bc(UnkSource_710020c780* source, const u64* key, u32 flag, f32* out);
void fn_71001c3200(UnkSource_710020c780* source);

struct UnkOwner284_b67a1 {
    u8 pad_0x0[0xfd];
    bool positive;  // 0xfd
    u8 pad_0xfe[0xa];
    UnkSource_710020c780* part;  // 0x108
    f32 value;  // 0x110
};

void fn_7100207920(UnkOwner284_b67a1* self) {
    f32 out = FLT_MAX;
    u64 key = 0xd4fec4bd56e3c8bULL;
    bool positive = true;
    f32 v;
    if (fn_71001c32bc(self->part, &key, 1, &out)) {
        v = out;
        positive = v > 0.0f;
    } else {
        v = FLT_MAX;
    }
    self->value = v;
    self->positive = positive;
    fn_71001c3200(self->part);
}
