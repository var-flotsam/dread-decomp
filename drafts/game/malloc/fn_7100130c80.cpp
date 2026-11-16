// malloc unit (tools/tu_map.py unit 110): finds an ear of a polygon for ear clipping, the corner with the
// smallest angle whose triangle holds no other vertex (corners that fail are set aside and the next
// smallest tried); 0xffff when there is none. Names and layouts are placeholders.
#include <float.h>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "types.h"

using base::global::CRntVector;
using base::math::CVector3D;

// A polygon of the same unit: its vertices (fn_7100130ab0, growing the storage on demand) and
// their count at +8.
struct UnkPolygon {
    u8 pad_0x0[0x8];
    u32 count;
};

struct UnkPolygonVertex;
UnkPolygonVertex* fn_7100130ab0(UnkPolygon* self, u32 index);
void fn_71001306b0(UnkPolygon* self);

// A vertex record: its angle and its position.
struct UnkPolygonCorner {
    u32 field_0x0;
    f32 angle;
    const CVector3D* pos;
};

static inline UnkPolygonCorner* Corner(UnkPolygon* self, u32 index) {
    return reinterpret_cast<UnkPolygonCorner*>(fn_7100130ab0(self, index));
}

static inline bool Contains(const CRntVector<u32>& v, u32 value) {
    for (u32 i = 0; i < v.count; ++i)
        if (v.data[i] == value)
            return true;
    return false;
}

// Whether p lies inside the triangle p0, p1, p2 (either winding), by its barycentric coordinates.
inline bool IsInTriangle(const CVector3D& p, const CVector3D& p0, const CVector3D& p1, const CVector3D& p2) {
    f32 s = p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y;
    f32 t = p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y;
    if ((s < 0.0f) != (t < 0.0f))
        return false;
    f32 area = -p1.y * p2.x + p0.y * (p2.x - p1.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y;
    if (area < 0.0f) {
        s = -s;
        t = -t;
        area = -area;
    }
    return s > 0.0f && t > 0.0f && s + t <= area;
}

u16 fn_7100130c80(UnkPolygon* self) {
    fn_71001306b0(self);
    u16 n = self->count;
    CRntVector<u32> rejected(0xf);
    rejected.Reserve(n);
    u16 result = 0xffff;
    while (rejected.count < n) {
        // The smallest corner not tried yet.
        f32 best = FLT_MAX;
        u16 cur = 0;
        for (u16 i = 0; i < n; ++i) {
            if (best > Corner(self, i)->angle && !Contains(rejected, i)) {
                best = Corner(self, i)->angle;
                cur = i;
            }
        }
        const CVector3D* a = Corner(self, cur)->pos;
        u16 prev = cur == 0 ? n - 1 : cur - 1;
        const CVector3D* b = Corner(self, prev)->pos;
        u16 next = cur == n - 1 ? 0 : cur + 1;
        const CVector3D* c = Corner(self, next)->pos;
        bool ear = true;
        for (u16 j = 0; j < n; ++j) {
            if (j != next && j != cur && j != prev && IsInTriangle(*Corner(self, j)->pos, *a, *b, *c)) {
                ear = false;
                break;
            }
        }
        if (ear) {
            result = cur;
            break;
        }
        rejected.PushBack(cur);
    }
    rejected.Free();
    return result;
}
