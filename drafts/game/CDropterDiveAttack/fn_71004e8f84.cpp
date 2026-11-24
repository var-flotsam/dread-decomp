// CDropterDiveAttack's unit (tools/tu_map.py unit 778): whether the dive has a clear line to the target: a circle
// placed just ahead of the owner and swept back must hit nothing, and one swept from the owner to 50 units past
// the target's "Chest001" point must hit the target's own body. Names and layouts are placeholders.
#include <algorithm>

#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkVec3fR;
void RoundVector3ComponentsHalfAwayFromZero(UnkVec3fR* v);

struct UnkPlayerMovementCache;
void* fn_71010d7a6c(UnkPlayerMovementCache* self);

bool SweepShapeAgainstCollisionTree(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);

__attribute__((visibility("hidden"))) extern const CVector3D g_unk_7101649ca4;

// A transform as the collision object's vf_0x70 returns it: the translation column at +0xc, +0x1c, +0x2c.
struct UnkDiveMatrix_l4e8f84 {
    u8 pad_0x0[0xc];
    f32 x;
    u8 pad_0x10[0xc];
    f32 y;
    u8 pad_0x20[0xc];
    f32 z;
};

static inline UnkDiveMatrix_l4e8f84* Transform(UnkCollisionObject* object) {
    return (*reinterpret_cast<UnkDiveMatrix_l4e8f84* (***)(UnkCollisionObject*)>(object))[0x70 / 8](object);
}

struct UnkDiveTarget_l4e8f84 {
    u8 pad_0x0[0x200];
    UnkPlayerMovementCache* movement;  // +0x200
    u8 pad_0x208[0x8];
    void* field_0x210;
    u8 pad_0x218[0x88];
    UnkCollisionContainer* collision;  // +0x2a0
};

// What the hit object belongs to: the bodies it stands for.
struct UnkDiveHitOwner_l4e8f84 {
    u8 pad_0x0[0x164];
    u32 count;
    void* bodies[1];
};

struct UnkDiveHitObject_l4e8f84 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
    UnkDiveHitOwner_l4e8f84* owner;
};

struct UnkDiveHitRef_l4e8f84 {
    UnkDiveHitObject_l4e8f84* ptr;

