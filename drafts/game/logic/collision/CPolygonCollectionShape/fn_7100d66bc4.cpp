// game::logic::collision::CPolygonCollectionShape's unit: the leaf visitor of the segment query. It
// tests the query segment against the edge of a tree leaf and hands the contact found to the query's
// sink. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU109.h"
#include "types.h"

struct UnkPoint3 {
    f32 x, y, z;
};

struct UnkContact {
    u32 value;

    UnkContact() { value = 0; }
    UnkContact(const UnkContact& other) { value = other.value; }
};

struct UnkContactPoint {
    UnkPoint3 point;
    UnkPoint3 normal_a;
    UnkPoint3 normal_b;
    u32 field_0x24;
    u32 flags;
};
static_assert(sizeof(UnkContactPoint) == 0x2c, "UnkContactPoint layout");
struct UnkContact_710012be70 : UnkContactPoint {};

struct UnkPolygon2D;
struct UnkPolygonCollection2D;

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
    UnkPoint3 offset;
    u16 field_0x14;
    const void* field_0x18;
    void* field_0x20;
};

struct UnkTreeNode {
    f32 min_x, min_y;
    f32 max_x, max_y;
    s32 next;
};

struct UnkLeafVisitor {
    const UnkSegment2D* shape;
    bool extra;
    UnkLocalQuery* query;
    UnkContact result;
};

template <u32 N>
struct UnkContactBuffer {
    u32 count;
    UnkContactPoint points[N];
};

bool fn_710012be70(const UnkSegment2D* a, const UnkSegment2D* b, UnkContact_710012be70* out, bool collinear,
                   f32 tolerance);
UnkSegment2D fn_710012436c(const UnkPolygonCollection2D* polygons, u32 handle);

static inline u32 LeafHandle(const UnkTreeNode* node) {
    return node->next < 0 ? (u32)node->next & 0x7ffffff : 0xfffffff;
}

// The query segment against a leaf's edge. False when the walk is to stop.
bool fn_7100d66bc4(UnkLeafVisitor* self, const UnkTreeNode* node) {
    UnkContactBuffer<1> found;
    found.count = 0;
    const auto* polygons = static_cast<const UnkPolygonCollection2D*>(self->query->field_0x18);
    UnkSegment2D edge = fn_710012436c(polygons, LeafHandle(node));
    bool hit;
    {
        UnkContact_710012be70 point = {};
        hit = fn_710012be70(&edge, self->shape, &point, false, 0.0f);
        if (hit) {
            __builtin_memcpy(&found.points[0], &point, sizeof(point));
            found.count = 1;
        }
    }
    if (hit && !(self->result.value & 2)) {
        for (const UnkContactPoint* p = found.points; p != found.points + found.count; ++p) {
            UnkLocalQuery* query = self->query;
            if (!query->field_0x20) {
                self->result.value |= 3;
            } else {
                auto* pool = reinterpret_cast<UnkPool_0x48*>(const_cast<void*>(query->field_0x18));
                const auto* polygon = reinterpret_cast<const UnkPolygon2D*>(fn_710012431c(pool, LeafHandle(node)));
                self->result.value |= query->vf_0x10(UnkPolygonHit{polygon, LeafHandle(node), *p}).value;
            }
            if (self->result.value & 2)
                break;
        }
    }
    return !(self->result.value & 2);
}
