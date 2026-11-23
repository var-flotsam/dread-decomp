// CChozoCommanderZeroLaserGroundedAttack: asks the target's aim action for the "DC_Shot" angle at the
// position held in the attack, converts it to radians and, in state 5, mirrors it into [-pi/2, pi/2].
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using UnkAimAction = UnkTrackAction;

struct UnkPosition {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

struct UnkZeroLaserAttack {
    u8 pad_0x0[8];
    UnkActor* field_0x8;
    u8 pad_0x10[0x48];
    s32 field_0x58;
    u8 pad_0x5c[8];
    UnkPosition field_0x64;
};

bool fn_7100b8fa7c(UnkAimAction* action, f32* angle, const UnkMatrix44* matrix, u64* hash, UnkPosition* position,
                   bool flag, bool flag2);

f32 fn_71004482d0(UnkZeroLaserAttack* self, bool flag) {
    f32 result = 0.0f;
    if (self->field_0x8->field_0x220) {
        UnkAimAction* action = fn_710070d500(fn_71011771e0(self->field_0x8->field_0x220, 0xf4061ff25a2163d6));
        if (action && fn_710009d50c(action->vf_0x18(), fn_7100b8f4e8())) {
            f32 angle = 0.0f;
            const UnkMatrix44* matrix =
                &fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->field_0x8->field_0x38));
            u64 hash = StringCrc64("DC_Shot", 7);
            if (fn_7100b8fa7c(action, &angle, matrix, &hash, &self->field_0x64, flag, false)) {
                result = (f32)((f64)angle * 3.141592653589793 / 180.0);
                if (self->field_0x58 == 5) {
                    // Mirror the angle into [-pi/2, pi/2].
                    if (result > 1.5707964f)
                        result = 3.1415927f - result;
                    else if (result < -1.5707964f)
                        result = -3.1415927f - result;
                }
            }
        }
    }
    return result;
}