    UnkDiveHitRef_l4e8f84() : ptr(nullptr) {}
    ~UnkDiveHitRef_l4e8f84() {
        if (UnkDiveHitObject_l4e8f84* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkDiveHitVec_l4e8f84 {
    f32 x, y, z;
    UnkDiveHitVec_l4e8f84(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkDiveHit_l4e8f84 {
    UnkDiveHitRef_l4e8f84 object;
    UnkDiveHitRef_l4e8f84 field_0x8;
    s32 id;
    UnkDiveHitVec_l4e8f84 point;
    UnkDiveHitVec_l4e8f84 field_0x20;
    UnkDiveHitVec_l4e8f84 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkDiveHit_l4e8f84()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

// The sweep's callback object: the first hit and where the sweep starts.
struct UnkDiveQuery_l4e8f84 {
    void** vtable;
    UnkDiveHit_l4e8f84 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkDiveQuery_l4e8f84(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkDiveQuery_l4e8f84() { vtable = g_unk_7101a20ed8 + 2; }
};

struct UnkLavaDropsShape_A2_b71 {
    u8 pad_0x0[0x8];
    u8 body[0xa8];

    UnkLavaDropsShape_A2_b71(const void* source, f32 size) { fn_7100d6169c(this, source, size); }
    ~UnkLavaDropsShape_A2_b71() { fn_7100d61750(this); }
};

// A 4x4 transform: the identity moved to a point.
struct UnkDiveMtx_l4e8f84 {
    f32 m[16];

    explicit UnkDiveMtx_l4e8f84(const CVector3D& at) {
        m[0] = 1.0f;
        m[1] = 0.0f;
        m[2] = 0.0f;
        m[3] = 0.0f;
        m[4] = 0.0f;
        m[5] = 1.0f;
        m[6] = 0.0f;
        m[7] = 0.0f;
        m[8] = 0.0f;
        m[9] = 0.0f;
        m[10] = 1.0f;
        m[11] = 0.0f;
        m[12] = 0.0f;
        m[13] = 0.0f;
        m[14] = 0.0f;
        m[15] = 1.0f;
        m[3] = at.x;
        m[7] = at.y;
        m[11] = at.z;
    }
};

struct UnkDropterDive_l4e8f84 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // +0x8
};

static inline bool Sweep(UnkTree* tree, u32 mask, UnkLavaDropsShape_A2_b71* shape, const UnkDiveMtx_l4e8f84& xf,
                         const CVector2D& delta, UnkDiveQuery_l4e8f84* query) {
    return SweepShapeAgainstCollisionTree(tree, mask, reinterpret_cast<UnkShape*>(shape), reinterpret_cast<const UnkMtx44*>(&xf),
                         reinterpret_cast<const Vec2*>(&delta), query, false);
}

bool fn_71004e8f84(UnkDropterDive_l4e8f84* self) {
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return false;
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    CStrId name("Chest001", true);
    UnkDiveTarget_l4e8f84* view = reinterpret_cast<UnkDiveTarget_l4e8f84*>(target);
    UnkCollisionObject* chest = fn_71008640a4(view->collision, &name);
    const f32* px;
    const f32* py;
    const f32* pz;
    if (view->field_0x210 || fn_71009e12d0(target->field_0x208)) {
        px = &target->field_0x44;
        py = &target->field_0x54;
        pz = &target->field_0x64;
    } else {
        UnkDiveMatrix_l4e8f84* m = Transform(chest);
        px = &m->x;
        py = &m->y;
        pz = &m->z;
    }
    f32 y = *py;
    f32 x = *px;
    f32 z = *pz;
    CVector3D aim = {x, y, z};
    RoundVector3ComponentsHalfAwayFromZero(reinterpret_cast<UnkVec3fR*>(&aim));

    UnkActor* owner = self->owner;
    CVector3D from = {owner->field_0x44, owner->field_0x54, owner->field_0x64};
    CVector3D delta = aim - from;
    f32 distance = Length(delta);
    CVector3D flat = {delta.x, delta.y, 0.0f};
    CVector3D dir = Normalized(flat);

    void* body = fn_71010d7a6c(view->movement);
    if (!body)
        return false;

    distance += 50.0f;
    CVector3D ahead = dir * 12.0f + from;
    CVector2D start;
    fn_710007b430(&start, &from);
    CVector2D back;
    {
        CVector3D point = ahead - dir * 130.0f;
        fn_710007b430(&back, &point);
    }
    CVector2D past;
    {
        CVector3D point = dir * distance + from;
        fn_710007b430(&past, &point);
    }
    CVector2D back_sweep = {back.x - start.x, back.y - start.y};
    CVector2D sweep = {past.x - start.x, past.y - start.y};

    UnkLavaDropsShape_A2_b71 circle(&g_unk_7101649ca4, 40.0f);
    UnkDiveMtx_l4e8f84 at_owner(from);
    UnkDiveMtx_l4e8f84 at_ahead(ahead);
    const u32 mask = 0x80429;
    UnkDiveQuery_l4e8f84 blocked(CVector3D{start.x, start.y, 0.0f});
    if (Sweep(tree, mask, &circle, at_ahead, back_sweep, &blocked))
        return false;
    bool reached = false;
    {
        UnkDiveQuery_l4e8f84 query(CVector3D{start.x, start.y, 0.0f});
        if (Sweep(tree, mask, &circle, at_owner, sweep, &query)) {
            UnkDiveHitOwner_l4e8f84* hit_owner = query.hit.object.ptr->owner;
            u32 count = hit_owner->count;
            if (count != 0) {
                void** first = hit_owner->bodies;
                void** last = first + count;
                reached = std::find(first, last, body) != last;
            }
        }
    }
    return reached;
}
