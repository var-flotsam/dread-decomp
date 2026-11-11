// CAreaFXComponent unit (tools/tu_map.py unit 1263): fn_71007fe090, the pair of points for two targets
// within a rectangle record: fn_71007fdd80 gives each point directly (0), says the target is inside
// (2, which sets *hit) or neither; for 0 the point comes from fn_71007fdc30 instead (the record layouts
// repeat CAreaFXComponent_Pair_A5_b66.cpp's). Names and layouts are placeholders.
#include "game/CAreaFXComponent.h"
#include "types.h"

typedef u32 __attribute__((may_alias)) a_u32_ovb0081;

struct UnkAreaFXRecord_A5_b66 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;

    UnkAreaFXRecord_A5_b66(const UnkAreaFXRecord_A5_b66& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0xc(other.field_0xc) {}
};

struct UnkAreaFXResult_A5_b66 {
    u64 field_0x0;

    UnkAreaFXResult_A5_b66(const UnkAreaFXResult_A5_b66& other);
};

struct UnkAreaFXPoint_ovb0081 {
    f32 x, y;
    UnkAreaFXPoint_ovb0081() : x(0.0f), y(0.0f) {}
    UnkAreaFXPoint_ovb0081(const UnkAreaFXPoint_ovb0081& o) : x(o.x), y(o.y) {}
    void Set(const UnkAreaFXResult_A5_b66& r) {
        ((a_u32_ovb0081*)this)[0] = ((const a_u32_ovb0081*)&r)[0];
        ((a_u32_ovb0081*)this)[1] = ((const a_u32_ovb0081*)&r)[1];
    }
};

struct UnkAreaFXPointPair_ovb0081 {
    UnkAreaFXPoint_ovb0081 first;
    UnkAreaFXPoint_ovb0081 second;
};

s32 fn_71007fdd80(UnkAreaFXRecord_A5_b66 record, void* target, UnkAreaFXPoint_ovb0081* out);

UnkAreaFXPointPair_ovb0081 fn_71007fe090(UnkAreaFXRecord_A5_b66 record, void* a, void* b, void* c, bool* hit) {
    UnkAreaFXPointPair_ovb0081 out;
    s32 r = fn_71007fdd80(record, b, &out.first);
    if (r == 0)
        out.first.Set(fn_71007fdc30(record, a, b));
    else if (r == 2)
        *hit = true;
    r = fn_71007fdd80(record, c, &out.second);
    if (r == 0)
        out.second.Set(fn_71007fdc30(record, a, c));
    else if (r == 2)
        *hit = true;
    return out;
}
