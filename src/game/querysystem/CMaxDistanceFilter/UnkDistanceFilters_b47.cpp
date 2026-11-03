// querysystem::CMaxDistanceFilter and querysystem::CMinDistanceFilter (tools/tu_map.py units 387 and 389,
// the names from reflection): whether the second actor passes the radius at +0x14 around the first
// (positions at +0x44, +0x54, +0x64); a radius that is not positive passes everything. Names are placeholders.
#include "game/UnkActor.h"
#include "types.h"

struct UnkDistanceFilter_b47 {
    u8 pad_0x0[0x14];
    f32 radius;  // 0x14
};

static inline f32 DistSq_b47(const UnkActor* a, const UnkActor* b) {
    f32 dx = a->field_0x44 - b->field_0x44;
    f32 dy = a->field_0x54 - b->field_0x54;
    f32 dz = a->field_0x64 - b->field_0x64;
    return dx * dx + dy * dy + dz * dz;
}

// querysystem::CMaxDistanceFilter: rejects the second actor when it is beyond the radius.
bool fn_71002c2f40(UnkDistanceFilter_b47* self, UnkActor* first, UnkActor* second) {
    if (self->radius > 0.0f) {
        f32 d2 = DistSq_b47(first, second);
        f32 radius = self->radius;
        if (radius * radius >= d2)
            return false;
    }
    return true;
}

// querysystem::CMinDistanceFilter: rejects the second actor when it is within the radius.
bool fn_71002c3e90(UnkDistanceFilter_b47* self, UnkActor* first, UnkActor* second) {
    if (self->radius > 0.0f) {
        f32 d2 = DistSq_b47(first, second);
        f32 radius = self->radius;
        if (radius * radius <= d2)
            return false;
    }
    return true;
}
