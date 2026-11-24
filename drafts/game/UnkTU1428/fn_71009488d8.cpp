// Unit 1428 (CMorphBallMovement's neighbourhood): queries the collision tracker with the owner's shape,
// shrunk to 97% for the query, at the owner's transform moved to the given point, and returns the closest hit.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::UnkRefCounted;

// The vtable of this query's callback (reached through the GOT).
extern void* g_unk_7101a84618[];

// A reference to a counted object, released when the count drops to zero.
struct UnkProbeRef_l9488d8 {
    UnkRefCounted* ptr;

    UnkProbeRef_l9488d8() : ptr(nullptr) {}
    UnkProbeRef_l9488d8(const UnkProbeRef_l9488d8& other) : ptr(nullptr) { Set(other.ptr); }
    ~UnkProbeRef_l9488d8() {
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
struct UnkProbeVec_l9488d8 {
    f32 x, y, z;

    UnkProbeVec_l9488d8(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkProbeVec_l9488d8(const UnkProbeVec_l9488d8& o) : x(o.x), y(o.y), z(o.z) {}
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the point, two vectors and
// flags.
struct UnkProbeHit_l9488d8 {
    UnkProbeRef_l9488d8 object;
    UnkProbeRef_l9488d8 field_0x8;
    s32 id;
    UnkProbeVec_l9488d8 point;
    UnkProbeVec_l9488d8 field_0x20;
    UnkProbeVec_l9488d8 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkProbeHit_l9488d8()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
    UnkProbeHit_l9488d8(const UnkProbeHit_l9488d8& o)
        : object(o.object), field_0x8(o.field_0x8), id(o.id), point(o.point), field_0x20(o.field_0x20),
          normal(o.normal), field_0x38(o.field_0x38), flags(o.flags), field_0x40(o.field_0x40) {}
};
static_assert(sizeof(UnkProbeHit_l9488d8) == 0x48, "layout");

struct UnkProbePos_l9488d8 {
    f32 x, y, z;
};

// The query's callback object: the closest hit so far and the query's origin. The vtables (reached through
// the GOT) are the callback's and its base's.
struct UnkProbeQuery_l9488d8 {
    void** vtable;
    UnkProbeHit_l9488d8 hit;
    bool field_0x50;
    UnkProbePos_l9488d8 origin;

    explicit UnkProbeQuery_l9488d8(UnkProbePos_l9488d8 o) : field_0x50(false), origin(o) {
        vtable = g_unk_7101a84618 + 2;
    }
    ~UnkProbeQuery_l9488d8() { vtable = g_unk_7101a20ed8 + 2; }
};

// The shape queried: slot 0x10 restores it, 0x18 gives its kind, 0x30 scales it and 0x40 gives the shape
// a kind-2 one stands for.
struct UnkProbeShape_l9488d8 {
    void** vtable;

    void Restore() { reinterpret_cast<void (*)(UnkProbeShape_l9488d8*)>(vtable[0x10 / 8])(this); }
    s32 Kind() { return reinterpret_cast<s32 (*)(UnkProbeShape_l9488d8*)>(vtable[0x18 / 8])(this); }
    void Scale(f32 factor) {
        reinterpret_cast<void (*)(UnkProbeShape_l9488d8*, f32)>(vtable[0x30 / 8])(this, factor);
    }
    UnkProbeShape_l9488d8* Inner() {
        return reinterpret_cast<UnkProbeShape_l9488d8* (*)(UnkProbeShape_l9488d8*)>(vtable[0x40 / 8])(this);
    }
};

struct UnkProbeBody_l9488d8 {
    u8 pad_0x0[0x158];
    UnkProbeShape_l9488d8* shape;  // 0x158
};

struct UnkProbeActor_l9488d8 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

struct UnkProbeSelf_l9488d8 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkProbeActor_l9488d8* actor;  // 0x30
    u8 pad_0x38[0x18c - 0x38];
    UnkProbePos_l9488d8 pos;  // 0x18c
    u8 pad_0x198[0x3c8 - 0x198];
    u32 collision_mask;  // 0x3c8

    UnkProbeBody_l9488d8* Body() {
        return reinterpret_cast<UnkProbeBody_l9488d8* (*)(UnkProbeSelf_l9488d8*)>(vtable[0x2a8 / 8])(this);
    }
};
static_assert(__builtin_offsetof(UnkProbeSelf_l9488d8, collision_mask) == 0x3c8, "layout");

UnkProbeHit_l9488d8 fn_71009488d8(UnkProbeSelf_l9488d8* self, const UnkProbePos_l9488d8& at) {
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    UnkTree* tree = sub ? static_cast<UnkTree*>(sub->field_0x820) : nullptr;
    UnkProbeShape_l9488d8* body = self->Body()->shape;
    UnkProbeShape_l9488d8* shape = (body->Kind() == 2 ? body : nullptr)->Inner();
    shape->Scale(0.97f);
    UnkMatrix44 xf;
    UnkCopyMatrix(&xf, fn_710002614c(&self->actor->transform));
    f32 x = at.x;
    f32 y = at.y;
    f32 z = at.z;
    xf.m[3] = x;
    xf.m[7] = y;
    xf.m[11] = z;
    UnkProbeQuery_l9488d8 query(self->pos);
    QueryCollisionTreeForTransformedShapeOverlaps(tree, self->collision_mask, reinterpret_cast<UnkShape*>(shape),
                  reinterpret_cast<const UnkMtx44*>(&xf), &query, 0);
    shape->Restore();
    return query.hit;
}