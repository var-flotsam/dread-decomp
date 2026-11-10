// Unit 442: a polyline path (points of 12 bytes: position and a kind word; a closed flag at +0x2c) that
// also keeps the length of each leg (+0x48) and one word per leg (+0x70). Names are placeholders.
#include "base/UnkEmptyDtorElem.h"
#include "base/UnkVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;

struct UnkElem {
    u32 field_0x0;

    // Copied bytewise: the original reloads the array pointer after the store.
    UnkElem(UnkElem&& other) { __builtin_memcpy(this, &other, sizeof(*this)); }
    ~UnkElem() { fn_71002874dc(this); }
};

struct UnkPathPoint_b0151 {
    CVector2D pos;
    u32 kind;
};

struct UnkPathVisitor_b0151 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30(const UnkSegment2D* seg);
};

struct UnkPathCtx_b0151 {
    u8 pad_0x0[0x20];
    UnkPathVisitor_b0151* visitor;  // 0x20
};

struct UnkPath_b0151 {
    base::UnkVector<UnkPathPoint_b0151> points;  // 0x0
    f32 field_0x28;
    u8 closed;  // 0x2c
    u8 pad_0x2d[0x1b];
    base::UnkVector<f32> lengths;  // 0x48
    base::UnkVector<UnkElem> elems;  // 0x70
    u8 pad_0x98[0x1008];
    void* field_0x10a0;
};
static_assert(__builtin_offsetof(UnkPath_b0151, lengths) == 0x48, "UnkPath_b0151 layout");
static_assert(__builtin_offsetof(UnkPath_b0151, elems) == 0x70, "UnkPath_b0151 layout");
static_assert(__builtin_offsetof(UnkPath_b0151, field_0x10a0) == 0x10a0, "UnkPath_b0151 layout");

void fn_710035f70c(base::UnkVector<UnkElem>* self, UnkElem* value);
void fn_710035f810(base::UnkVector<UnkElem>* self, UnkElem* value, u32 index);
void fn_710035ded8(UnkPath_b0151* self, UnkSegment2D* seg, UnkElem* elem);
void fn_710035e124(UnkPath_b0151* self, UnkSegment2D* seg, UnkElem* elem);
bool fn_710027df74(void* obj, UnkElem* elem);

void fn_710035debc(UnkPath_b0151* self, bool append, UnkSegment2D* seg, UnkElem* elem) {
    if (append)
        fn_710035ded8(self, seg, elem);
    else
        fn_710035e124(self, seg, elem);
}

static inline u32 Legs(const UnkPath_b0151* p) {
    u32 not_closed = p->closed == 0;
    u32 n = p->points.count;
    u32 open = (u32)(n != 0) & not_closed;
    return n - open;
}

__attribute__((flatten)) void fn_710035f810(base::UnkVector<UnkElem>* self, UnkElem* value, u32 index) {
    if (self->count == index) {
        fn_710035f70c(self, value);
        return;
    }
    self->Insert(static_cast<UnkElem&&>(*value), index);
}


bool fn_710012c1b0(const UnkSegment2D* a, const UnkSegment2D* b, UnkSegment2D* out);
UnkVec2Result fn_710012c6e0(const CVector2D* point, const UnkSegment2D* seg);

// The closest point to `point` on the legs of `other` whose points are of kind 0, as met by each leg
// of `path` (the visitor of `ctx` sees each leg first); `point` itself when none is nearer.
UnkVec2Result fn_710035f500(UnkPath_b0151* path, UnkPath_b0151* other, CVector2D* point, UnkPathCtx_b0151* ctx) {
    UnkVec2Result best(reinterpret_cast<const UnkVec2Result&>(*point));
    if (!other)
        return best;
    if (Legs(other) == 0)
        return best;
    u32 path_legs = Legs(path);
    if (path_legs == 0)
        return best;
    f32 best_dist = 3.4028235e38f;
    u32 i = 0;
    do {
        UnkPathPoint_b0151* points = path->points.data;
        UnkSegment2D seg(points[i].pos, i == path->points.count - 1 ? points[0].pos : points[i + 1].pos);
        if (ctx->visitor)
            ctx->visitor->vf_0x30(&seg);
        u32 other_legs = Legs(other);
        for (u32 j = 0; j != other_legs; ++j) {
            UnkPathPoint_b0151* q = other->points.data;
            if (q[j].kind != 0)
                continue;
            UnkSegment2D leg(q[j].pos, j == other->points.count - 1 ? q[0].pos : q[j + 1].pos);
            UnkSegment2D hit;
            __builtin_memset(&hit, 0, 16);
            hit.field_0x10 = -1.0f;
            if (!fn_710012c1b0(&seg, &leg, &hit))
                continue;
            UnkVec2Result r = fn_710012c6e0(point, &hit);
            CVector2D p = *point;
            f32 dx = p.x - r.x;
            f32 dy = p.y - r.y;
            f32 dist = dx * dx + dy * dy;
            if (dist < best_dist) {
                best_dist = dist;
                best.x = r.x;
                best.y = r.y;
            }
        }
    } while (++i != path_legs);
    return best;
}
