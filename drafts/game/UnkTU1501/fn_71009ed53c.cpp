// Unit 1501 (CSamusMovement's neighbours): looks for a wall in a horizontal direction. Clears the hit,
// gives up on a direction with no sideways part, reuses the remembered hit (+0x2390) when it lies ahead,
// else sweeps the shape from the point along the direction and keeps the hit when its surface faces
// the way the direction allows. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "game/UnkAssignedSlot.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU365.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkTree;
struct UnkShape;
struct UnkMtx44;
struct Vec2;
struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

bool fn_7100d5babc(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);

struct UnkWallOwner_l9ed53c {
    u8 pad_0x0[0x20];
    void* kind;
};
struct UnkWallObject_l9ed53c : base::UnkRefCounted {
    UnkWallOwner_l9ed53c* owner;
};

// A reference to a counted object, released when the count drops to zero; assigned out of line.
struct UnkWallRef_l9ed53c {
    UnkWallObject_l9ed53c* ptr;

    UnkWallRef_l9ed53c() : ptr(nullptr) {}
    ~UnkWallRef_l9ed53c() { Release(); }
    void Release() {
        if (UnkWallObject_l9ed53c* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkWallRef_l9ed53c& operator=(const UnkWallRef_l9ed53c& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
};

struct UnkWallVec_l9ed53c {
    f32 x, y, z;

    UnkWallVec_l9ed53c(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkWallVec_l9ed53c& operator=(const UnkWallVec_l9ed53c& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

// A collision hit (0x48 bytes), as UnkTU1501's floor probe has it.
struct UnkWallHit_l9ed53c {
    UnkWallRef_l9ed53c object;
    UnkWallRef_l9ed53c field_0x8;
    s32 id;
    UnkWallVec_l9ed53c point;
    UnkWallVec_l9ed53c field_0x20;
    UnkWallVec_l9ed53c normal;
    f32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkWallHit_l9ed53c()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0.0f),
          flags(0), field_0x40(0) {}
    UnkWallHit_l9ed53c& operator=(const UnkWallHit_l9ed53c& o) {
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
static_assert(sizeof(UnkWallHit_l9ed53c) == 0x48, "UnkWallHit_l9ed53c layout");

// The sweep's callback object: the closest hit so far and the sweep's origin. The vtables (reached
// through the GOT) are the callback's and its base's.
struct UnkWallQuery_l9ed53c {
    void** vtable;
    UnkWallHit_l9ed53c hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkWallQuery_l9ed53c(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin = o;
        field_0x68 = 0;
    }
    ~UnkWallQuery_l9ed53c() { vtable = g_unk_7101a20ed8 + 2; }
};

// Drops what `slot` refers to.
static inline void DropRef(UnkWallObject_l9ed53c** slot) {
    if (UnkWallObject_l9ed53c* p = *slot) {
        if (--p->ref_count == 0)
            delete p;
        *slot = nullptr;
    }
}

struct UnkStaticZero_l9ed53c {
    u32 value;
    UnkStaticZero_l9ed53c() : value(0) {}
};
inline const UnkStaticZero_l9ed53c& StaticZero_l9ed53c() {
    static UnkStaticZero_l9ed53c s_value asm("g_unk_71726b5354");
    return s_value;
}

struct UnkSamusMovement_l9ed53c {
    u8 pad_0x0[0x3cc];
    u32 mask;
    u8 pad_0x3d0[0x2390 - 0x3d0];
    UnkWallHit_l9ed53c wall;
};

static inline f32 Dot(const CVector3D& a, const UnkWallVec_l9ed53c& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline bool Near(f32 a, f32 b, f32 eps) {
    return a - eps <= b && a + eps >= b;
}

bool fn_71009ed53c(UnkSamusMovement_l9ed53c* self, const CVector3D* pos, const CVector3D* dir, UnkShape* shape,
                   UnkWallHit_l9ed53c* out, f32 distance) {
    DropRef(&out->object.ptr);
    DropRef(&out->field_0x8.ptr);
    out->flags = StaticZero_l9ed53c().value;
    if (Near(dir->x, 0.0f, 0.1f))
        return false;
    UnkGameManagerSub_0x608* world = g_unk_7101d47310->field_0x608;
    if (!world)
        return false;
    UnkTree* tree = static_cast<UnkTree*>(world->field_0x820);

    UnkWallHit_l9ed53c& wall = self->wall;
    if (wall.object.ptr && wall.object.ptr->owner) {
        CVector3D toWall = {wall.point.x - pos->x, wall.point.y - pos->y, wall.point.z - pos->z};
        if (dir->x * toWall.x + dir->y * toWall.y + dir->z * toWall.z > 0.0f) {
            *out = wall;
            return true;
        }
    }

    UnkWallQuery_l9ed53c query(*pos);
    u32 mask = self->mask;
    UnkMatrix44_7100078c9c xf = fn_7100078cd8(*reinterpret_cast<const UnkVec3f*>(pos));
    CVector2D delta;
    f32 x = dir->x;
    f32 y = dir->y;
    CVector3D sweep;
    sweep.y = y * distance;
    sweep.x = x * distance;
    sweep.z = dir->z * distance;
    fn_710007b430(&delta, &sweep);
    if (!fn_7100d5babc(tree, mask, shape, reinterpret_cast<const UnkMtx44*>(&xf), reinterpret_cast<const Vec2*>(&delta),
                       &query, true))
        return false;

    const UnkWallHit_l9ed53c& hit = query.hit;
    if ((hit.flags & 0x200) && hit.field_0x38 != 0.0f) {
        f32 facing = Dot(*dir, hit.normal);
        if (facing >= 0.0f)
            return false;
        if (facing > -0.7f && hit.normal.y >= -0.001f)
            return false;
    } else {
        f32 facing = Dot(*dir, hit.field_0x20);
        if (facing < 0.0f)
            return false;
        if (facing < 0.7f && hit.field_0x20.y < 0.0f)
            return false;
    }
    *out = query.hit;
    return true;
}
