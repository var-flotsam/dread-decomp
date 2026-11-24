// CMovablePlatformComponent unit: before the platform moves by `delta`, sweeps the player's collision shape
// (backed off by one unit along the move) through the collision world against the move, ignoring the
// platform's own collider; on a hit returns false with `out` cut short to the hit, otherwise true with `out`
// the full move. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "base/global/CStrId.h"
#include "game/CCharacterMovement.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

bool fn_7100d5babc(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

extern void* g_unk_7101a84990[];  // A vtable.

struct UnkHitObj_l951bf4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkHitRef_l951bf4 {
    UnkHitObj_l951bf4* ptr;

    UnkHitRef_l951bf4() : ptr(nullptr) {}
    ~UnkHitRef_l951bf4() {
        if (UnkHitObj_l951bf4* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkHitVec_l951bf4 {
    f32 x, y, z;
    UnkHitVec_l951bf4(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

// A sweep's hit: the fraction of the move done before it at +0x38.
struct UnkCollisionHit_l951bf4 {
    UnkHitRef_l951bf4 object;
    UnkHitRef_l951bf4 field_0x8;
    s32 id;
    UnkHitVec_l951bf4 point;
    UnkHitVec_l951bf4 field_0x20;
    UnkHitVec_l951bf4 normal;
    f32 fraction;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l951bf4()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), fraction(0),
          flags(0), field_0x40(0) {}
};

// Where the sweep starts, its direction, the distance it tolerates and the collider it ignores.
struct UnkSweepRay_l951bf4 {
    CVector3D origin;
    CVector3D dir;
    f32 tolerance;
    UnkCollisionObject* ignore;

    UnkSweepRay_l951bf4(const CVector3D& o, const CVector3D& d, UnkCollisionObject* c)
        : origin(o), dir(d), tolerance(0.001f), ignore(c) {}
};

// The hit query the sweep fills (vtable g_unk_7101a84990): the ray it was given and the ray it is at.
struct UnkSweepQuery_l951bf4 {
    void** vtable;
    UnkCollisionHit_l951bf4 hit;
    bool field_0x50;
    UnkSweepRay_l951bf4 ray;
    bool field_0x80;
    UnkSweepRay_l951bf4 current;

    explicit UnkSweepQuery_l951bf4(const UnkSweepRay_l951bf4& r)
        : field_0x50(false), ray(r), field_0x80(false), current(r) {
        vtable = g_unk_7101a84990 + 2;
    }
    ~UnkSweepQuery_l951bf4() { vtable = g_unk_7101a20ed8 + 2; }
};
static_assert(sizeof(UnkSweepQuery_l951bf4) == 0xb0, "");

// A sweep offset: the x and y of a 3D one.
struct UnkSweepOffset_l951bf4 {
    CVector2D v;

    explicit UnkSweepOffset_l951bf4(const CVector3D* offset) { fn_710007b430(&v, offset); }
};

static inline bool Sweep(UnkTree* tree, u32 mask, UnkShape* shape, const UnkMatrix44_7100078c9c& xf,
                         const UnkSweepOffset_l951bf4& sweep, UnkSweepQuery_l951bf4* query) {
    return fn_7100d5babc(tree, mask, shape, reinterpret_cast<const UnkMtx44*>(&xf),
                         reinterpret_cast<const Vec2*>(&sweep.v), query, false);
}

struct UnkPlayerMovement_l951bf4 {
    u8 pad_0x0[0x3cc];
    u32 mask;
};

struct UnkPlayerShape_l951bf4 {
    u8 pad_0x0[0x158];
    UnkShape* shape;
};

struct UnkPlatformPlayer_l951bf4 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
    u8 pad_0x68[0x198];
    UnkPlayerMovement_l951bf4* movement;  // +0x200
};
static_assert(__builtin_offsetof(UnkPlatformPlayer_l951bf4, movement) == 0x200, "");

struct UnkPlatformOwner_l951bf4 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkPlatformDesc_l951bf4 {
    u8 pad_0x0[0x68];
    CStrId collider_name;
};

struct UnkMovablePlatform_l951bf4 {
    u8 pad_0x0[0x30];
    UnkPlatformOwner_l951bf4* owner;  // +0x30
    u8 pad_0x38[0x1f8];
    UnkPlatformDesc_l951bf4* desc;  // +0x230
};

bool fn_7100951bf4(UnkMovablePlatform_l951bf4* self, const CVector3D* delta, CVector3D* out) {
    out->x = delta->x;
    out->y = delta->y;
    out->z = delta->z;
    UnkPlatformPlayer_l951bf4* player =
        reinterpret_cast<UnkPlatformPlayer_l951bf4*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return true;
    f32 x = player->x;
    f32 y = player->y;
    f32 z = player->z;
    CVector3D dir = Normalized(*delta);
    UnkGameManagerSub_0x608* world = g_unk_7101d47310->field_0x608;
    if (!world)
        return true;
    UnkTree* tree = static_cast<UnkTree*>(world->field_0x820);
    if (!tree)
        return true;

    UnkPlayerShape_l951bf4* shape = reinterpret_cast<UnkPlayerShape_l951bf4*>(
        fn_7100976588(reinterpret_cast<UnkFlagged*>(player->movement)));
    UnkMatrix44_7100078c9c xf = BuildTranslationMatrixFromVec3(UnkVec3f(x - dir.x, y - dir.y, z - dir.z));
    CVector3D sweep = dir + *delta;
    CVector3D sweep_dir = Normalized(sweep);
    CVector3D origin = {xf.m[3], xf.m[7], xf.m[11]};
    UnkCollisionContainer* collision = self->owner->collision;
    UnkCollisionObject* own = collision ? fn_71008640a4(collision, &self->desc->collider_name) : nullptr;
    bool hit;
    {
        UnkSweepQuery_l951bf4 query(UnkSweepRay_l951bf4(origin, sweep_dir, own));
        hit = Sweep(tree, player->movement->mask, shape->shape, xf, UnkSweepOffset_l951bf4(&sweep), &query);
        if (hit)
            *out = sweep * query.hit.fraction - dir;
    }
    if (hit)
        return false;
    return true;
}
