// tools/tu_map.py unit 110: a polygon's vertex at an index (the polygon of fn_7100130ab0's callers in
// src/engine/SystemStartup.cpp), base::global::CRntVector::GrowAt inlined: an index past the end grows the
// vertex array first. Names and layouts are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/global/CRntVector.h"
#include "base/math/UnkZeroVector3.h"
#include "types.h"

using base::math::CVector3D;

// A polygon vertex: a flag word, the angle at the vertex and the position (the shared zero vector
// until set). The angle is copied bitwise.
struct UnkPolygonVertex {
    u16 field_0x0;
    f32 angle;
    const CVector3D* pos;

    UnkPolygonVertex() : field_0x0(0), angle(0.0f) {
        const CVector3D* zero = &g_unk_710164779c;
        pos = zero;
    }
    UnkPolygonVertex(const UnkPolygonVertex& o) : field_0x0(o.field_0x0) {
        angle = o.angle;
        pos = o.pos;
    }
};

struct UnkPolygon {
    base::global::CRntVector<UnkPolygonVertex> vertices;
};

UnkPolygonVertex* fn_7100130ab0(UnkPolygon* self, u32 index) {
    return &self->vertices.GrowAt(index);
}
