// CMagnetSurfaceComponent's unit (tools/tu_map.py): switching the surface on or off (its collider and glow material, as src/game/CMagnetSlidingBlockComponent/CMagnetSlidingBlockComponent_Slide_ovb0086.cpp fades the block's) and building its collider from the tracked shape.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CChozoCommanderSentenceSphereAttack.h"
#include "game/CMaterialFXComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CStrId;
using base::math::UnkVector4D;

struct UnkStruct_7100233684;
base::reflection::UnkReflObject* fn_7100233684(UnkStruct_7100233684* self);
bool fn_7100199d88(void* table, const u64* key, u32* id);

struct UnkRefl_fn_710092482c;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_710092482c>(void*);

// The surface collider's names: the collider at +0 and three more it is created with.
struct UnkSurfaceNames_ovb0137 {
    CStrId collider;
    CStrId field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
};
extern const UnkSurfaceNames_ovb0137 g_unk_7172737d08 __attribute__((visibility("hidden")));

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};
UnkMatrix44_7100078c9c fn_7100078cd8(const UnkVec3f& t);

struct UnkTracker;
struct UnkTriggerCollider_b0058;

struct UnkMagnetColorChange_b0086 {
    u32 kind;
    u8 pad_0x4[0x34];
    union {
        base::UnkStrIdMember id;  // 0x38
    };
    u8 pad_0x40[0x18];

    UnkMagnetColorChange_b0086() {}  // built by fn_71009272c4
    ~UnkMagnetColorChange_b0086() { id.~UnkStrIdMemberOf(); }
};

struct UnkMagnetFx_b0086;
void fn_71009272c4(UnkMagnetColorChange_b0086* self, u32 index, bool flag, const UnkVector4D* from,
                   const UnkVector4D* to, f32 time);

struct UnkSurfaceMaterial_ovb0137 {
    u8 pad_0x0[0x14c];
    f32 r, g, b, a;  // 0x14c
};

struct UnkSurfaceMaterials_ovb0137 {
    u8 pad_0x0[0x48];
    UnkSurfaceMaterial_ovb0137** entries;  // 0x48
};

struct UnkSurfaceModelRes_ovb0137 {
    u8 pad_0x0[0x78];
    UnkSurfaceMaterials_ovb0137* materials;  // 0x78
};

struct UnkSurfaceModel_ovb0137 {
    u8 pad_0x0[0x138];
    UnkSurfaceModelRes_ovb0137* res;  // 0x138
};

struct UnkSurfaceModelComponent_ovb0137 {
    u8 pad_0x0[0x50];
    UnkSurfaceModel_ovb0137* model;  // 0x50
};

struct UnkMagnetSurface_ovb0137 {
    u8 pad_0x0[0x18];
    void* owner;  // 0x18
    u8 pad_0x20[0x18];
    bool on;  // 0x38
};

static inline f32 NonNegative(f32 v) { return v <= 0.0f ? 0.0f : v; }

// Switches the surface on or off: its collider (g_unk_7172737d08) and its glow material (key
// 0x117cd46281e7f67a), faded to full or no alpha.
void fn_7100924684(UnkMagnetSurface_ovb0137* self, bool on) {
    self->on = on;
    if (UnkCollisionContainer* collision = static_cast<UnkCollisionContainer*>(fn_710043acb4(self->owner))) {
        if (UnkCollisionObject* collider = fn_71008640a4(collision, &g_unk_7172737d08.collider))
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(collider), on);
    }
    u64 key = 0x117cd46281e7f67a;
    UnkSurfaceModelComponent_ovb0137* model = reinterpret_cast<UnkSurfaceModelComponent_ovb0137*>(
        fn_7100233684(static_cast<UnkStruct_7100233684*>(self->owner)));
    UnkMagnetFx_b0086* fx = reinterpret_cast<UnkMagnetFx_b0086*>(
        base::reflection::refl_FindTyped<UnkRefl_fn_710092482c>(self->owner));
    if (!model || !fx)
        return;
    UnkSurfaceModel_ovb0137* m = model->model;
    if (!m)
        return;
    u32 index;
    if (!fn_7100199d88(m->res->materials, &key, &index))
        return;
    u32 i = index;
    UnkSurfaceMaterial_ovb0137* mat = m->res->materials->entries[i];
    f32 r = NonNegative(mat->r);
    f32 g = NonNegative(mat->g);
    f32 b = NonNegative(mat->b);
    f32 a = NonNegative(mat->a);
    CColor4F to(r, g, b, a);
    CColor4F from(to);
    from.a = on ? 1.0f : 0.0f;
    UnkMagnetColorChange_b0086 change;
    UnkVector4D vto = fn_7100030878(&to);
    UnkVector4D vfrom = fn_7100030878(&from);
    fn_71009272c4(&change, i, false, &vto, &vfrom, 0.5f);
    change.kind = 2;
    fn_7100926aa0(fx, &change);
}

