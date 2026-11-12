// CMorphBallMovement unit (tools/tu_map.py, the name from reflection): after fn_7100978f58, probes
// from the actor's position (fn_71010d15d0, with the radius at +0x60 of the actor's +0x228 shape and
// a cleared result); on a hit it runs virtual slot 0x318 when the flag at +0xf9 is set, then slot
// 0x380. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CCharacterMovement.h"
#include "game/CMovementComponent.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector3D;

struct UnkProbeResult {
    u64 field_0x0;
    u32 field_0x8;
};

// `arg` (x1) reaches fn_7100978f58 untouched in the original (tools/forwarded_args.py).
void fn_710094014c(char* self, const void* arg) {
    fn_7100978f58(self, arg);
    UnkProbeResult result;
    result.field_0x0 = 0;
    result.field_0x8 = 0;
    UnkActor* actor = *reinterpret_cast<UnkActor**>(self + 0x30);
    f32 x = actor->field_0x44, y = actor->field_0x54, z = actor->field_0x64;
    CVector3D pos = {x, y, z};
    f32 radius = *reinterpret_cast<f32*>(static_cast<char*>(actor->field_0x228) + 0x60);
    if (fn_71010d15d0(self, &pos, &result, radius)) {
        using Fn = void (*)(char*);
        if (*reinterpret_cast<bool*>(self + 0xf9))
            (*reinterpret_cast<Fn**>(self))[0x318 / 8](self);
        (*reinterpret_cast<Fn**>(self))[0x380 / 8](self);
    }
}
