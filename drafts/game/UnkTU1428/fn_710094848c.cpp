// Unit 1428 (CMorphBallMovement's neighbourhood): queries the collision tracker with the owner's shape,
// shrunk to 97% for the query, at the owner's transform, and returns the closest hit.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::UnkRefCounted;

// A reference to a counted object, released when the count drops to zero.
struct UnkProbeRef_l94848c {
    UnkRefCounted* ptr;

    UnkProbeRef_l94848c() : ptr(nullptr) {}
    UnkProbeRef_l94848c(const UnkProbeRef_l94848c& other) : ptr(nullptr) { Set(other.ptr); }
    ~UnkProbeRef_l94848c() {
        if (UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    void Set(UnkRefCounted* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted* old = ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            ptr = p;
        }
    }
};

// A vector copied component by component.
struct UnkProbeVec_l94848c {
    f32 x, y, z;

    UnkProbeVec_l94848c(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkProbeVec_l94848c(const UnkProbeVec_l94848c& o) : x(o.x), y(o.y), z(o.z) {}
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the point, two vectors and
// flags.
struct UnkProbeHit_l94848c {
    UnkProbeRef_l94848c object;
    UnkProbeRef_l94848c field_0x8;
    s32 id;
    UnkProbeVec_l94848c point;
    UnkProbeVec_l94848c field_0x20;
    UnkProbeVec_l94848c normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkProbeHit_l94848c()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
    UnkProbeHit_l94848c(const UnkProbeHit_l94848c& o)
        : object(o.object), field_0x8(o.field_0x8), id(o.id), point(o.point), field_0x20(o.field_0x20),
          normal(o.normal), field_0x38(o.field_0x38), flags(o.flags), field_0x40(o.field_0x40) {}
};
static_assert(sizeof(UnkProbeHit_l94848c) == 0x48, "layout");

struct UnkProbePos_l94848c {
    f32 v[3];
};

// The query's callback object: the closest hit so far and the query's origin. The vtables (reached through
// the GOT) are the callback's and its base's.
struct UnkProbeQuery_l94848c {
    void** vtable;
    UnkProbeHit_l94848c hit;
    bool field_0x50;
    u32 field_0x54;
    UnkProbePos_l94848c origin;
    u64 field_0x68;

    explicit UnkProbeQuery_l94848c(UnkProbePos_l94848c o) : field_0x50(false), origin(o), field_0x68(0) {
        vtable = g_unk_7101a20fa8 + 2;
    }
    ~UnkProbeQuery_l94848c() { vtable = g_unk_7101a20ed8 + 2; }
};

// The shape queried: slot 0x10 restores it, 0x18 gives its kind, 0x30 scales it and 0x40 gives the shape
// a kind-2 one stands for.
struct UnkProbeShape_l94848c {
    void** vtable;

    void Restore() { reinterpret_cast<void (*)(UnkProbeShape_l94848c*)>(vtable[0x10 / 8])(this); }
    s32 Kind() { return reinterpret_cast<s32 (*)(UnkProbeShape_l94848c*)>(vtable[0x18 / 8])(this); }
    void Scale(f32 factor) {
        reinterpret_cast<void (*)(UnkProbeShape_l94848c*, f32)>(vtable[0x30 / 8])(this, factor);
    }
    UnkProbeShape_l94848c* Inner() {
        return reinterpret_cast<UnkProbeShape_l94848c* (*)(UnkProbeShape_l94848c*)>(vtable[0x40 / 8])(this);
    }
};

struct UnkProbeBody_l94848c {
    u8 pad_0x0[0x158];
    UnkProbeShape_l94848c* shape;  // 0x158
};

struct UnkProbeActor_l94848c {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

struct UnkProbeSelf_l94848c {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkProbeActor_l94848c* actor;  // 0x30
    u8 pad_0x38[0x18c - 0x38];
    UnkProbePos_l94848c pos;  // 0x18c
    u8 pad_0x198[0x3c8 - 0x198];
    u32 collision_mask;  // 0x3c8

    UnkProbeBody_l94848c* Body() {
        return reinterpret_cast<UnkProbeBody_l94848c* (*)(UnkProbeSelf_l94848c*)>(vtable[0x2a8 / 8])(this);
    }
};
static_assert(__builtin_offsetof(UnkProbeSelf_l94848c, collision_mask) == 0x3c8, "layout");

UnkProbeHit_l94848c fn_710094848c(UnkProbeSelf_l94848c* self) {
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    UnkTree* tree = sub ? static_cast<UnkTree*>(sub->field_0x820) : nullptr;
    UnkProbeShape_l94848c* body = self->Body()->shape;
    UnkProbeShape_l94848c* shape = (body->Kind() == 2 ? body : nullptr)->Inner();
    shape->Scale(0.97f);
    UnkMatrix44 xf;
    UnkCopyMatrix(&xf, fn_710002614c(&self->actor->transform));
    UnkProbeQuery_l94848c query(self->pos);
    QueryCollisionTreeForTransformedShapeOverlaps(tree, self->collision_mask, reinterpret_cast<UnkShape*>(shape),
                  reinterpret_cast<const UnkMtx44*>(&xf), &query, 0);
    shape->Restore();
    return query.hit;
}
