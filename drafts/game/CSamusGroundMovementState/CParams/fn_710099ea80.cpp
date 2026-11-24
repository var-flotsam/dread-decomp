// CSamusGroundMovementState::CParams' unit (tools/tu_map.py unit 1462): whether the ground state may step down
// a ledge ahead: a capsule like the movement's own, swept along its direction, must be clear ahead and above
// the drop and find ground below it. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CCapsuleShape2D.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

namespace base { namespace reflection { struct UnkTunable; } }
extern base::reflection::UnkTunable g_unk_7172a03d80;

bool fn_71009e1310(void* self);
bool SweepShapeAgainstCollisionTree(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);
f32 fn_7100d60238(void* shape);
f32 fn_7100d60230(void* shape);
void fn_7100d60184(void* shape, f32 scale);

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

// The tunable's members this reads.
struct UnkGroundTunable_l99ea80 {
    u8 pad_0x0[0x3f];
    bool field_0x3f;  // the check is on
    u8 pad_0x40[0x8];
    f32 field_0x48;   // how far ahead the first sweep goes
};

// The game manager's flag that fails the check (+0x13c).
struct UnkGameManagerFlags_l99ea80 {
    u8 pad_0x0[0x13c];
    bool field_0x13c;
};

struct UnkShape2D_l99ea80;
struct UnkShape2DVtbl_l99ea80 {
    void* pad_0x0[3];
    u32 (*GetType)(UnkShape2D_l99ea80* self);  // vf_0x18
    void* pad_0x20[9];
    UnkVec3f (*vf_0x68)(UnkShape2D_l99ea80* self);  // the shape's centre offset
    void* pad_0x70[2];
    void (*vf_0x80)(UnkShape2D_l99ea80* self);  // refreshes what the shape derives from its size
};

struct UnkShape2D_l99ea80 {
    const UnkShape2DVtbl_l99ea80* vtbl;
};

struct UnkCapsule_l99ea80 : UnkShape2D_l99ea80 {
    u8 pad_0x8[0xa0];
    f32 center_x;     // 0xa8
    f32 center_y;     // 0xac
    f32 half_height;  // 0xb0
    f32 radius;       // 0xb4
    u8 pad_0xb8[0x8];

    UnkCapsule_l99ea80() { fn_7100d5ff7c(this); }
    ~UnkCapsule_l99ea80() { fn_7100d6006c(this); }
};
static_assert(sizeof(UnkCapsule_l99ea80) == 0xc0, "layout");

struct UnkGroundCollider_l99ea80 {
    u8 pad_0x0[0x158];
    UnkShape2D_l99ea80* shape;
};

struct UnkGroundMovement_l99ea80;
struct UnkGroundMovementVtbl_l99ea80 {
    void* pad_0x0[0x55];
    UnkGroundCollider_l99ea80* (*vf_0x2a8)(UnkGroundMovement_l99ea80* self);
};

struct UnkGroundMovement_l99ea80 {
    const UnkGroundMovementVtbl_l99ea80* vtbl;
    u8 pad_0x8[0x38];
    UnkVec3f dir;  // 0x40
    u8 pad_0x4c[0xc];
    UnkVec3f normal;  // 0x58
    u8 pad_0x64[0x368];
    u32 mask;  // 0x3cc
};
static_assert(__builtin_offsetof(UnkGroundMovement_l99ea80, mask) == 0x3cc, "layout");

struct UnkGroundOwner_l99ea80 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
};

struct UnkGroundState_l99ea80 {
    u8 pad_0x0[0x30];
    UnkGroundOwner_l99ea80* owner;
    UnkGroundMovement_l99ea80* movement;
};

static inline UnkVec3f operator+(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}
static inline UnkVec3f operator*(f32 s, const UnkVec3f& v) {
    return UnkVec3f(s * v.x, s * v.y, s * v.z);
}
static inline UnkVec3f operator*(const UnkVec3f& v, f32 s) {
    return UnkVec3f(v.x * s, v.y * s, v.z * s);
}

// The x and y of a 3D vector.
static inline void Project(base::math::CVector2D* out, const UnkVec3f& v) {
    fn_710007b430(out, reinterpret_cast<const base::math::CVector3D*>(&v));
}

