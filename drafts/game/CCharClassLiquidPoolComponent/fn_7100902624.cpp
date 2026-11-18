// CCharClassLiquidPoolComponent's unit (tools/tu_map.py unit 1394): turns the pool solid or back to liquid. The
// owner and the collider are raised by 25 (solid) or lowered back, the collider and its companion change mode,
// the scenario's collision tracker is told, the scene switches one pair of effects off and the other on, and
// the owner's navigation item is tagged "solid" or "liquid". Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU372.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::reflection::UnkReflObject;

// As src/game/CAreaFXComponent/CAreaFXComponent_Colliders_ovb0081.cpp declares it.
struct UnkTracker_71008618cc;
void fn_7100d59994(UnkTracker_71008618cc*, UnkReflObject*);

struct UnkStruct_7100861148_Collider;

// Switches the scene's effect named `id` on or off.
void fn_7100199854(void* effects, const CFilePathStrId* id, bool enable);

struct UnkLiquidPoolEffects_l902624 {
    u8 pad_0x0[0x78];
    void* field_0x78;
};

struct UnkLiquidPoolScene_l902624 {
    u8 pad_0x0[0x138];
    UnkLiquidPoolEffects_l902624* effects;  // 0x138
};

struct UnkLiquidPoolModel_l902624 {
    u8 pad_0x0[0x50];
    UnkLiquidPoolScene_l902624* scene;  // 0x50
};

struct UnkLiquidPoolOwner_l902624 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x194];
    UnkLiquidPoolModel_l902624* model;  // 0x228
    u8 pad_0x230[0x180];
    bool moved;  // 0x3b0

    void MoveBy(const UnkVec3f& d) {
        f32 x = xform.m[0][3] + d.x;
        f32 y = xform.m[1][3] + d.y;
        f32 z = xform.m[2][3] + d.z;
        xform.m[0][3] = x;
        xform.m[1][3] = y;
        xform.m[2][3] = z;
        moved = true;
    }
};
static_assert(__builtin_offsetof(UnkLiquidPoolOwner_l902624, model) == 0x228, "layout");
static_assert(__builtin_offsetof(UnkLiquidPoolOwner_l902624, moved) == 0x3b0, "layout");

struct UnkLiquidPoolCollider_l902624 {
    void** vtable;

    void vf_0x60(u32 mode) {
        reinterpret_cast<void (*)(UnkLiquidPoolCollider_l902624*, u32)>(vtable[0x60 / 8])(this, mode);
    }
    const UnkMatrix44* vf_0x70() {
        return reinterpret_cast<const UnkMatrix44* (*)(UnkLiquidPoolCollider_l902624*)>(vtable[0x70 / 8])(this);
    }
    void vf_0x78(const UnkMatrix44* m) {
        reinterpret_cast<void (*)(UnkLiquidPoolCollider_l902624*, const UnkMatrix44*)>(vtable[0x78 / 8])(this, m);
    }

};

struct UnkLiquidPoolSolid_l902624 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkLiquidPoolOwner_l902624* owner;  // 0x30
    u8 pad_0x38[0x50];
    UnkLiquidPoolCollider_l902624* collider;   // 0x88
    UnkLiquidPoolCollider_l902624* companion;  // 0x90
    u8 pad_0x98[0xb78];
    bool solid;  // 0xc10
    u8 pad_0xc11[0xf];
    CFilePathStrId liquid_off;  // 0xc20: switched off when the pool turns liquid
    CFilePathStrId liquid_on;   // 0xc28
    CFilePathStrId solid_off;   // 0xc30
    CFilePathStrId solid_on;    // 0xc38

    // Moves the collider's transform by `d`.
    void MoveColliderBy(const UnkVec3f& d) {
        UnkMatrix44 m;
        UnkCopyMatrix(&m, *collider->vf_0x70());
        m.m[3] += d.x;
        m.m[7] += d.y;
        m.m[11] += d.z;
        collider->vf_0x78(&m);
    }

    void vf_0x228(bool solid) {
        reinterpret_cast<void (*)(UnkLiquidPoolSolid_l902624*, bool)>(vtable[0x228 / 8])(this, solid);
    }
};
static_assert(__builtin_offsetof(UnkLiquidPoolSolid_l902624, solid) == 0xc10, "layout");
static_assert(__builtin_offsetof(UnkLiquidPoolSolid_l902624, solid_on) == 0xc38, "layout");

static inline UnkVec3f Negated(const UnkVec3f& v) {
    return UnkVec3f(-v.x, -v.y, -v.z);
}

static inline bool IsSet(const CFilePathStrId& id) {
    return &g_unk_7101d47200->empty != id.data;
}

// The navigation tags.
static const CStrId& SolidTag() {
    static CStrId s_solid("solid", true);
    return s_solid;
}

static const CStrId& LiquidTag() {
    static CStrId s_liquid("liquid", true);
    return s_liquid;
}

void fn_7100902624(UnkLiquidPoolSolid_l902624* self, bool solid) {
    if (self->solid == solid || !self->collider)
        return;
    self->solid = solid;
    self->vf_0x228(solid);

    UnkGameManagerSub_0x608* scenario = g_unk_7101d47310->field_0x608;
    UnkTracker_71008618cc* tracker =
        scenario ? static_cast<UnkTracker_71008618cc*>(scenario->field_0x820) : nullptr;
    const UnkVec3f lift(0.0f, 25.0f, 0.0f);
    if (solid) {
        self->collider->vf_0x60(0x40b);
        self->owner->MoveBy(lift);
        self->MoveColliderBy(lift);
        if (UnkLiquidPoolCollider_l902624* companion = self->companion)
            companion->vf_0x60(1);
        fn_7100d59994(tracker, reinterpret_cast<UnkReflObject*>(self->collider));
        // The scene switches the solid pair: one effect off, the other on.
        if (UnkLiquidPoolOwner_l902624* owner = self->owner) {
            if (owner->model) {
                if (UnkLiquidPoolScene_l902624* scene = owner->model->scene) {
                    if (IsSet(self->solid_off))
                        fn_7100199854(scene->effects->field_0x78, &self->solid_off, false);
                    if (IsSet(self->solid_on))
                        fn_7100199854(scene->effects->field_0x78, &self->solid_on, true);
                }
            }
        }
    } else {
        self->owner->MoveBy(Negated(lift));
        self->MoveColliderBy(Negated(lift));
        self->collider->vf_0x60(0);
        if (UnkLiquidPoolCollider_l902624* companion = self->companion)
            companion->vf_0x60(0);
        fn_7100d5abfc(tracker, reinterpret_cast<UnkStruct_7100861148_Collider*>(self->collider));
        // The scene switches the liquid pair: one effect off, the other on.
        if (UnkLiquidPoolOwner_l902624* owner = self->owner) {
            if (owner->model) {
                if (UnkLiquidPoolScene_l902624* scene = owner->model->scene) {
                    if (IsSet(self->liquid_off))
                        fn_7100199854(scene->effects->field_0x78, &self->liquid_off, false);
                    if (IsSet(self->liquid_on))
                        fn_7100199854(scene->effects->field_0x78, &self->liquid_on, true);
                }
            }
        }
    }

    if (void* nav = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner))) {
        const CStrId& tag = solid ? SolidTag() : LiquidTag();
        fn_71009c0f1c(nav, tag.data->Key());
    }
}
