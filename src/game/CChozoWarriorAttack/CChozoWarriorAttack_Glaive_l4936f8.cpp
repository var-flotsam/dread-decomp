// CChozoWarriorAttack's unit (tools/tu_map.py unit 728): while the CCharClassChozoWarriorEnableGlaiveTrack
// holds, sweeps the glaive's collider (a 2D polygon collection, created on first use) over the triangle
// between the hand, the glaive's tip now and its tip last time. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU108.h"
#include "game/UnkTU54.h"
#include "game/UnkTileOutline.h"
#include "game/UnkTilePoint.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkVec3_71003e6530;

static_assert(sizeof(UnkTileOutline) == 0x48, "UnkTileOutline placeholder layout");

__attribute__((visibility("hidden"))) extern CStrId g_unk_71726d0890;  // the glaive collider's name
extern char g_unk_7101649998[];                                         // the hand's node key

// The glaive track's record: the glaive's length at +0x18.
struct UnkGlaiveTrack_l4936f8 {
    u8 pad_0x0[0x18];
    f32 length;
};

// The collider's shape: slot 0x18 answers its kind (16 for a polygon collection).
struct UnkGlaiveShape_l4936f8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
    u8 pad_0x8[0x90];
    bool field_0x98;  // set once the outline changed
};

struct UnkGlaiveCollider_l4936f8 {
    void** vtable;
    u8 pad_0x8[0x150];
    UnkGlaiveShape_l4936f8* shape;  // 0x158
};

using UnkCreateCollider_l4936f8 = UnkGlaiveCollider_l4936f8* (*)(UnkCollisionContainer*, const CStrId*,
                                                                  const CStrId*, const CStrId*, const CStrId*, u32,
                                                                  u32, u32, void*, s32, void*, s32, void*, s32,
                                                                  bool, bool, bool);

// An empty string id: the pool's empty string, the pool created on first use.
struct UnkEmptyId_l4936f8 {
    base::global::UnkStringData* data;

    UnkEmptyId_l4936f8() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    ~UnkEmptyId_l4936f8() { reinterpret_cast<CStrId*>(this)->~CStrId(); }
};

struct UnkChozoWarriorActor_l4936f8 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    UnkEntity* model;                  // 0x228
    u8 pad_0x230[0x70];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkChozoWarriorAttack_l4936f8 {
    void** vtable;
    UnkChozoWarriorActor_l4936f8* actor;  // 0x8
    u8 pad_0x10[0x20];
    UnkVec3f last_tip;  // 0x30
};

// Appends the point of `v` projected on the plane (z 0).
static inline void AddPoint(UnkTileOutline* outline, UnkTilePoint& point, CVector2D& p, const CVector3D* v) {
    fn_710007b430(&p, v);
    point.x = p.x;
    point.y = p.y;
    point.z = 0.0f;
    fn_7100118b94(outline, reinterpret_cast<UnkVec3_71003e6530*>(&point));
}

// The glaive's tip: `length` along the hand's -y axis, through the hand's matrix `m`.
static inline UnkVec3f TipOf(const UnkMatrix44& m, f32 length) {
    UnkVec3f along(0.0f, -length, 0.0f);
    return TransformPointByMatrixWithPerspectiveDivide(reinterpret_cast<const UnkMat4f*>(&m), &along);
}

void CChozoWarriorAttack_UpdateSweptTriangleCollider(UnkChozoWarriorAttack_l4936f8* self) {
    UnkChozoWarriorActor_l4936f8* actor = self->actor;
    UnkAnimationComponent* animation = actor->animation;
    UnkEntity* model = actor->model;
    UnkCollisionContainer* collision = actor->collision;
    if (!animation || !model || !collision)
        return;
    UnkGlaiveCollider_l4936f8* collider =
        reinterpret_cast<UnkGlaiveCollider_l4936f8*>(fn_71008640a4(collision, &g_unk_71726d0890));
    UnkTrackQuery query = {fn_7100bf82c8(), nullptr, -1.0f};
    fn_710117b08c(animation, &query);
    if (!query.result) {
        if (collider)
            fn_7100863a8c(collision, &g_unk_71726d0890);
        return;
    }

    UnkMatrix44 m = {};
    m.m[15] = 1.0f;
    m.m[10] = 1.0f;
    m.m[5] = 1.0f;
    m.m[0] = 1.0f;
    fn_71011735e8(model, g_unk_7101649998, &m, 0);
    UnkVec3f hand(m.m[3], m.m[7], m.m[11]);
    UnkVec3f tip = TipOf(m, reinterpret_cast<UnkGlaiveTrack_l4936f8*>(query.result)->length);

    if (!collider) {
        self->last_tip = tip;
        UnkEmptyId_l4936f8 none;
        const CStrId* type = reinterpret_cast<const CStrId*>(
            reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + 0x1f98);  // "POLYCOLLECTION2D"
        UnkCreateCollider_l4936f8 create = (*reinterpret_cast<UnkCreateCollider_l4936f8**>(collision))[0x168 / 8];
        collider = create(collision, &g_unk_71726d0890, &g_unk_71726d0890, reinterpret_cast<const CStrId*>(&none),
                          type, 0x20000100, 0, 1, nullptr, -1, nullptr, -1, nullptr, -1, false, false, true);
        if (!collider)
            return;
    }
    UnkGlaiveShape_l4936f8* shape = collider->shape;
    if (!shape || shape->vf_0x18() != 16)
        return;
    UnkTileOutlines* outlines = reinterpret_cast<UnkTileOutlines*>(shape);
    fn_7100d633ec(outlines);
    UnkTileOutline outline;
    UnkTilePoint point;
    CVector2D p;
    AddPoint(&outline, point, p, reinterpret_cast<const CVector3D*>(&hand));
    AddPoint(&outline, point, p, reinterpret_cast<const CVector3D*>(&tip));
    AddPoint(&outline, point, p, reinterpret_cast<const CVector3D*>(&self->last_tip));
    fn_7100119f98(&outline, true);
    fn_7100d62d40(outlines, &outline);
    shape->field_0x98 = true;
    self->last_tip = tip;
}
