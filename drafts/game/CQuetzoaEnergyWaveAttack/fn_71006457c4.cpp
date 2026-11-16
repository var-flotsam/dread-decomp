// CQuetzoaEnergyWaveAttack's unit (tools/tu_map.py unit 968): whether the energy wave may be fired: a
// circle swept from just ahead of the attacker along the aim direction must hit something whose flags
// (virtual slot 0x58) have bit 4 set. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU365.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "types.h"

namespace base::reflection {
struct UnkTunable;
}
using base::reflection::UnkTunable;

// As the caller declares them (src/game/CQuetzoaEnergyWaveAttack/CQuetzoaEnergyWaveAttack_Aim_ovb0102.cpp).
struct UnkEnergyWaveAttack_ovb0102;
struct UnkVec3_ovb0102 {
    f32 x, y, z;
};

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

struct UnkTree;
struct UnkShape;
struct UnkMtx44;
struct Vec2;
bool fn_7100d5babc(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);

struct UnkLavaDropsShape_A2_b71;
void fn_7100d61750(UnkLavaDropsShape_A2_b71* self);

extern void* g_unk_7101a23590[];
extern UnkTunable g_unk_71726fb238;

// The attack's tunable as read here: the sweep length per unit of direction at +0x24.
struct UnkEnergyWaveTunable_l6457c4 {
    u8 pad_0x0[0x24];
    f32 sweep_length;  // 0x24
};

struct UnkWaveCollider_l6457c4 {
    u8 pad_0x0[0x218];
    void* field_0x218;
    u8 pad_0x220[0x1ac];
    u32 mask;  // 0x3cc
};

struct UnkWaveOwnerLink_l6457c4;
UnkTree* fn_710110c498(UnkWaveOwnerLink_l6457c4* link);

struct UnkWaveActor_l6457c4 {
    u8 pad_0x0[0x218];
    UnkWaveCollider_l6457c4* collider;  // 0x218
    u8 pad_0x220[0x20];
    UnkWaveOwnerLink_l6457c4* link;  // 0x240
};

struct UnkWaveAttack_l6457c4 {
    void** vtable;
    UnkWaveActor_l6457c4* actor;  // 0x8
};

// What a hit record points at: its flags come from virtual slot 0x58.
struct UnkWaveHitTarget_l6457c4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual u32 vf_0x58();
};

// A reference-counted hit object, deleted through virtual slot 0x10.
struct UnkWaveHitObj_l6457c4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
    UnkWaveHitTarget_l6457c4* target;  // 0x10
};

struct UnkWaveHitRef_l6457c4 {
    UnkWaveHitObj_l6457c4* ptr;

    UnkWaveHitRef_l6457c4() : ptr(nullptr) {}
    ~UnkWaveHitRef_l6457c4() {
        if (UnkWaveHitObj_l6457c4* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkWaveHitVec_l6457c4 {
    f32 x, y, z;
    UnkWaveHitVec_l6457c4(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkWaveHit_l6457c4 {
    UnkWaveHitRef_l6457c4 object;
    UnkWaveHitRef_l6457c4 field_0x8;
    s32 id;
    UnkWaveHitVec_l6457c4 point;
    UnkWaveHitVec_l6457c4 field_0x20;
    UnkWaveHitVec_l6457c4 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkWaveHit_l6457c4()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

// The hit query the sweep fills in.
struct UnkWaveHitQuery_l6457c4 {
    void** vtable;
    UnkWaveHit_l6457c4 hit;
    bool field_0x50;

    UnkWaveHitQuery_l6457c4() {
        vtable = g_unk_7101a23590 + 2;
        field_0x50 = false;
    }
    ~UnkWaveHitQuery_l6457c4() { vtable = g_unk_7101a20ed8 + 2; }
};

// A circle shape (CCircleShape2D) on the stack.
struct UnkWaveCircle_l6457c4 {
    u64 storage[0x16];

    explicit UnkWaveCircle_l6457c4(f32 radius) {
        fn_7100d61664(this);
        fn_7100d618f8(reinterpret_cast<UnkSphereShape_ovb0135*>(this), radius);
    }
    ~UnkWaveCircle_l6457c4() { fn_7100d61750(reinterpret_cast<UnkLavaDropsShape_A2_b71*>(this)); }
};

bool fn_71006457c4(UnkEnergyWaveAttack_ovb0102* attack, const UnkVec3_ovb0102* pos, const UnkVec3_ovb0102* dir) {
    UnkWaveAttack_l6457c4* self = reinterpret_cast<UnkWaveAttack_l6457c4*>(attack);
    f32 sign = (f32)((dir->x > 0.0f) - (dir->x < 0.0f));
    UnkVec3f start(pos->x + sign * 115.0f, pos->y + 20.0f, pos->z + 0.0f);

    UnkGameManagerSub_0x608* scenario = g_unk_7101d47310->field_0x608;
    UnkTree* tree = scenario ? static_cast<UnkTree*>(scenario->field_0x820) : nullptr;
    if (!scenario)
        return false;
    bool clear = false;
    UnkWaveActor_l6457c4* actor = self->actor;
    UnkWaveOwnerLink_l6457c4* link = actor->link;
    if (!link)
        return clear;
    UnkWaveCollider_l6457c4* collider = actor->collider;
    if (!collider)
        return clear;
    if (tree) {
        u32 mask = collider->mask;
        UnkWaveHitQuery_l6457c4 query;
        UnkMatrix44_7100078c9c xf = fn_7100078cd8(start);
        UnkWaveCircle_l6457c4 circle(50.0f);
        f32 length = reinterpret_cast<const UnkEnergyWaveTunable_l6457c4*>(&g_unk_71726fb238)->sweep_length;
        base::math::CVector2D dir2;
        fn_710007b430(&dir2, reinterpret_cast<const base::math::CVector3D*>(dir));
        base::math::CVector2D delta = {length * dir2.x, length * dir2.y};

        bool hit = fn_7100d5babc(tree, mask, reinterpret_cast<UnkShape*>(&circle),
                                 reinterpret_cast<const UnkMtx44*>(&xf), reinterpret_cast<const Vec2*>(&delta), &query,
                                 false);
        if (fn_710110c498(link))
            hit = hit | fn_7100d5babc(fn_710110c498(link), mask, reinterpret_cast<UnkShape*>(&circle),
                                      reinterpret_cast<const UnkMtx44*>(&xf), reinterpret_cast<const Vec2*>(&delta),
                                      &query, false);
        if (hit) {
            if (UnkWaveHitObj_l6457c4* object = query.hit.object.ptr) {
                if (UnkWaveHitTarget_l6457c4* target = object->target)
                    clear = (target->vf_0x58() & 0x10) != 0;
            }
        }
    }
    return clear;
}
