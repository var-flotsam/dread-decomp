// CDredhedDiveAttack's unit, fn_71004e59e0: whether the target (fn_7100d7c990) is in a 300 by 50 box set
// 150 ahead of the owner's model point 0x5593c6788b4efe5b (fn_71011735e8), and, unless the attack's flag
// at +0x34 is set, not in the floor-slide character class. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkAssignedSlot.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CAABoxShape2D.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
// A reference to a counted object (base::UnkRefCounted), released when the count drops to zero.
struct UnkHitRefA_ovb0198u {
    base::UnkRefCounted* ptr;

    UnkHitRefA_ovb0198u() : ptr(nullptr) {}
    UnkHitRefA_ovb0198u(const UnkHitRefA_ovb0198u& other) : ptr(nullptr) { Set(other.ptr); }
    ~UnkHitRefA_ovb0198u() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkHitRefA_ovb0198u& operator=(const UnkHitRefA_ovb0198u& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
    void Set(base::UnkRefCounted* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (base::UnkRefCounted* old = ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            ptr = p;
        }
    }
};
struct UnkHitRefB_ovb0198u {
    base::UnkRefCounted* ptr;

    UnkHitRefB_ovb0198u() : ptr(nullptr) {}
    UnkHitRefB_ovb0198u(const UnkHitRefB_ovb0198u& other) : ptr(nullptr) { Set(other.ptr); }
    ~UnkHitRefB_ovb0198u() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkHitRefB_ovb0198u& operator=(const UnkHitRefB_ovb0198u& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
    void Set(base::UnkRefCounted* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (base::UnkRefCounted* old = ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            ptr = p;
        }
    }
};

// A vector copied component by component.
struct UnkHitVec_ovb0198u {
    f32 x, y, z;

    UnkHitVec_ovb0198u() {}
    UnkHitVec_ovb0198u(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkHitVec_ovb0198u(const UnkHitVec_ovb0198u& o) : x(o.x), y(o.y), z(o.z) {}
    UnkHitVec_ovb0198u& operator=(const UnkHitVec_ovb0198u& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the point, the
// reversed normal and the normal, and flags.
struct UnkCollisionHit_ovb0198u {
    UnkHitRefA_ovb0198u object;
    UnkHitRefB_ovb0198u field_0x8;
    s32 id;
    UnkHitVec_ovb0198u point;
    UnkHitVec_ovb0198u field_0x20;
    UnkHitVec_ovb0198u normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_ovb0198u()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
    // The hit as seen from the other side: the id kept only when flag 0 says it is set, the
    // second vector the reversed normal, flags 0x21 added.
    struct Reversed {};
    UnkCollisionHit_ovb0198u(const UnkCollisionHit_ovb0198u& o, Reversed)
        : object(o.object), field_0x8(o.field_0x8), id(o.flags & 1 ? o.id : -1), point(o.point), normal(o.normal),
          field_0x38(o.field_0x38), flags(o.flags | 0x21), field_0x40(o.field_0x40) {
        field_0x20 = UnkHitVec_ovb0198u(-normal.x, -normal.y, -normal.z);
    }
    UnkCollisionHit_ovb0198u& operator=(const UnkCollisionHit_ovb0198u& o) {
        if (this == &o)
            return *this;
        object = o.object;
        field_0x8 = o.field_0x8;
        id = o.id;
        point = o.point;
        field_0x20 = o.field_0x20;
        normal = o.normal;
        field_0x38 = o.field_0x38;
        flags = o.flags;
        field_0x40 = o.field_0x40;
        return *this;
    }
};
static_assert(sizeof(UnkCollisionHit_ovb0198u) == 0x48, "UnkCollisionHit_ovb0198u layout");

// The hit a collision test reports, under the base query's vtable.
struct UnkDredhedHitQuery_l4e59e0 {
    void** vtable;
    UnkCollisionHit_ovb0198u hit;

    UnkDredhedHitQuery_l4e59e0() { vtable = g_unk_7101a20ed8 + 2; }
    ~UnkDredhedHitQuery_l4e59e0() { vtable = g_unk_7101a20ed8 + 2; }
};

// A collision shape (0xc0 bytes) built from a 2D box (fn_7100d5e1f8) and destroyed by
// fn_7100d5e2c8.
struct UnkBox2D {
    f32 min_x, min_y, max_x, max_y;

    UnkBox2D(f32 min_x_, f32 min_y_, f32 max_x_, f32 max_y_)
        : min_x(min_x_), min_y(min_y_), max_x(max_x_), max_y(max_y_) {}
};
struct UnkCollisionShape {
    u8 data[0xc0];
};

// Builds the shape for a box under a transform.
void fn_7100d5e260(UnkCollisionShape* self, const UnkMatrix44* transform, const UnkBox2D* box);

// The shape built for a box under a transform, destroyed with it.
struct UnkDredhedBoxShape_l4e59e0 {
    UnkCollisionShape shape;

    UnkDredhedBoxShape_l4e59e0(const UnkMatrix44* transform, const UnkBox2D* box) {
        fn_7100d5e260(&shape, transform, box);
    }
    ~UnkDredhedBoxShape_l4e59e0() { fn_7100d5e2c8(&shape); }
};

struct UnkPlayerMovementCache;
void* fn_71010d7a6c(UnkPlayerMovementCache* self);

// The target as this test reads it.
struct UnkDredhedTarget_l4e59e0 {
    u8 pad_0x0[0x200];
    UnkPlayerMovementCache* movement;  // 0x200
    u8 pad_0x208[0x18];
    UnkAnimationComponent* char_class;  // 0x220
};

struct UnkDredhedDiveAttack_l4e59e0 {
    void* vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x34 - 0x10];
    bool field_0x34;
};

static inline f32 Sign(f32 v) {
    return (f32)((v > 0.0f) - (v < 0.0f));
}

static inline void SetIdentity(UnkMatrix44* m) {
    for (s32 i = 0; i < 16; i++)
        m->m[i] = 0.0f;
    m->m[15] = 1.0f;
    m->m[10] = 1.0f;
    m->m[5] = 1.0f;
    m->m[0] = 1.0f;
}

bool fn_71004e59e0(UnkDredhedDiveAttack_l4e59e0* self) {
    UnkMatrix44 transform;
    SetIdentity(&transform);
    u64 point = 0x5593c6788b4efe5bull;
    fn_71011735e8(static_cast<UnkEntity*>(self->owner->field_0x228), &point, &transform, 0);
    UnkDredhedTarget_l4e59e0* target =
        reinterpret_cast<UnkDredhedTarget_l4e59e0*>(fn_7100d7c990(self->owner));
    CVector2D center;
    {
        CVector3D position = {transform.m[3], transform.m[7], transform.m[11]};
        fn_710007b430(&center, &position);
    }
    UnkCopyMatrix(&transform, g_unk_7101a1a680);

    const UnkMatrix44& facing = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    center.x += Sign(facing.m[2]) * 150.0f;
    UnkBox2D box(center.x - 150.0f, center.y - 25.0f, center.x + 150.0f, center.y + 25.0f);
    UnkDredhedBoxShape_l4e59e0 shape(&transform, &box);
    void* body = fn_71010d7a6c(target->movement);
    if (!body)
        return false;
    UnkDredhedHitQuery_l4e59e0 query;
    return fn_7100d57ca0(reinterpret_cast<UnkLavaDropsShape_A2_b71*>(&shape), &transform, body, &query, 0) &&
           (self->field_0x34 || (target->char_class && !fn_710117b348(target->char_class, fn_7100cf919c())));
}