bool fn_710099ea80(UnkGroundState_l99ea80* self) {
    const UnkGroundTunable_l99ea80& tunable = reinterpret_cast<const UnkGroundTunable_l99ea80&>(g_unk_7172a03d80);
    if (!tunable.field_0x3f)
        return false;

    UnkGroundMovement_l99ea80* movement = self->movement;
    const UnkVec3f& dir = movement->dir;
    if (dir.x + 0.1f >= 0.0f && dir.x - 0.1f <= 0.0f)
        return false;
    const UnkVec3f& normal = movement->normal;
    if (dir.x * normal.x + dir.y * normal.y + dir.z * normal.z < 0.0f && fn_71009e1310(movement))
        return false;

    UnkGameManagerSub_0x608* collision = g_unk_7101d47310->field_0x608;
    if (!collision)
        return false;
    UnkTree* tree = static_cast<UnkTree*>(collision->field_0x820);
    UnkShape2D_l99ea80* shape = self->movement->vtbl->vf_0x2a8(self->movement)->shape;
    u32 type = shape->vtbl->GetType(shape);
    if (!shape || type != 8)
        return false;

    UnkCapsule_l99ea80 capsule;
    f32 height = fn_7100d60238(shape) * 2.0f;
    f32 radius = fn_7100d60230(shape);
    capsule.center_x = 0.0f;
    capsule.center_y = 0.0f;
    capsule.radius = radius;
    capsule.half_height = height * 0.5f;
    capsule.vtbl->vf_0x80(&capsule);
    fn_7100d60184(&capsule, 0.999f);

    UnkGroundOwner_l99ea80* owner = self->owner;
    UnkVec3f origin = UnkVec3f(owner->x, owner->y, owner->z) + shape->vtbl->vf_0x68(shape);
    f32 step = tunable.field_0x48;
    {
        // Clear ahead at the movement's height.
        u32 mask = self->movement->mask;
        UnkMatrix44_7100078c9c xf = BuildTranslationMatrixFromVec3(origin);
        UnkVec3f delta = step * dir;
        base::math::CVector2D delta2d;
        Project(&delta2d, delta);
        if (SweepShapeAgainstCollisionTree(tree, mask, reinterpret_cast<UnkShape*>(&capsule), reinterpret_cast<const UnkMtx44*>(&xf),
                          reinterpret_cast<const Vec2*>(&delta2d), nullptr, false))
            return false;
    }
    // Nothing right below the end of that sweep.
    UnkVec3f ahead = step * dir + origin + UnkVec3f(0.0f, -100.0f, 0.0f);
    {
        u32 mask = self->movement->mask;
        UnkMatrix44_7100078c9c xf = BuildTranslationMatrixFromVec3(ahead);
        if (QueryCollisionTreeForTransformedShapeOverlaps(tree, mask, reinterpret_cast<UnkShape*>(&capsule), reinterpret_cast<const UnkMtx44*>(&xf),
                          nullptr, 1))
            return false;
    }
    {
        // Clear further ahead from there.
        u32 mask = self->movement->mask;
        UnkMatrix44_7100078c9c xf = BuildTranslationMatrixFromVec3(ahead);
        UnkVec3f delta = dir * 100.0f;
        base::math::CVector2D delta2d;
        Project(&delta2d, delta);
        if (SweepShapeAgainstCollisionTree(tree, mask, reinterpret_cast<UnkShape*>(&capsule), reinterpret_cast<const UnkMtx44*>(&xf),
                          reinterpret_cast<const Vec2*>(&delta2d), nullptr, false))
            return false;
    }
    // And ground below the far end.
    UnkVec3f below = dir * 100.0f + ahead + UnkVec3f(0.0f, -10.0f, 0.0f);
    u32 mask = self->movement->mask;
    UnkMatrix44_7100078c9c xf = BuildTranslationMatrixFromVec3(below);
    if (!QueryCollisionTreeForTransformedShapeOverlaps(tree, mask, reinterpret_cast<UnkShape*>(&capsule), reinterpret_cast<const UnkMtx44*>(&xf),
                       nullptr, 1))
        return false;
    return !reinterpret_cast<UnkGameManagerFlags_l99ea80*>(g_unk_7101d47310)->field_0x13c;
}
