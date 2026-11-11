// CChozoWarriorXSpitMovementComponent's unit (tools/tu_map.py): after the base launch (fn_7100ac3880),
// creates the spit's scene object at +0x5d8 when there is none and the owner's collider named
// g_unk_71726cfd30 exists: half the collider's 2D width sets the object's timing (fn_710014826c), then
// it is configured (fn_71001beee4) and switched off. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSceneObject.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkSelf;
void fn_7100ac3880(UnkSelf*);
void* fn_7100149930(UnkRenderPool*, void*, UnkSceneObject**, base::global::CStrId*);
void fn_71001beee4(UnkSceneObjectSub* target, u32 mode);

struct UnkRGB_710050f26c {
    f32 r;
    f32 g;
    f32 b;
};

struct UnkColorArg_710050f26c {
    UnkRGB_710050f26c rgb;
    f32 a;
    UnkColorArg_710050f26c(const UnkRGB_710050f26c& rgb_, f32 a_) : rgb(rgb_), a(a_) {}
    UnkColorArg_710050f26c(const UnkColorArg_710050f26c& o) : rgb(o.rgb), a(o.a) {}
};
bool fn_710014826c(UnkSceneObject*, u32, u32, UnkColorArg_710050f26c, bool, f32);

extern const base::global::CStrId g_unk_71726cfd30 __attribute__((visibility("hidden")));  // the collider's name

// A collider's bounds, as its shape gives them.
struct UnkSpitBounds_ovb0127 {
    CVector3D min;
    CVector3D max;
};

struct UnkSpitRect_ovb0127 {
    CVector2D min;
    CVector2D max;
};

struct UnkSpitShape_ovb0127 {
    void** vtable;
    u8 field_0x8[0x8];
    UnkSpitBounds_ovb0127 Bounds() {
        return reinterpret_cast<UnkSpitBounds_ovb0127 (*)(UnkSpitShape_ovb0127*, void*)>(vtable[0x28 / 8])(this, field_0x8);
    }
};

struct UnkSpitCollider_ovb0127 {
    u8 pad_0x0[0x158];
    UnkSpitShape_ovb0127* shape;  // 0x158
};

struct UnkSpitOwner_ovb0127 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* colliders;  // 0x2a0
};

struct UnkChozoWarriorXSpitMovement_ovb0127 {
    u8 pad_0x0[0x30];
    UnkSpitOwner_ovb0127* owner;  // 0x30
    u8 pad_0x38[0x5a0];
    UnkSceneObject* effect;  // 0x5d8
};

void fn_710049069c(UnkChozoWarriorXSpitMovement_ovb0127* self) {
    fn_7100ac3880(reinterpret_cast<UnkSelf*>(self));
    if (self->effect)
        return;
    UnkCollisionContainer* colliders = self->owner->colliders;
    if (!colliders)
        return;
    auto* collider = reinterpret_cast<UnkSpitCollider_ovb0127*>(fn_71008640a4(colliders, &g_unk_71726cfd30));
    if (!collider)
        return;
    UnkSpitRect_ovb0127 rect;
    f32 half_width;
    {
        UnkSpitBounds_ovb0127 bounds = collider->shape->Bounds();
        fn_710007b430(&rect.min, &bounds.min);
        fn_710007b430(&rect.max, &bounds.max);
        half_width = (rect.max.x - rect.min.x) * 0.5f;
    }
    UnkRenderManager* manager = g_unk_7101d47300;
    UnkSceneObject* effect = nullptr;
    fn_7100149930(manager->field_0x218, nullptr, &effect, &g_unk_7101d470e0);
    fn_7100219ae0(manager, effect ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<char*>(effect) + 8) : nullptr, 0);
    self->effect = effect;
    fn_710014826c(effect, 6, 0x28, UnkColorArg_710050f26c(UnkRGB_710050f26c{0.0f, 0.1f, 1.0f}, 1.0f), true,
                  half_width);
    fn_71001beee4(self->effect->field_0x130, 1);
    fn_7100149490(self->effect, false);
}