struct UnkSurfaceShape_ovb0137 {
    void** vtable;
    UnkMatrix44_7100078c9c transform;  // 0x8

    u32 vf_0x18() { return reinterpret_cast<u32 (*)(UnkSurfaceShape_ovb0137*)>(vtable[0x18 / 8])(this); }
    UnkVec3f vf_0x68() { return reinterpret_cast<UnkVec3f (*)(UnkSurfaceShape_ovb0137*)>(vtable[0x68 / 8])(this); }
    void vf_0x80() { reinterpret_cast<void (*)(UnkSurfaceShape_ovb0137*)>(vtable[0x80 / 8])(this); }
};

struct UnkSurfaceTracker_ovb0137 {
    u8 pad_0x0[0x30];
    UnkSurfaceShape_ovb0137* shape;  // 0x30
};


struct UnkSurfaceCollision_ovb0137 {
    void** vtable;

    UnkTriggerCollider_b0058* vf_0x168(const CStrId* name, const CStrId* a, const CStrId* b, const CStrId* c,
                                       u32 flags, bool f1, bool f2, void* p1, u32 s1, void* p2, u32 s2, void* p3,
                                       u32 s3, bool f3, bool f4, bool f5) {
        using Fn = UnkTriggerCollider_b0058* (*)(UnkSurfaceCollision_ovb0137*, const CStrId*, const CStrId*,
                                                 const CStrId*, const CStrId*, u32, bool, bool, void*, u32, void*,
                                                 u32, void*, u32, bool, bool, bool);
        return reinterpret_cast<Fn>(vtable[0x168 / 8])(this, name, a, b, c, flags, f1, f2, p1, s1, p2, s2, p3, s3,
                                                       f3, f4, f5);
    }
};

struct UnkSurfaceCollider_ovb0137 {
    void** vtable;
    void vf_0x38() { reinterpret_cast<void (*)(UnkSurfaceCollider_ovb0137*)>(vtable[0x38 / 8])(this); }
};

struct UnkSurfaceOwner_ovb0137 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

// A position relative to the owner's translation.
static inline UnkVec3f Offset(const UnkVec3f& pos, const UnkSurfaceOwner_ovb0137* owner) {
    return UnkVec3f(pos.x - owner->transform.m[3], pos.y - owner->transform.m[7], pos.z - owner->transform.m[11]);
}

// Builds the surface's collider from the tracked shape (a kind 0x10 shape gets a new one) and moves
// the shape to its position relative to the owner.
void fn_71009244a4(UnkMagnetSurface_ovb0137* self) {
    UnkSurfaceTracker_ovb0137* tracker = reinterpret_cast<UnkSurfaceTracker_ovb0137*>(fn_710028aca8(self->owner));
    if (!tracker)
        return;
    UnkSurfaceCollision_ovb0137* collision = static_cast<UnkSurfaceCollision_ovb0137*>(fn_710043acb4(self->owner));
    if (!collision)
        return;
    if (UnkSurfaceShape_ovb0137* shape = tracker->shape) {
        if (shape->vf_0x18() == 0x10) {
            UnkTriggerCollider_b0058* collider =
                collision->vf_0x168(&g_unk_7172737d08.collider, &g_unk_7172737d08.field_0x8,
                                    &g_unk_7172737d08.field_0x18, &g_unk_7172737d08.field_0x10, 0x20800, true, false,
                                    nullptr, 0xffffffff, nullptr, 0xffffffff, nullptr, 0xffffffff, false, false,
                                    true);
            fn_7100d36d30(collider, shape);
        }
    }
    UnkSurfaceShape_ovb0137* shape = tracker->shape;
    const UnkMatrix44_7100078c9c& m =
        fn_7100078cd8(Offset(shape->vf_0x68(), static_cast<UnkSurfaceOwner_ovb0137*>(self->owner)));
    UnkCopyMatrix(reinterpret_cast<UnkMatrix44*>(&shape->transform), reinterpret_cast<const UnkMatrix44&>(m));
    shape->vf_0x80();
    reinterpret_cast<UnkSurfaceCollider_ovb0137*>(
        fn_71008640a4(reinterpret_cast<UnkCollisionContainer*>(collision), &g_unk_7172737d08.collider))
        ->vf_0x38();
}
