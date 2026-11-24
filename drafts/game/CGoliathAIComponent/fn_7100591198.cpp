// CGoliathAIComponent's unit (tools/tu_map.py unit 844): rebuilds the Goliath's 2D collision shape from its
// path: an outline over the first and last segments joined by two quarter-circle sweeps, and a box beside
// them. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/CCharClassGoliathAttack.h"
#include "game/UnkTU108.h"
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkVec2;
struct UnkPointSet_b61;
void* fn_71003e6530(void* owner, const UnkVec2* origin, const UnkVec2* dir, void* sink, f32 radius, f32 limit);
void* fn_7100118950(UnkPointSet_b61* self, u32 index);

// An outline: its points (fn_7100118b94 appends one), a z, flags, its bounds and a word; the
// outline set copies it (0x48 bytes).
struct UnkTilePoint {
    f32 x, y, z;
};
struct UnkTileOutline {
    base::global::CRntVector<UnkTilePoint> points;
    f32 field_0x28;
    bool field_0x2c;
    bool field_0x2d;
    base::math::CVector2D min;
    base::math::CVector2D max;
    u32 field_0x40;

    UnkTileOutline()
        : points(7), field_0x28(0.0f), field_0x2c(false), field_0x2d(true), min{3.4028235e38f, 3.4028235e38f},
          max{-3.4028235e38f, -3.4028235e38f}, field_0x40(0) {}
    ~UnkTileOutline() { points.Free(); }
};
static_assert(sizeof(UnkTileOutline) == 0x48, "UnkTileOutline placeholder layout");

// The tunables this reads: a half width at +0x34 and a height at +0x40.
struct UnkGoliathTunables_l591198 {
    u8 pad_0x0[0x34];
    f32 field_0x34;
    u8 pad_0x38[0x8];
    f32 field_0x40;
};

// The Goliath's path: its points, then whether it is closed (a byte flag).
struct UnkGoliathPath_l591198 {
    base::global::CRntVector<UnkTilePoint> points;
    f32 field_0x28;
    u8 closed;  // 0x2c
    u8 pad_0x2d[0x1b];
};
static_assert(sizeof(UnkGoliathPath_l591198) == 0x48, "UnkGoliathPath_l591198");

struct UnkGoliathAI_l591198 {
    void** vtable;
    u8 pad_0x8[0x60];
    UnkGoliathPath_l591198 path;  // 0x68
    u8 pad_0xb0[0x14a30 - 0xb0];
    void* field_0x14a30;

