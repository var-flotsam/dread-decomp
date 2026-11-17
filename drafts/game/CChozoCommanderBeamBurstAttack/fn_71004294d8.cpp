// CChozoCommanderBeamBurstAttack's unit (tools/tu_map.py unit 646): aims the beam effect along the beam
// node's x axis. It casts a segment 5000 units long from the node (its transform from fn_71011735e8 with the
// id table's id at +0xd58), keeps the first hit in `hit` (or clears it on a miss), and places the effect at
// the node, turned towards the hit point or the segment's end. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkAssignedSlot.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU133.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

extern "C" f32 atan2f(f32 y, f32 x);
extern void* g_unk_7101a23e70[];

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

// The query's callback object for the first hit: the hit, the segment's origin (twice) and state words. The
// vtables (reached through the GOT) are the callback's and its base's.
struct UnkFirstHitQuery_l4294d8 {
    void** vtable;
    UnkCollisionHit_ovb0198u hit;
    bool field_0x50;
    u32 field_0x54;
    UnkVec3f origin;
    u64 field_0x68;
    bool field_0x70;
    u32 field_0x74;
    UnkVec3f field_0x78;
    u64 field_0x88;

    explicit UnkFirstHitQuery_l4294d8(const UnkVec3f& o) {
        vtable = g_unk_7101a23e70 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
        field_0x70 = false;
        field_0x78.x = o.x;
        field_0x78.y = o.y;
        field_0x78.z = o.z;
        field_0x88 = 0;
    }
    ~UnkFirstHitQuery_l4294d8() { vtable = g_unk_7101a20ed8 + 2; }
};
static_assert(sizeof(UnkFirstHitQuery_l4294d8) == 0x90, "layout");

// A zero whose constructor is not constexpr, so the static is guarded.
struct UnkStaticZero_l4294d8 {
    u32 value;
    UnkStaticZero_l4294d8() : value(0) {}
};

inline const UnkStaticZero_l4294d8& StaticZero_l4294d8() {
    static UnkStaticZero_l4294d8 s_value asm("g_unk_71726b5354");
    return s_value;
}

// Drops a hit's reference, releasing the object when it was the last.
template <class Ref>
inline void ReleaseRef_l4294d8(Ref& ref) {
    if (base::UnkRefCounted* p = ref.ptr) {
        if (--p->ref_count == 0)
            delete p;
        ref.ptr = nullptr;
    }
}

// The translation of a transform.
inline void GetTranslation_l4294d8(const UnkMatrix44& m, UnkVec3f& out) {
    out = UnkVec3f(m.m[3], m.m[7], m.m[11]);
}

struct UnkBeamFx_l4294d8 {
    u8 pad_0x0[0x121];
    bool active;  // 0x121
};

struct UnkBeamOwner_l4294d8 {
    u8 pad_0x0[0x228];
    UnkEntity* entity;  // 0x228
};

struct UnkBeamAim_l4294d8 {
    u8 pad_0x0[0x8];
    UnkBeamOwner_l4294d8* owner;  // 0x8
    u8 pad_0x10[0x50];
    UnkBeamFx_l4294d8* fx;  // 0x60
};

void fn_71004294d8(UnkBeamAim_l4294d8* self, UnkCollisionHit_ovb0198u* hit) {
    if (!self->fx || !self->fx->active)
        return;
    UnkEntity* entity = self->owner->entity;
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);

    UnkMatrix44 m = {};
    m.m[15] = 1.0f;
    m.m[10] = 1.0f;
    m.m[5] = 1.0f;
    m.m[0] = 1.0f;
    const CStrId& id =
        *reinterpret_cast<const CStrId*>(reinterpret_cast<u8*>(&GameManagerIdTable()) + 0xd58);
    {
        u64 key = *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(id.data) + 0x30);
        fn_71011735e8(entity, &key, &m, 0);
    }

    // The node's position, and its x axis on the plane.
    UnkVec3f origin;
    GetTranslation_l4294d8(m, origin);
    f32 axisX = m.m[0];
    f32 axisY = m.m[4];
    f32 axisZ = 0.0f;
    f32 inv = 1.0f / (__builtin_sqrtf(axisX * axisX + axisY * axisY + axisZ * axisZ) + 0.0001f);

    f32 endX = axisX * inv * 5000.0f + origin.x;
    f32 endY = axisY * inv * 5000.0f + origin.y;
    UnkHitSegment_ovb0198u segment(CVector2D{origin.x, origin.y}, CVector2D{endX, endY});
    UnkFirstHitQuery_l4294d8 query(origin);
    f32 x, y, z;
    if (fn_7100d5c518(tree, 0x80429, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
        *hit = query.hit;
        x = hit->point.x;
        y = hit->point.y;
        z = hit->point.z;
    } else {
        ReleaseRef_l4294d8(hit->object);
        ReleaseRef_l4294d8(hit->field_0x8);
        hit->flags = StaticZero_l4294d8().value;
        x = endX;
        y = endY;
        z = axisZ * inv * 5000.0f + origin.z;
    }

    f32 dx = x - origin.x;
    f32 dy = y - origin.y;
    f32 dz = z - origin.z;
    f32 scale = 1.0f / (__builtin_sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
    f32 nx = dx * scale;
    f32 ny = dy * scale;
    fn_7100145f28(reinterpret_cast<UnkSwitchObject*>(self->fx), &origin);
    UnkSwitchObject* fx = reinterpret_cast<UnkSwitchObject*>(self->fx);
    UnkVec3f angles(0.0f, 0.0f, atan2f(ny, nx) + 3.1415927f);
    fn_7100145f3c(fx, &angles);
}
