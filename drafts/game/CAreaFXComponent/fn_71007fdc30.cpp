// CAreaFXComponent unit (tools/tu_map.py unit 1263): the point a target is drawn to within a rectangle
// record: the target's point clamped into the rectangle, then the first collision hit on the segment from
// there to the target, or the target's own point when nothing is hit. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "game/CAreaFXComponent.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;

// The rectangle: its low corner, then its high corner.
struct UnkAreaFXRecord_A5_b66 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;

    UnkAreaFXRecord_A5_b66(const UnkAreaFXRecord_A5_b66& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0xc(other.field_0xc) {}
};

struct UnkAreaFXResult_A5_b66 {
    f32 x, y;

    UnkAreaFXResult_A5_b66(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkAreaFXResult_A5_b66(const UnkAreaFXResult_A5_b66& other);
};

struct UnkAreaFXTarget_l7fdc30 {
    u8 pad_0x0[0x14];
    f32 x;  // 0x14
    f32 y;  // 0x18
};

struct UnkHitVec_l7fdc30 {
    f32 x, y, z;
    UnkHitVec_l7fdc30(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

// A collision hit (0x48 bytes).
struct UnkCollisionHit_l7fdc30 {
    base::UnkRefPtrMember object;
    base::UnkRefPtrMember field_0x8;
    s32 id;
    UnkHitVec_l7fdc30 point;
    UnkHitVec_l7fdc30 field_0x20;
    UnkHitVec_l7fdc30 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l7fdc30()
        : object{nullptr}, field_0x8{nullptr}, id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f),
          normal(0.0f, 0.0f, 0.0f), field_0x38(0), flags(0), field_0x40(0) {}
};
static_assert(sizeof(UnkCollisionHit_l7fdc30) == 0x48, "UnkCollisionHit_l7fdc30 layout");

// The segment query's base callback object: the closest hit so far.
struct UnkHitQuery_l7fdc30 {
    void** vtable;
    UnkCollisionHit_l7fdc30 hit;

    UnkHitQuery_l7fdc30() { vtable = g_unk_7101a20ed8 + 2; }
    ~UnkHitQuery_l7fdc30() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkHitSegment_l7fdc30 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkHitSegment_l7fdc30(f32 sx, f32 sy, f32 ex, f32 ey) {
        start.x = sx;
        start.y = sy;
        end.x = ex;
        end.y = ey;
        delta.x = ex - sx;
        delta.y = ey - sy;
    }
};

// v moved into [lo, hi].
static inline f32 ClampInto_l7fdc30(f32 v, f32 lo, const f32& hi) {
    if (lo > v)
        return lo;
    if (hi < v)
        return hi;
    return v;
}

UnkAreaFXResult_A5_b66 fn_71007fdc30(UnkAreaFXRecord_A5_b66 record, void* a, void* b) {
    UnkTree* tree = static_cast<UnkTree*>(a);
    const UnkAreaFXTarget_l7fdc30* target = static_cast<const UnkAreaFXTarget_l7fdc30*>(b);
    f32 x = target->x;
    f32 y = target->y;
    f32 nx = ClampInto_l7fdc30(x, record.field_0x0, record.field_0x8);
    f32 ny = ClampInto_l7fdc30(y, record.field_0x4, record.field_0xc);
    UnkHitSegment_l7fdc30 segment(nx, ny, x, y);
    UnkHitQuery_l7fdc30 query;
    if (fn_7100d5c518(tree, 1, reinterpret_cast<const UnkBox*>(&segment), &query, 2))
        return UnkAreaFXResult_A5_b66(query.hit.point.x, query.hit.point.y);
    return UnkAreaFXResult_A5_b66(x, y);
}
