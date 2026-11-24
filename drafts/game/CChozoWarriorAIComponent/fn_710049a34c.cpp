// CChozoWarriorAIComponent unit (tools/tu_map.py): a hit while the shield is up (+0x14999) shows the target's
// melee effect (kind 5) at the hit point, turned to the hit's normal, and attaches "Shield_SKN" to the owner's
// effects at that point in the owner's own frame; otherwise the base handles the hit (fn_710110893c). Names
// and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CAutclastChargeAttack.h"
#include "game/CFXComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2739.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkMelee;
struct UnkSwitchObject;
const CFilePathStrId& fn_710092f204(UnkMelee* self, s32 kind);

struct UnkMatrix44Words {
    u32 m[16];

    UnkMatrix44Words() {}
    UnkMatrix44Words(const UnkMatrix44Words& other);
};
UnkMatrix44Words CopyMatrixWithoutTranslation(const UnkMatrix44Words* src);
void fn_710007839c(UnkMat4f* m);

void fn_710007b8f0(char** self, const char* str);
void fn_710007b9d8(char** self);

extern char g_unk_71016499f0[];

struct UnkFxObject_ovb0041;
struct UnkFxObjectVtable_ovb0111 {
    void* unk_0x0[2];
    void (*set_position)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x10
    void (*set_rotation)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x18
    void (*set_scale)(UnkFxObject_ovb0041* self, const UnkVec3f* v);     // 0x20
    void* unk_0x28[13];
    void (*set_loop)(UnkFxObject_ovb0041* self, bool loop);  // 0x90
    void (*set_speed)(UnkFxObject_ovb0041* self, f32 speed);  // 0x98
};
static inline UnkFxObjectVtable_ovb0111* FxVtable(UnkFxObject_ovb0041* fx) {
    return *reinterpret_cast<UnkFxObjectVtable_ovb0111**>(fx);
}

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

// The actor the warrior acts against, as far as this function reads it.
struct UnkChozoWarriorTarget_l49a34c {
    u8 pad_0x0[0x2f8];
    UnkMelee* melee;  // 0x2f8
};

struct UnkChozoWarriorHit_l49a34c {
    u8 pad_0x0[0x14];
    UnkVec3f position;  // 0x14
    UnkVec3f normal;    // 0x20
};

struct UnkChozoWarriorAI_l49a34c {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x14999 - 0x38];
    bool shield_up;  // 0x14999
};

void fn_710110893c(UnkChozoWarriorAI_l49a34c* self, void* arg, UnkChozoWarriorHit_l49a34c* hit);

void fn_710049a34c(UnkChozoWarriorAI_l49a34c* self, void* arg, UnkChozoWarriorHit_l49a34c* hit) {
    if (!self->shield_up) {
        fn_710110893c(self, arg, hit);
        return;
    }
    UnkActor* target = fn_7100d7c990(self->owner);
    UnkMelee* melee = reinterpret_cast<UnkChozoWarriorTarget_l49a34c*>(target)->melee;
    if (!melee)
        return;

    f32 nx = hit->normal.x;
    f32 ny = hit->normal.y;
    f32 len = sqrtf(nx * nx + ny * ny + hit->normal.z * hit->normal.z);
    f32 inv = 1.0f / (len + 0.0001f);
    UnkVec3f point = hit->position;
    f32 angle = atan2f(ny * inv, nx * inv);

    UnkModelManager* manager = ModelManager();
    const CFilePathStrId& path = fn_710092f204(melee, 5);
    UnkVec3f position = point;
    UnkVec3f rotation(0.0f, -1.57079637f, angle);
    UnkVec3f scale(1.0f, 1.0f, 1.0f);
    UnkFxObject_ovb0041* fx = fn_71003b7db0(manager, &path, false, true);
    if (fx) {
        FxVtable(fx)->set_position(fx, &position);
        FxVtable(fx)->set_rotation(fx, &rotation);
        FxVtable(fx)->set_scale(fx, &scale);
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(fx), true);
        FxVtable(fx)->set_loop(fx, false);
        FxVtable(fx)->set_speed(fx, 1.0f);
        fn_71011a9bb4(manager, fx);
    }

    // The hit point in the owner's frame: relative to the shield joint when the owner has one.
    UnkActor* owner = self->owner;
    f32 origin_x = owner->field_0x44;
    f32 origin_y = owner->field_0x54;
    f32 origin_z = owner->field_0x64;
    UnkMatrix44 joint;
    for (u32 i = 0; i < 16; ++i)
        joint.m[i] = 0.0f;
    joint.m[15] = 1.0f;
    joint.m[10] = 1.0f;
    joint.m[5] = 1.0f;
    joint.m[0] = 1.0f;
    if (fn_71011735e8(static_cast<UnkEntity*>(owner->field_0x228), g_unk_71016499f0, &joint, 0)) {
        origin_x = joint.m[3];
        origin_z = joint.m[11];
        origin_y = joint.m[7];
    }
    UnkVec3f local(point.x - origin_x, point.y - origin_y, point.z - origin_z);
    UnkMatrix44Words frame = CopyMatrixWithoutTranslation(reinterpret_cast<const UnkMatrix44Words*>(&joint));
    fn_710007839c(reinterpret_cast<UnkMat4f*>(&frame));
    local = TransformPointByMatrixWithPerspectiveDivide(reinterpret_cast<const UnkMat4f*>(&frame), &local);

    void* effects = self->owner->field_0x280;
    char* name = nullptr;
    base::math::CVector3D offset;
    offset.x = local.x;
    offset.y = local.y;
    offset.z = local.z;
    base::math::CVector3D turn;
    turn.x = 0.0f;
    turn.y = -1.57079637f;
    turn.z = angle;
    fn_710007b8f0(&name, "Shield_SKN");
    fn_710115217c(effects, fx, 0, 6, &g_unk_7101d470e0, offset, turn, name);
    fn_710007b9d8(&name);
}
