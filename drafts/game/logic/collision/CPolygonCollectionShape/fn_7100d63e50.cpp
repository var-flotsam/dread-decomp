// game::logic::collision::CPolygonCollectionShape's unit: a type-8 shape against every edge of every
// polygon of a collection; every contact found goes to the query's sink. Names and layouts are
// placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU109.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkContact {
    u32 value;

    UnkContact() { value = 0; }
    UnkContact(const UnkContact& other) { value = other.value; }
};

inline const UnkContact& NoContact() {
    static UnkContact s_none;
    return s_none;
}

struct UnkContactPoint {
    CVector3D point;
    CVector3D normal_a;
    CVector3D normal_b;
    u32 field_0x24;
    u32 flags;
};
static_assert(sizeof(UnkContactPoint) == 0x2c, "UnkContactPoint layout");

struct UnkPolygonVertex {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkPolygon2D {
    UnkPolygonVertex* vertices;
    u32 count;
    u8 unk_0xc[0x20];
    u8 closed;
    u8 unk_0x2d[0x1b];
};
static_assert(sizeof(UnkPolygon2D) == 0x48, "UnkPolygon2D layout");

struct UnkPolygonCollection2D {
    UnkPolygon2D* polygons;
    u32 count;
};

struct UnkPolygonHit {
    const UnkPolygon2D* polygon;
    u32 handle;
    UnkContactPoint point;
};
static_assert(sizeof(UnkPolygonHit) == 0x38, "UnkPolygonHit layout");

struct UnkLocalQuery {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkContact vf_0x10(UnkPolygonHit hit);
    CVector3D offset;
    u16 field_0x14;
    const void* field_0x18;
    void* field_0x20;
};

struct UnkShape8Geometry {
    f32 x, y;
    f32 field_0x8;
    f32 field_0xc;
};

// The number of edges: an open polygon has none from its last vertex back to the first.
static inline u32 EdgeCount(const UnkPolygon2D& p) {
    return p.count - (!p.closed & (p.count != 0));
}

template <u32 N>
struct UnkContactBuffer {
    u32 count;
    UnkContactPoint points[N];
};

bool fn_710012c760(const UnkShape8Geometry* a, const void* b, UnkContactBuffer<4>* out, bool flag);


// A type-8 shape against every edge of every polygon; with a sink, every point found is reported
// with its polygon and handle, without one the first stops the query.
UnkContact fn_7100d63e50(UnkPolygonCollection2D* self, const UnkShape8Geometry* shape, bool flag,
                         UnkLocalQuery* query) {
    UnkContact result = NoContact();
    u32 index = 0;
    for (const UnkPolygon2D *p = self->polygons, *end = p + self->count; p != end && !(result.value & 2);
         ++p, ++index) {
        for (u64 i = 0; !(result.value & 2) && i < EdgeCount(*p); ++i) {
            const UnkPolygonVertex* v = &p->vertices[i];
            UnkSegment2D edge(v->pos, (i == p->count - 1 ? p->vertices : v + 1)->pos);
            UnkContactBuffer<2> found;
            found.count = 0;
            if (!fn_710012c760(shape, &edge, reinterpret_cast<UnkContactBuffer<4>*>(&found), flag))
                continue;
            for (const UnkContactPoint* c = found.points; !(result.value & 2) && c != found.points + found.count;
                 ++c) {
                if (query)
                    result.value |= query->vf_0x10(UnkPolygonHit{p, (index << 16) + (u32)i, *c}).value;
                else
                    result.value |= 3;
            }
        }
    }
    return result;
}