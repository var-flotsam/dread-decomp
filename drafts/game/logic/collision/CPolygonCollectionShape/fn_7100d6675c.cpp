// game::logic::collision::CPolygonCollectionShape's unit: the leaf visitor of a box, tested against a
// tree leaf's edge; every contact found goes to the query's sink. Names and layouts are placeholders.
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

struct UnkBox2D {
    f32 min_x, min_y;
    f32 max_x, max_y;
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

template <u32 N>
struct UnkContactBuffer {
    u32 count;
    UnkContactPoint points[N];
};

UnkSegment2D fn_710012436c(const UnkPolygonCollection2D* polygons, u32 handle);
bool fn_710012b2c0(const void* b, const UnkBox2D* a, UnkContactBuffer<2>* out, bool flag);


// The box against a leaf's edge: every point found is reported to the query's sink (with no sink,
// the first stops the walk). False when the walk is to stop.
bool fn_7100d6675c(UnkLeafVisitor<UnkBox2D, bool>* self, const UnkTreeNode* node) {
    UnkContactBuffer<2> found;
    found.count = 0;
    bool hit;
    {
        const auto* polygons = static_cast<const UnkPolygonCollection2D*>(self->query->field_0x18);
        UnkSegment2D edge = fn_710012436c(polygons, LeafHandle(node));
        hit = fn_710012b2c0(&edge, self->shape, &found, self->extra);
    }
    if (hit) {
        for (const UnkContactPoint* p = found.points; !(self->result.value & 2) && p != found.points + found.count;
             ++p) {
            UnkLocalQuery* query = self->query;
            u32 value;
            if (!query->field_0x20) {
                value = 3;
            } else {
                auto* pool = reinterpret_cast<UnkPool_0x48*>(const_cast<void*>(query->field_0x18));
                const auto* polygon = reinterpret_cast<const UnkPolygon2D*>(fn_710012431c(pool, LeafHandle(node)));
                value = query->vf_0x10(UnkPolygonHit{polygon, LeafHandle(node), *p}).value;
            }
            self->result.value |= value;
        }
    }
    return !(self->result.value & 2);
}