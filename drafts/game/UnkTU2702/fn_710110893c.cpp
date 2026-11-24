// Unit 2702 (hint CAIComponent): a hit landed on the actor's target: plays the target's melee hit effect
// (kind 5) at the hit position, turned to the hit direction, and shows the owner's hit effect at that
// position in the owner's frame. Names and layouts are placeholders.
#include <math.h>
#include "base/UnkMatrix44.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CAutclastChargeAttack.h"
#include "game/CFXComponent.h"
#include "game/UnkActor.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2739.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::math::CVector3D;

struct UnkMelee;
const CFilePathStrId& fn_710092f204(UnkMelee* self, s32 kind);

struct UnkMatrix44Words {
    u32 m[16];

    UnkMatrix44Words() {}
    UnkMatrix44Words(const UnkMatrix44Words& other);
};

UnkMatrix44Words CopyMatrixWithoutTranslation(const UnkMatrix44Words* src);
void fn_710007839c(UnkMatrix44* m);

struct UnkSwitchObject;

struct UnkFxObjectVtable_l10893c {
    void* unk_0x0[2];
    void (*set_position)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x10
    void (*set_rotation)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x18
    void (*set_scale)(UnkFxObject_ovb0041* self, const UnkVec3f* v);     // 0x20
    void* unk_0x28[13];
    void (*set_loop)(UnkFxObject_ovb0041* self, bool loop);  // 0x90
    void (*set_speed)(UnkFxObject_ovb0041* self, f32 speed);  // 0x98
};
static inline UnkFxObjectVtable_l10893c* FxVtable(UnkFxObject_ovb0041* fx) {
    return *reinterpret_cast<UnkFxObjectVtable_l10893c**>(fx);
}

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

struct UnkHitTarget_l10893c {
    u8 pad_0x0[0x2f8];
    UnkMelee* melee;  // 0x2f8
};

struct UnkHitOwner_l10893c {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x208];
    void* fx_object;  // 0x280
};
static_assert(__builtin_offsetof(UnkHitOwner_l10893c, fx_object) == 0x280, "layout");

struct UnkHitAI_l10893c {
    u8 pad_0x0[0x30];
    UnkHitOwner_l10893c* owner;  // 0x30
};

struct UnkHitMessage_l10893c {
    u8 pad_0x0[0x14];
    UnkVec3f position;   // 0x14
    UnkVec3f direction;  // 0x20
};

void fn_710110893c(UnkHitAI_l10893c* self, void* sender, const UnkHitMessage_l10893c* hit) {
    UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
    if (!target)
        return;
    UnkMelee* melee = reinterpret_cast<UnkHitTarget_l10893c*>(target)->melee;
    if (!melee)
        return;

    f32 dx = hit->direction.x;
    f32 dy = hit->direction.y;
    f32 dz = hit->direction.z;
    f32 inv = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
    UnkVec3f position = hit->position;
    f32 nx = dx * inv;
    f32 ny = dy * inv;
    f32 angle = atan2f(ny, nx);

    UnkModelManager* manager = ModelManager();
    const CFilePathStrId& path = fn_710092f204(melee, 5);
    UnkVec3f at = position;
    UnkVec3f rotation(0.0f, -1.5707964f, angle);
    UnkVec3f scale(1.0f, 1.0f, 1.0f);
    UnkFxObject_ovb0041* fx = fn_71003b7db0(manager, &path, false, true);
    if (fx) {
        FxVtable(fx)->set_position(fx, &at);
        FxVtable(fx)->set_rotation(fx, &rotation);
        FxVtable(fx)->set_scale(fx, &scale);
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(fx), true);
        FxVtable(fx)->set_loop(fx, false);
        FxVtable(fx)->set_speed(fx, 1.0f);
        fn_71011a9bb4(manager, fx);
    }

    UnkHitOwner_l10893c* owner = self->owner;
    const UnkMatrix44& m = owner->transform;
    UnkVec3f local(position.x - m.m[3], position.y - m.m[7], position.z - m.m[11]);
    UnkMatrix44Words inverse =
        CopyMatrixWithoutTranslation(reinterpret_cast<const UnkMatrix44Words*>(&fn_710002614c(&owner->transform)));
    fn_710007839c(reinterpret_cast<UnkMatrix44*>(&inverse));
    UnkVec3f moved = TransformPointByMatrixWithPerspectiveDivide(reinterpret_cast<const UnkMat4f*>(&inverse), &local);
    local = moved;
    UnkVec3f offset = moved;
    UnkVec3f turn(0.0f, -1.5707964f, angle);
    fn_710115217c(self->owner->fx_object, fx, 0, 4, &g_unk_7101d470e0, reinterpret_cast<const CVector3D&>(offset),
                  reinterpret_cast<const CVector3D&>(turn), nullptr);
}
