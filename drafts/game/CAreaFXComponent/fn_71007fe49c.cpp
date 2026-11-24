// CAreaFXComponent unit (tools/tu_map.py unit 1263): refreshes the area effect. Enables the owner's
// effect (or its shape), takes the area's centre (the owner's position or the saved one), builds the
// area's rectangle around it, fits it (fn_71007fefc4, which also gives an offset the owner is moved by
// when it is not zero), creates the effect with it and keeps the new centre. Names and layouts are
// placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/CEnemyMovement.h"
#include "game/CSceneComponent.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

extern const CVector2D g_unk_7101647778;

struct UnkAreaFXColl_ovb0081;
struct UnkFXComponentSelf;
void CAreaFXComponent_CollectOwnerColliders(UnkAreaFXColl_ovb0081* self);
void fn_71007ff210(UnkFXComponentSelf* self, const f32* color);

struct UnkAreaFXOwner_l7fe49c {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1b0];
    void* movement;  // 0x218
    u8 pad_0x220[0x8];
    void* shape;  // 0x228
    u8 pad_0x230[0x180];
    bool moved;  // 0x3b0

    CVector3D Position() const { return CVector3D{x, y, z}; }
};
static_assert(__builtin_offsetof(UnkAreaFXOwner_l7fe49c, moved) == 0x3b0, "");

struct UnkAreaFXDef_l7fe49c {
    u8 pad_0x0[0x110];
    CVector2D size;  // 0x110
};

// An axis-aligned rectangle: its low and high corners.
struct UnkAreaRect_l7fe49c {
    CVector2D lo;
    CVector2D hi;
    ~UnkAreaRect_l7fe49c() {}
};

struct UnkAreaFXComponent_l7fe49c {
    u8 pad_0x0[0x30];
    UnkAreaFXOwner_l7fe49c* owner;  // 0x30
    u8 pad_0x38[0x3b0];
    CVector3D centre;  // 0x3e8
    bool use_centre;   // 0x3f4
    u8 pad_0x3f5[0x3];
    UnkAreaFXDef_l7fe49c* def;  // 0x3f8
};
static_assert(__builtin_offsetof(UnkAreaFXComponent_l7fe49c, centre) == 0x3e8, "");

void* fn_71007fe6b8(UnkAreaFXOwner_l7fe49c* owner);
void fn_710088c098(void* effect);
void fn_71007fe794(UnkAreaFXComponent_l7fe49c* self);
void fn_71007feb70(UnkAreaFXComponent_l7fe49c* self, CVector3D* centre);
UnkAreaRect_l7fe49c fn_71007fefc4(UnkAreaFXComponent_l7fe49c* self, const UnkAreaRect_l7fe49c* rect,
                                  CVector2D* offset);
void fn_71007ff358(UnkAreaFXComponent_l7fe49c* self, const UnkAreaRect_l7fe49c* rect);
void fn_71007fa0c4(UnkAreaFXComponent_l7fe49c* self, s32 mode);

static inline bool IsNear(f32 a, f32 b) {
    return fabsf(a - b) < 0.001f;
}

void fn_71007fe49c(UnkAreaFXComponent_l7fe49c* self, s32 mode) {
    if (void* effect = fn_71007fe6b8(self->owner))
        fn_710088c098(effect);
    else if (void* shape = self->owner->shape)
        fn_71009f19b0(shape, false);
    if (mode == 1)
        fn_71007fe794(self);
    CVector3D centre = self->owner->Position();
    if (self->use_centre) {
        void* movement = self->owner->movement;
        if (!movement || fn_71010cc3a4(movement) != 2)
            centre = self->centre;
        self->use_centre = false;
    } else {
        fn_71007feb70(self, &centre);
    }
    CAreaFXComponent_CollectOwnerColliders(reinterpret_cast<UnkAreaFXColl_ovb0081*>(self));
    CVector2D offset = {0.0f, 0.0f};
    CVector2D half = {self->def->size.x * 0.5f, self->def->size.y * 0.5f};
    UnkAreaRect_l7fe49c area;
    area.lo = {centre.x - half.x, centre.y - half.y};
    area.hi = {centre.x + half.x, centre.y + half.y};
    UnkAreaRect_l7fe49c bounds;
    bounds = fn_71007fefc4(self, &area, &offset);
    UnkAreaRect_l7fe49c fitted = bounds;
    UnkAreaRect_l7fe49c color = bounds;
    fn_71007ff210(reinterpret_cast<UnkFXComponentSelf*>(self), &color.lo.x);
    if (!(IsNear(offset.x, g_unk_7101647778.x) && IsNear(offset.y, g_unk_7101647778.y))) {
        UnkAreaFXOwner_l7fe49c* o = self->owner;
        o->moved = true;
        o->x += offset.x;
        o->y += offset.y;
    }
    fn_71007ff358(self, &fitted);
    self->centre = {(bounds.lo.x + bounds.hi.x) * 0.5f, (bounds.lo.y + bounds.hi.y) * 0.5f, 0.0f};
    fn_71007fa0c4(self, mode);
}
