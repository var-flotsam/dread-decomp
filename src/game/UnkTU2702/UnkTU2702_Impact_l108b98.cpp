// Unit 2702 (hint CAIComponent): a melee attack was blocked with an impact: starts the slow-motion
// effect with its default settings and sends the game manager the event "MELEE_BLOCKED_WITH_IMPACT" with
// a direction taken from the owner's transform. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

using base::math::CVector3D;

struct UnkImpactOwner_l108b98 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

struct UnkImpactAI_l108b98 {
    u8 pad_0x0[0x30];
    UnkImpactOwner_l108b98* owner;  // 0x30
};

void fn_7101108b98(UnkImpactAI_l108b98* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    {
        base::global::CRntString settings("22si");
        fn_7100df6d3c(manager, &settings, 0.15f, 0.1f);
    }
    manager = g_unk_7101d47310;
    CStrId id("MELEE_BLOCKED_WITH_IMPACT", true);
    const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
    f32 x = m.m[2], y = m.m[6], z = m.m[10];
    CVector3D dir = {x, y, z};
    fn_7100e038b0(manager, &id, reinterpret_cast<UnkVec*>(&dir), 0);
}
