// CDredhedDiveAttack's unit, fn_71004e468c: whether the target (fn_7100d7c990) has a clear line both up
// and down: segments from its position, its radius (+0x37c) plus 30 long, tested against the game
// manager's collision tree (fn_7100d5c518). Built as src/game/UnkTU437/UnkTU437_SegmentHit_l3523bc.cpp
// builds its segment query. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkAssignedSlot.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
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

// The segment query's callback object: the closest hit so far and the segment's origin. The
// vtables (reached through the GOT) are the callback's and its base's.
struct UnkSegmentHitQuery_ovb0198u {
    void** vtable;
    UnkCollisionHit_ovb0198u hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentHitQuery_ovb0198u(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentHitQuery_ovb0198u() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkHitSegment_ovb0198u {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkHitSegment_ovb0198u(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

// The target as this test reads it.
struct UnkDredhedTarget_l4e468c {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x210 - 0x68];
    void* field_0x210;
    u8 pad_0x218[0x37c - 0x218];
    f32 radius;  // 0x37c
};

struct UnkDredhedDiveAttack_l4e468c {
    void* vtable;
    UnkActor* owner;  // 0x8
};

// The segment from the target's position, its radius plus 30 along `dir`.
static inline UnkHitSegment_ovb0198u Reach(const UnkDredhedTarget_l4e468c* target, const CVector2D& dir) {
    f32 length = target->radius + 30.0f;
    f32 x = target->x;
    f32 y = target->y;
    return UnkHitSegment_ovb0198u(CVector2D{x, y}, CVector2D{x + length * dir.x, y + length * dir.y});
}
static const CVector2D kUp = {0.0f, 1.0f};
static const CVector2D kDown = {0.0f, -1.0f};

bool fn_71004e468c(UnkDredhedDiveAttack_l4e468c* self) {
    UnkDredhedTarget_l4e468c* target =
        reinterpret_cast<UnkDredhedTarget_l4e468c*>(fn_7100d7c990(self->owner));
    if (!target || !target->field_0x210)
        return false;
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);

    UnkHitSegment_ovb0198u up = Reach(target, kUp);
    UnkSegmentHitQuery_ovb0198u upQuery(CVector3D{target->x, target->y, target->z});
    if (!fn_7100d5c518(tree, 0x80409, reinterpret_cast<const UnkBox*>(&up), &upQuery, 2))
        return false;

    UnkHitSegment_ovb0198u down = Reach(target, kDown);
    UnkSegmentHitQuery_ovb0198u downQuery(CVector3D{target->x, target->y, target->z});
    return fn_7100d5c518(tree, 0x80409, reinterpret_cast<const UnkBox*>(&down), &downQuery, 2);
}