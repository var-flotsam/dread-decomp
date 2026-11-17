// Unit 109 (base::spatial::CPolygonCollection2D's): whether a point lies inside the collection's closed
// polygons. A ray from the point towards (1, 1) is cut with every edge near it; the nearest crossing
// decides, by which way that edge faces. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU108.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// As src/base/spatial/UnkPolygon2D.cpp defines them.
struct UnkPolygonVertex {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkPolygon2D {
    UnkPolygonVertex* vertices;
    u32 count;
    u8 unk_0xc[0x20];
    u8 closed;
    bool box_dirty;  // 0x2d: the box below is stale
    u8 unk_0x2e[0x2];
    CVector2D box_min;  // 0x30
    CVector2D box_max;  // 0x38
    u8 unk_0x40[0x8];
};
static_assert(sizeof(UnkPolygon2D) == 0x48, "UnkPolygon2D layout");

// As src/game/malloc/fn_710012be70.cpp defines it.
struct UnkContact_710012be70 {
    CVector3D point;
    CVector3D normal_a;
    CVector3D normal_b;
    u32 field_0x24;
    u32 flags;

    void SetPoint(const CVector3D& p) {
        point = p;
        flags |= 8;
    }
    void SetNormalA(const UnkVec2Result& n) {
        normal_a.x = n.x;
        normal_a.y = n.y;
        normal_a.z = 0.0f;
        flags |= 0x20;
    }
    void SetNormalB(const UnkVec2Result& n) {
        normal_b.x = n.x;
        normal_b.y = n.y;
        normal_b.z = 0.0f;
        flags |= 0x10;
    }
};

bool fn_710012be70(const UnkSegment2D* a, const UnkSegment2D* b, UnkContact_710012be70* out,
                   bool collinear, f32 tolerance);

struct UnkPolygonList_l1247c8 {
    UnkPolygon2D* polygons;
    u32 count;
};

// The ray's direction.
static const f32 kRay = 0.70710677f;

inline u32 EdgeCount(const UnkPolygon2D* p) {
    return p->count - (!p->closed & (p->count != 0));
}

inline bool Near(f32 a, f32 b) {
    return (a + 0.001f >= b) & (a - 0.001f <= b);
}

// Whether `v` lies between `lo` and `hi`.
inline bool InRange(f32 v, f32 lo, f32 hi) {
    return v <= hi && !(v < lo);
}

inline bool Near(const CVector2D& a, const CVector2D& b) {
    return Near(a.x, b.x) && Near(a.y, b.y);
}

inline f32 DistanceSq(const CVector2D& a, const CVector2D& b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    return dx * dx + dy * dy;
}

inline CVector2D Normalized(f32 x, f32 y) {
    f32 length = sqrtf(x * x + y * y);
    if (length != 0.0f) {
        f32 inv = 1.0f / length;
        x *= inv;
        y *= inv;
    }
    return CVector2D{x, y};
}

// How far the segment's normal points along the ray.
inline f32 Facing(const UnkSegment2D& seg) {
    UnkVec2Result n = fn_7100125e78(&seg);
    return n.x * kRay + n.y * kRay;
}

bool fn_71001247c8(const UnkPolygonList_l1247c8* list, const CVector2D* point) {
    f32 far_x = point->x + 100000.0f;
    f32 far_y = point->y + 100000.0f;
    UnkSegment2D ray(*point, CVector2D{far_x, far_y});
    UnkSegment2D best;
    best.start.x = 0.0f;
    best.start.y = 0.0f;
    best.end.x = 0.0f;
    best.end.y = 0.0f;
    best.field_0x10 = -1.0f;
    f32 best_distance = -1.0f;

    for (u32 n = list->count; n != 0; --n) {
        UnkPolygon2D* polygon = &list->polygons[n - 1];
        if (!polygon->closed)
            continue;
        if (polygon->box_dirty)
            fn_7100119b88(polygon);
        if (!InRange(point->x, polygon->box_min.x - 0.1f, polygon->box_max.x + 0.1f))
            continue;
        if (!InRange(point->y, polygon->box_min.y - 0.1f, polygon->box_max.y + 0.1f))
            continue;

        // The polygon's crossings count only if the nearest of them faces away.
        UnkSegment2D saved;
        saved.start.x = best.start.x;
        saved.start.y = best.start.y;
        saved.end.x = best.end.x;
        saved.end.y = best.end.y;
        saved.field_0x10 = -1.0f;
        f32 saved_distance = best_distance;
        bool away = true;
        CVector2D last = {0.0f, 0.0f};
        for (u32 i = EdgeCount(polygon); i != 0; --i) {
            UnkPolygonVertex* v = polygon->vertices;
            UnkSegment2D seg(v[i - 1].pos, (i == polygon->count ? v : &v[i])->pos);
            // A point on a corner is inside.
            if (Near(*point, seg.start) || Near(*point, seg.end))
                return true;
            UnkContact_710012be70 contact = {};
            if (!fn_710012be70(&ray, &seg, &contact, false, 0.0f))
                continue;
            CVector2D hit;
            fn_710007b430(&hit, &contact.point);
            bool facing = true;
            if (Near(hit, seg.start) || Near(hit, seg.end))
                facing = !(Facing(seg) > 0.0f);

            if (best_distance < 0.0f) {
                fn_7100125b44(&best, &seg);
                last = hit;
                best_distance = DistanceSq(hit, *point);
                away = facing;
                continue;
            }
            f32 distance = DistanceSq(hit, *point);
            if (!(best_distance + 0.1f >= distance && best_distance - 0.1f <= distance)) {
                if (!(distance < best_distance))
                    continue;
                if (away)
                    facing = true;
                else if (DistanceSq(hit, last) <= 0.1f)
                    facing = false;
            } else {
                // As near as the best: the edge turned further from the ray wins.
                CVector2D a;
                CVector2D b;
                if (Near(seg.start, best.end)) {
                    a = Normalized(seg.end.x - seg.start.x, seg.end.y - seg.start.y);
                    b = CVector2D{best.start.x - best.end.x, best.start.y - best.end.y};
                } else {
                    a = Normalized(seg.start.x - seg.end.x, seg.start.y - seg.end.y);
                    b = CVector2D{best.end.x - best.start.x, best.end.y - best.start.y};
                }
                b = Normalized(b.x, b.y);
                f32 along_a = a.x * kRay + a.y * kRay;
                f32 along_b = b.x * kRay + b.y * kRay;
                if (!(along_a < along_b)) {
                    if (along_a != along_b || !(Facing(best) >= 0.0f))
                        continue;
                }
                if (away)
                    facing = true;
                else if (!facing && DistanceSq(hit, last) <= 0.1f)
                    facing = false;
            }
            fn_7100125b44(&best, &seg);
            last = hit;
            best_distance = distance;
            away = facing;
        }
        if (!away) {
            fn_7100125b44(&best, &saved);
            best_distance = saved_distance;
        }
    }
    if (best_distance < 0.0f)
        return false;
    return Facing(best) < 0.0f;
}
