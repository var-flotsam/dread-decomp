// Unit 110 ('malloc' hint): contact points between two axis-aligned 2D boxes. Each corner of the overlap
// that lies on an edge of both boxes becomes a contact with two edge directions; with none and the flag set,
// the centre of the contained box is reported instead. Names and layouts are placeholders.
#include "types.h"

struct UnkBox2D {
    f32 field_0x0;  // min x
    f32 field_0x4;  // min y
    f32 field_0x8;  // max x
    f32 field_0xc;  // max y
};

struct UnkContactVec3_l12b330 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkContactPoint_l12b330 {
    UnkContactVec3_l12b330 field_0x0;   // position
    UnkContactVec3_l12b330 field_0xc;   // first edge direction
    UnkContactVec3_l12b330 field_0x18;  // second edge direction
    u32 field_0x24;
    u32 field_0x28;  // kind flags
};
static_assert(sizeof(UnkContactPoint_l12b330) == 0x2c);

template <u32 N>
struct UnkContactBuffer {
    u32 count;
    UnkContactPoint_l12b330 points[N];
};

namespace {

inline bool OffEdges(const UnkBox2D* box, f32 x, f32 y) {
    return x != box->field_0x0 && x != box->field_0x8 && y != box->field_0x4 && y != box->field_0xc;
}

inline bool Contains(const UnkBox2D* box, f32 x, f32 y) {
    return x >= box->field_0x0 && x <= box->field_0x8 && y >= box->field_0x4 && y <= box->field_0xc;
}

inline UnkContactPoint_l12b330* AddPoint(UnkContactBuffer<4>* out) {
    u32 index = 0xffffffff;
    if (out->count != 0xffffffff) {
        index = out->count;
        __builtin_memset(&out->points[index], 0, sizeof(UnkContactPoint_l12b330));
        out->count = index + 1;
    }
    return &out->points[index];
}

inline void SetCorner(UnkContactPoint_l12b330* point, f32 x, f32 y, f32 dirX, f32 dirY, f32 dir2X,
                      f32 dir2Y) {
    point->field_0x0.x = x;
    point->field_0x0.y = y;
    point->field_0x0.z = 0.0f;
    point->field_0xc.x = dirX;
    point->field_0xc.y = dirY;
    point->field_0xc.z = 0.0f;
    point->field_0x18.x = dir2X;
    point->field_0x18.y = dir2Y;
    point->field_0x18.z = 0.0f;
    point->field_0x24 = 0;
    point->field_0x28 = 0x38;
}

}  // namespace

bool fn_710012b330(const UnkBox2D* a, const void* b, UnkContactBuffer<4>* out, bool flag) {
    const UnkBox2D* other = static_cast<const UnkBox2D*>(b);

    f32 lowX = a->field_0x0 > other->field_0x0 ? a->field_0x0 : other->field_0x0;
    f32 lowY = a->field_0x4 > other->field_0x4 ? a->field_0x4 : other->field_0x4;
    f32 highX = a->field_0x8 < other->field_0x8 ? a->field_0x8 : other->field_0x8;
    f32 highY = a->field_0xc < other->field_0xc ? a->field_0xc : other->field_0xc;
    f32 width = highX - lowX;
    f32 height = highY - lowY;
    f32 overlap = height < width ? height : width;

    if (overlap >= 0.0f) {
        if (!OffEdges(a, lowX, highY) && !OffEdges(other, lowX, highY)) {
            bool left = a->field_0x0 < other->field_0x0;
            f32 dirX = left ? 0.0f : -1.0f;
            f32 dirY = left ? 1.0f : 0.0f;
            f32 dir2X = left ? -1.0f : 0.0f;
            f32 dir2Y = left ? 0.0f : 1.0f;
            SetCorner(AddPoint(out), lowX, highY, dirX, dirY, dir2X, dir2Y);
        }
        if (!OffEdges(a, highX, lowY) && !OffEdges(other, highX, lowY)) {
            bool right = a->field_0x8 > other->field_0x8;
            f32 dirX = right ? 0.0f : 1.0f;
            f32 dirY = right ? -1.0f : 0.0f;
            f32 dir2X = right ? 1.0f : 0.0f;
            f32 dir2Y = right ? 0.0f : -1.0f;
            SetCorner(AddPoint(out), highX, lowY, dirX, dirY, dir2X, dir2Y);
        }
        if (!OffEdges(a, lowX, lowY) && !OffEdges(other, lowX, lowY)) {
            bool left = a->field_0x0 < other->field_0x0;
            f32 dirX = left ? 0.0f : -1.0f;
            f32 dirY = left ? -1.0f : 0.0f;
            SetCorner(AddPoint(out), lowX, lowY, dirX, dirY, dirY, dirX);
        }
        if (!OffEdges(a, highX, highY) && !OffEdges(other, highX, highY)) {
            bool right = a->field_0x8 > other->field_0x8;
            f32 dirX = right ? 0.0f : 1.0f;
            f32 dirY = right ? 1.0f : 0.0f;
            SetCorner(AddPoint(out), highX, highY, dirX, dirY, dirY, dirX);
        }

        if (out->count == 0 && flag) {
            const UnkBox2D* inner;
            if (Contains(a, (other->field_0x0 + other->field_0x8) * 0.5f,
                         (other->field_0x4 + other->field_0xc) * 0.5f)) {
                out->count = 0;
                AddPoint(out);
                inner = other;
            } else if (Contains(other, a->field_0x0, a->field_0x4)) {
                out->count = 0;
                AddPoint(out);
                inner = a;
            } else {
                return out->count != 0;
            }
            f32 centreX = (inner->field_0x0 + inner->field_0x8) * 0.5f;
            f32 centreY = (inner->field_0x4 + inner->field_0xc) * 0.5f;
            UnkContactPoint_l12b330* point = &out->points[0];
            point->field_0x28 = 8;
            point->field_0x0.x = centreX;
            point->field_0x0.y = centreY;
            point->field_0x0.z = 0.0f;
        }
    }
    return out->count != 0;
}