    f32 vf_0x3c8() { return reinterpret_cast<f32 (*)(UnkGoliathAI_l591198*)>(vtable[0x3c8 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkGoliathAI_l591198, field_0x14a30) == 0x14a30, "UnkGoliathAI_l591198");

struct UnkGoliathShape_l591198 {
    u8 pad_0x0[0x98];
    bool field_0x98;  // set once the outlines changed
};

static inline const CVector2D& XY(const UnkTilePoint& p) {
    return reinterpret_cast<const CVector2D&>(p);
}

// The number of segments: one per point when the outline is closed, one fewer when it is open.
static inline u32 SegmentCount(const UnkGoliathPath_l591198& o) {
    return o.points.count - ((o.points.count != 0) & (o.closed == 0));
}

// Segment `i`: from point i to the next one, the last one wrapping round to the first.
static inline UnkSegment2D Segment(const UnkGoliathPath_l591198& o, u32 i) {
    const UnkTilePoint* p = o.points.data;
    return UnkSegment2D(XY(p[i]), XY(i + 1 == o.points.count ? p[0] : p[i + 1]));
}

static inline void AddPoint(UnkTileOutline* outline, f32 x, f32 y) {
    UnkTilePoint point;
    point.x = x;
    point.y = y;
    point.z = 0.0f;
    fn_7100118b94(outline, reinterpret_cast<UnkVec3*>(&point));
}

// The same, through the caller's point.
static inline void AddPoint(UnkTileOutline* outline, UnkTilePoint& point, f32 x, f32 y) {
    point.x = x;
    point.y = y;
    point.z = 0.0f;
    fn_7100118b94(outline, reinterpret_cast<UnkVec3*>(&point));
}

// Appends a copy of the outline's point `i`.
static inline void AddPointOf(UnkTileOutline* outline, UnkTileOutline* from, s32 i) {
    const UnkTilePoint* e = static_cast<const UnkTilePoint*>(fn_7100118950(reinterpret_cast<UnkPointSet_b61*>(from), i));
    UnkTilePoint point;
    point.x = e->x;
    point.y = e->y;
    point.z = e->z;
    fn_7100118b94(outline, reinterpret_cast<UnkVec3*>(&point));
}

// Sweeps a quarter circle of radius 800 from `origin` into `arc`, to the side `angles` picks, as far as
// the Goliath's reach.
static inline void SweepArc(UnkGoliathAI_l591198* self, const CVector2D& origin, const CVector3D& angles,
                            UnkTileOutline* arc) {
    fn_71003e6530(self, reinterpret_cast<const UnkVec2*>(&origin), reinterpret_cast<const UnkVec2*>(&angles), arc,
                  800.0f, self->vf_0x3c8());
}

static inline CVector3D Yaw(f32 y) {
    CVector3D v;
    v.x = 0.0f;
    v.y = y;
    v.z = 0.0f;
    return v;
}

// Empties the outline and opens it again; its bounds stay.
static inline void Reset(UnkTileOutline* o) {
    o->field_0x28 = 0.0f;
    o->points.count = 0;
    o->field_0x2c = false;
    o->field_0x2d = true;
}

bool fn_7100591198(UnkGoliathAI_l591198* self, UnkGoliathShape_l591198* shape) {
    UnkTileOutlines* outlines = reinterpret_cast<UnkTileOutlines*>(shape);
    fn_7100d633ec(outlines);
    if (SegmentCount(self->path) == 0 || !self->field_0x14a30)
        return false;

    UnkTileOutline sweep;
    UnkSegment2D first = Segment(self->path, 0);
    UnkSegment2D last = Segment(self->path, SegmentCount(self->path) - 1);
    f32 leftX = first.start.x + 200.0f;
    f32 leftY = first.start.y;
    f32 rightX = last.end.x - 200.0f;
    f32 rightY = last.end.y;
    AddPoint(&sweep, leftX, leftY + 800.0f);
    AddPoint(&sweep, rightX, rightY + 800.0f);

    UnkTileOutline arc;
    CVector2D right = {rightX, rightY};
    SweepArc(self, right, Yaw(1.5707964f), &arc);
    for (s32 i = 0; i < (s32)arc.points.count; ++i)
        AddPointOf(&sweep, &arc, i);

    Reset(&arc);
    CVector2D left = {leftX, leftY};
    SweepArc(self, left, Yaw(-1.5707964f), &arc);
    for (s32 i = (s32)arc.points.count - 1; i >= 0; --i)
        AddPointOf(&sweep, &arc, i);
    SetPolylineClosed(&sweep, true);
    fn_7100d62d40(outlines, &sweep);

    UnkTileOutline box;
    UnkTilePoint corner;
    UnkGoliathTunables_l591198* t = reinterpret_cast<UnkGoliathTunables_l591198*>(&g_unk_71726e5e70);
    f32 bottom = left.y;
    f32 x0 = left.x - (t->field_0x34 + 800.0f + 150.0f);
    AddPoint(&box, corner, x0, bottom);
    AddPoint(&box, corner, x0, t->field_0x40 + bottom);
    f32 x1 = right.x + (t->field_0x34 + 800.0f - 150.0f);
    f32 top = right.y + t->field_0x40;
    AddPoint(&box, corner, x1, top);
    AddPoint(&box, corner, x1, top - t->field_0x40);
    SetPolylineClosed(&box, true);
    fn_7100d62d40(outlines, &box);
    shape->field_0x98 = true;
    return true;
}
