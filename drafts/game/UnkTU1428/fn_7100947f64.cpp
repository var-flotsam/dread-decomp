// CMorphBallMovement's unit (hint only): takes a new contact hit. A previous contact accepted by
// fn_71010d9638 is replaced by an empty hit first; then, for a hit on a polygon collection (kind 0x10)
// whose edge index is in range, keeps the edge end nearest the owner. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU109.h"
#include "game/UnkTU2689.h"
#include "types.h"

using base::math::CVector2D;

struct UnkPolygonCollection2D;
UnkSegment2D fn_710012436c(const UnkPolygonCollection2D* self, u32 handle);

// A counted reference released through the object's virtual 0x10.
struct UnkContactRef_l947f64 {
    base::UnkRefCounted* ptr;

    UnkContactRef_l947f64() : ptr(nullptr) {}
    ~UnkContactRef_l947f64() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

// The polygon a handle's bits 16 to 30 name: its vertex count and whether it is closed.
struct UnkContactPolygon_l947f64 {
    void* vertices;
    u32 count;  // 0x8
    u8 pad_0xc[0x20];
    u8 closed;  // 0x2c

    // An open polygon has one edge fewer than vertices.
    u32 EdgeCount() const { return count - ((count != 0) & (closed == 0)); }
};

struct UnkContactShape_l947f64 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
    u8 pad_0x8[0xa0];
    u8 polygons[0x48];  // 0xa8: the polygon collection
};

struct UnkContactActor_l947f64 {
    u8 pad_0x0[0x158];
    UnkContactShape_l947f64* shape;  // 0x158
};

struct UnkContactLink_l947f64 {
    u8 pad_0x0[0x10];
    UnkContactActor_l947f64* target;  // 0x10
};

struct UnkContactHit_l947f64 {
    UnkContactRef_l947f64 link;
    UnkContactRef_l947f64 field_0x8;
    u32 handle;  // 0x10
    f32 values[12];

    UnkContactHit_l947f64() : handle(-1), values() {}

    UnkContactLink_l947f64* Link() const { return reinterpret_cast<UnkContactLink_l947f64*>(link.ptr); }
};
static_assert(sizeof(UnkContactHit_l947f64) == 0x48, "UnkContactHit_l947f64 layout");

struct UnkContactOwner_l947f64 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
};

struct UnkContactVec3_l947f64 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkContactMovement_l947f64 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkContactOwner_l947f64* owner;  // 0x30
    u8 pad_0x38[0x248c - 0x38];
    CVector2D nearest;  // 0x248c
    u8 pad_0x2494[0x24c4 - 0x2494];
    UnkContactVec3_l947f64 field_0x24c4;

    void vf_0x218(UnkContactHit_l947f64* hit) {
        reinterpret_cast<void (*)(UnkContactMovement_l947f64*, UnkContactHit_l947f64*)>(vtable[0x218 / 8])(this, hit);
    }
};
static_assert(__builtin_offsetof(UnkContactMovement_l947f64, field_0x24c4) == 0x24c4, "layout");

static inline f32 SquaredLength(const CVector2D& v) {
    return v.x * v.x + v.y * v.y;
}

// Whether fn_71010d9638 accepts the hit for the movement.
inline bool Accepts(UnkContactMovement_l947f64* self, UnkContactHit_l947f64* hit) {
    u64 unused = 0;
    return fn_71010d9638(reinterpret_cast<UnkMove_b152*>(self), reinterpret_cast<UnkHitInfo_b152*>(hit), &unused);
}

void fn_7100947f64(UnkContactMovement_l947f64* self, UnkContactHit_l947f64* hit) {
    if (Accepts(self, hit)) {
        UnkContactHit_l947f64 empty;
        self->vf_0x218(&empty);
    }

    UnkContactLink_l947f64* link = hit->Link();
    if (!link || !link->target)
        return;

    UnkContactShape_l947f64* shape = link->target->shape;
    s32 kind = shape->vf_0x18();
    self->vf_0x218(hit);
    self->field_0x24c4.x = 0.0f;
    self->field_0x24c4.y = 0.0f;
    self->field_0x24c4.z = 0.0f;

    if (shape && kind == 0x10) {
        UnkPool_0x48* pool = reinterpret_cast<UnkPool_0x48*>(shape->polygons);
        auto* polygon = reinterpret_cast<UnkContactPolygon_l947f64*>(ResolveHandleToPoolSlot(pool, hit->handle));
        if (polygon->EdgeCount() > (hit->handle & 0xffff)) {
            UnkSegment2D edge = fn_710012436c(reinterpret_cast<UnkPolygonCollection2D*>(pool), hit->handle);
            UnkContactOwner_l947f64* owner = self->owner;
            CVector2D toStart = {edge.start.x - owner->x, edge.start.y - owner->y};
            CVector2D toEnd = {edge.end.x - owner->x, edge.end.y - owner->y};
            self->nearest = SquaredLength(toStart) < SquaredLength(toEnd) ? toStart : toEnd;
        }
    }
}
