// CCharClassBaseBigFistAIComponent unit (tools/tu_map.py): picks one of two adjacent 8-byte
// entries (g_unk_71726c1278) by the sign of the x axis component of the owner transform's
// matrix (+0x38). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"

extern u64 g_unk_71726c1278[2] __attribute__((visibility("hidden")));

struct UnkBigFistOwner {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

const u64* fn_71003e8f4c(void* self, const UnkBigFistOwner* owner) {
    const UnkMatrix44& m = fn_710002614c(&owner->transform);
    const f32 axis[3] = {m.m[2], m.m[6], m.m[10]};
    const f32 x[3] = {1.0f, 0.0f, 0.0f};
    f32 d = axis[0] * x[0];
    for (int i = 1; i < 3; ++i)
        d += axis[i] * x[i];
    return d > 0.0f ? &g_unk_71726c1278[0] : &g_unk_71726c1278[1];
}
