// CRockDiverAIComponent's unit (tools/tu_map.py): on reaching a surface, unless the owner's animation is
// already in its dive action, sends it the dive request; then aims along the surface normal (the signed
// angle between the owner's forward axis and the reversed normal, mapped from [-45, 45] degrees to [0, 1],
// into the property bag) and spawns the dive effect just off the hit point, turned to the normal.
// Names and layouts are placeholders.
#include <math.h>

#include "../../game_s2/UnkSpitSend.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "game/CAutclastChargeAttack.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2739.h"
#include "game/UnkXformNode.h"
#include "types.h"

// The two value keys the dive track fills: the aim and the dive ratio.
extern const unk_prop_bag::CStrId g_unk_71726fd810[2] __attribute__((visibility("hidden")));
extern const u8 g_unk_71726fd7e0[];  // the dive effect's path id

// The effect object the model manager hands out.
struct UnkFxObject_ovb0041;
struct UnkDiveFxVtable_l663780 {
    void* unk_0x0[2];
    void (*set_position)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x10
    void (*set_rotation)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x18
    void (*set_scale)(UnkFxObject_ovb0041* self, const UnkVec3f* v);     // 0x20
    void* unk_0x28[13];
    void (*set_loop)(UnkFxObject_ovb0041* self, bool loop);  // 0x90
    void (*set_speed)(UnkFxObject_ovb0041* self, f32 speed);  // 0x98
};

static inline UnkDiveFxVtable_l663780* Vtable_l663780(UnkFxObject_ovb0041* fx) {
    return *reinterpret_cast<UnkDiveFxVtable_l663780**>(fx);
}

static inline UnkModelManager* ModelManager_l663780() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

// Where the diver met the surface.
struct UnkRockDiverHit_l663780 {
    u8 pad_0x0[0x14];
    UnkVec3f point;  // 0x14
    u8 pad_0x20[0xc];
    UnkVec3f normal;  // 0x2c
};

struct UnkRockDiverAI_l663780 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x146e8 - 0x38];
    f32 aim;  // 0x146e8
};

void fn_7100663780(UnkRockDiverAI_l663780* self, UnkRockDiverHit_l663780* hit) {
    UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    // The dive request: mode 0 with the all-bits mask.
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    if (animation && !fn_710117ae38(animation, 0xb7b132b204e40961ull, false)) {
        mode.id = 0;
        mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(animation, 0xb7b132b204e40961ull, 0xf4061ff25a2163d6ull, &mode, &g_unk_710164b958,
                      &g_unk_7172769740, &mask);
    }

    f32 nx = -hit->normal.x;
    f32 ny = -hit->normal.y;
    f32 nz = -hit->normal.z;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    f32 fx = m.m[2];
    f32 fy = m.m[6];
    f32 fz = m.m[10];
    f32 dot = fx * nx + fy * ny + fz * nz;
    f32 cosine = dot / (sqrtf(fx * fx + fy * fy + fz * fz) * sqrtf(nx * nx + ny * ny + nz * nz));
    f32 angle;
    if (cosine >= 1.0f)
        angle = 0.0f;
    else if (cosine <= -1.0f)
        angle = 180.0f;
    else
        angle = acosf(cosine) * 57.29578f;

    const UnkMatrix44& side = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    f32 sx = side.m[1];
    f32 sy = side.m[5];
    f32 sz = side.m[9];
    f32 across = sx * nx + sy * ny + sz * nz;
    f32 aim = ((across < 0.0f ? -angle : angle) + 45.0f) / 90.0f;
    self->aim = aim;
    unk_prop_bag::fn_710033187c(reinterpret_cast<unk_prop_bag::UnkPropBag*>(animation->field_0x80),
                                g_unk_71726fd810[0], aim);

    f32 px = hit->point.x + hit->normal.x;
    f32 py = hit->point.y + hit->normal.y;
    f32 pz = hit->point.z + hit->normal.z;
    f32 yaw = atan2f(hit->normal.y, hit->normal.x) + 1.57f;
    UnkModelManager* manager = ModelManager_l663780();
    UnkVec3f position(px, py, pz);
    UnkVec3f angles(0.0f, 0.0f, yaw);
    UnkVec3f scale(1.0f, 1.0f, 1.0f);
    UnkFxObject_ovb0041* fx_object = fn_71003b7db0(manager, g_unk_71726fd7e0, false, true);
    if (fx_object) {
        Vtable_l663780(fx_object)->set_position(fx_object, &position);
        Vtable_l663780(fx_object)->set_rotation(fx_object, &angles);
        Vtable_l663780(fx_object)->set_scale(fx_object, &scale);
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(fx_object), true);
        Vtable_l663780(fx_object)->set_loop(fx_object, true);
        Vtable_l663780(fx_object)->set_speed(fx_object, -1.0f);
        fn_71011a9bb4(manager, fx_object);
    }
}
