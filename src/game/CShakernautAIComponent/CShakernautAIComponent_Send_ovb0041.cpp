// CShakernautAIComponent's unit (tools/tu_map.py unit 1047). fn_71006ae194: after the base step (fn_71010ff2b8)
// sends id 4 (mode at +0x14804 zero) or 8 to the owner's (+0x30) controller (+0x220) through fn_71011785a8.
// fn_71006b14ec: an impact record takes its point, the target's three values (+0x240, +0x10, +0x728..), and
// spawns its effect (g_unk_7172706450) at the point, turned half a turn when the transform's z axis points left.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "game/CAnimationComponent.h"
#include "game/CAutclastChargeAttack.h"
#include "game/UnkModelManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU2739.h"
#include "game/UnkTU285.h"
#include "game/UnkXformNode.h"
#include "types.h"

// As src/game/CShakernautAIComponent/CShakernautAIComponent_Flip_b53.cpp declares them.
struct UnkShakernautCtrl_b53;
struct UnkShakernautId_b53 {
    u32 value;
    u32 unk_0x4;
};
// As src/game/CPoisonFlyAIComponent/CPoisonFlyAIComponent_Invoke.cpp declares it.

struct UnkShkOwner_ovb0041 {
    u8 pad_0x0[0x220];
    UnkShakernautCtrl_b53* ctrl;  // 0x220
};

struct UnkShkAI_ovb0041 {
    u8 pad_0x0[0x30];
    UnkShkOwner_ovb0041* owner;  // 0x30
    u8 pad_0x38[0x14804 - 0x38];
    s32 mode;  // 0x14804
};

void fn_71006ae194(UnkShkAI_ovb0041* self) {
    fn_71010ff2b8(reinterpret_cast<char*>(self));
    UnkShakernautCtrl_b53* ctrl = self->owner->ctrl;
    if (self->mode == 0) {
        UnkShakernautId_b53 a;
        a.value = 4;
        fn_71011785a8(ctrl, &a, true, false);
    } else {
        UnkShakernautId_b53 b;
        b.value = 8;
        fn_71011785a8(ctrl, &b, true, false);
    }
}

using base::math::CVector2D;
using base::math::CVector3D;

// The effect object the model manager hands out, as its virtual calls see it (as in
// src/game/CMeleeComponent/CMeleeComponent_Checks_ovb0041.cpp).
struct UnkFxObject_ovb0041;
struct UnkFxObjectVtable_ovb0041 {
    void* unk_0x0[2];
    void (*set_position)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x10
    void (*set_rotation)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x18
    void (*set_scale)(UnkFxObject_ovb0041* self, const UnkVec3f* v);     // 0x20
    void* unk_0x28[13];
    void (*set_loop)(UnkFxObject_ovb0041* self, bool loop);               // 0x90
    void (*set_speed)(UnkFxObject_ovb0041* self, f32 speed);              // 0x98
};
struct UnkFxObject_ovb0041 {
    UnkFxObjectVtable_ovb0041* vtable;
};

extern const u8 g_unk_7172706450[] __attribute__((visibility("hidden")));  // the effect's path id

struct UnkShkTargetComps_ovb0041 {
    u8 pad_0x0[0x728];
    u32 field_0x728;
    u32 field_0x72c;
    u32 field_0x730;
};
struct UnkShkTargetAI_ovb0041 {
    u8 pad_0x0[0x10];
    UnkShkTargetComps_ovb0041* comps;  // 0x10
};
struct UnkShkTarget_ovb0041 {
    u8 pad_0x0[0x240];
    UnkShkTargetAI_ovb0041* ai;  // 0x240
};

struct UnkShkXformOwner_ovb0041 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
};
struct UnkShkXformRef_ovb0041 {
    u8 pad_0x0[0x10];
    UnkShkXformOwner_ovb0041* owner;  // 0x10
};

struct UnkShkImpact_ovb0041 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    f32 x;  // 0xc
    f32 y;  // 0x10
    u8 pad_0x14[0x4];
    UnkShkXformRef_ovb0041* xform;  // 0x18
};

struct UnkShkPoint_ovb0041 {
    f32 x, y;
};

void fn_71006b16b0(UnkShkImpact_ovb0041* self, UnkShkTarget_ovb0041* target);

static inline CVector3D ColumnZ(const UnkMatrix44& m) {
    CVector3D v;
    v.x = m.m[2];
    v.y = m.m[6];
    v.z = m.m[10];
    return v;
}

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

void fn_71006b14ec(UnkShkImpact_ovb0041* self, const UnkShkPoint_ovb0041* at, void* unused, UnkShkTarget_ovb0041* target) {
    self->x = at->x;
    self->y = at->y;
    fn_71006b16b0(self, target);
    UnkShkTargetComps_ovb0041* comps = target->ai->comps;
    self->field_0x0 = comps->field_0x728;
    self->field_0x8 = comps->field_0x72c;
    self->field_0x4 = comps->field_0x730;
    UnkShkXformRef_ovb0041* ref = self->xform;
    UnkShkXformOwner_ovb0041* owner = ref ? ref->owner : nullptr;
    const UnkMatrix44& m = fn_710002614c(&owner->xform);
    CVector2D flat;
    CVector3D axis = ColumnZ(m);
    fn_710007b430(&flat, &axis);
    f32 side = flat.x + flat.y * 0.0f;
    f32 yaw = side < 0.0f ? 3.1415927f : 0.0f;
    UnkModelManager* manager = ModelManager();
    UnkVec3f position;
    position.x = self->x;
    position.y = self->y;
    position.z = 0.0f;
    UnkVec3f rotation(0.0f, yaw, 0.0f);
    UnkVec3f scale(1.0f, 1.0f, 1.0f);
    UnkFxObject_ovb0041* fx = fn_71003b7db0(manager, g_unk_7172706450, false, true);
    if (fx) {
        fx->vtable->set_position(fx, &position);
        fx->vtable->set_rotation(fx, &rotation);
        fx->vtable->set_scale(fx, &scale);
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(fx), true);
        fx->vtable->set_loop(fx, true);
        fx->vtable->set_speed(fx, -1.0f);
        fn_71011a9bb4(manager, fx);
    }
    fn_71002093c8(reinterpret_cast<char*>(fx) + 0x28, false);
}
