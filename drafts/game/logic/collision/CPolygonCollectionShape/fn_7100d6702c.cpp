// game::logic::collision::CPolygonCollectionShape's unit: the leaf visitor of a type-8 shape moving
// along a direction, tested against a tree leaf's edge; a contact that passes the edge's own test
// goes to the query's sink. Names and layouts are placeholders.
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

struct UnkTreeNode {
    f32 min_x, min_y;
    f32 max_x, max_y;
    s32 next;
};
static_assert(sizeof(UnkTreeNode) == 0x14, "UnkTreeNode layout");

struct UnkShape8Geometry {
    f32 x, y;
    f32 field_0x8;
    f32 field_0xc;
};

static inline bool CopyOf(bool b) {
    return b;
}
static inline CVector2D CopyOf(const CVector2D& v) {
    CVector2D r;
    r.x = v.x;
    r.y = v.y;
    return r;
}

template <class G, class E>
struct UnkLeafVisitor {
    const G* shape;
    E extra;
    UnkLocalQuery* query;
    UnkContact result;

    UnkLeafVisitor(const G* s, const E& e, UnkLocalQuery* q)
        : shape(s), extra(CopyOf(e)), query(q), result(NoContact()) {}
};

static inline u32 LeafHandle(const UnkTreeNode* node) {
    return node->next < 0 ? (u32)node->next & 0x7ffffff : 0xfffffff;
}

UnkSegment2D fn_710012436c(const UnkPolygonCollection2D* polygons, u32 handle);
bool fn_710012f750(const UnkShape8Geometry* shape, const UnkVec2Result& step, const UnkSegment2D* seg,
                   UnkContactPoint* out);
struct UnkPolygonProbe;
bool fn_71001244ac(UnkPolygonCollection2D* self, u32 handle, const UnkPolygonProbe* probe);


// The type-8 shape moved by `extra` against a leaf's edge. False when the walk is to stop.
bool fn_7100d6702c(UnkLeafVisitor<UnkShape8Geometry, CVector2D>* self, const UnkTreeNode* node) {
    UnkContactPoint point = {};
    bool hit;
    {
        const UnkShape8Geometry* shape = self->shape;
        const UnkVec2Result& step = reinterpret_cast<const UnkVec2Result&>(self->extra);
        const auto* edges = static_cast<const UnkPolygonCollection2D*>(self->query->field_0x18);
        UnkSegment2D edge = fn_710012436c(edges, LeafHandle(node));
        hit = fn_710012f750(shape, step, &edge, &point);
    }
    if (hit) {
        auto* polygons = static_cast<UnkPolygonCollection2D*>(const_cast<void*>(self->query->field_0x18));
        if (fn_71001244ac(polygons, LeafHandle(node), reinterpret_cast<const UnkPolygonProbe*>(&point))) {
            UnkLocalQuery* query = self->query;
            u32 value;
            if (!query->field_0x20) {
                value = 3;
            } else {
                auto* pool = reinterpret_cast<UnkPool_0x48*>(const_cast<void*>(query->field_0x18));
                const auto* polygon = reinterpret_cast<const UnkPolygon2D*>(ResolveHandleToPoolSlot(pool, LeafHandle(node)));
                value = query->vf_0x10(UnkPolygonHit{polygon, LeafHandle(node), point}).value;
            }
            self->result.value |= value;
        }
    }
    return !(self->result.value & 2);
}
