// CGun (tools/tu_map.py unit 1302; the name from reflection): the distance to check behind the
// actor. When the actor's character class (+0x220) has a CCharClassGunCheckBackHangDistanceTrack,
// a ...BackAnalogDistanceTrack or a ...BackAnalogFallDistanceTrack (tested in that order), the
// matching distance (+0x30, +0x28, +0x2c) is returned with the flag set; otherwise the default
// (+0x24) with it clear. Names and layout are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkGun_b44a2 {
    u8 pad_0x0[0x24];
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
};

static inline UnkAnimationComponent* Tracks_b44a2(UnkActor* actor) {
    return static_cast<UnkAnimationComponent*>(actor->field_0x220);
}

void fn_71008350e0(UnkGun_b44a2* self, UnkActor* actor, f32* distance, bool* found) {
    const f32* d;
    bool f;
    if (actor && Tracks_b44a2(actor)) {
        if (fn_710117b348(Tracks_b44a2(actor), fn_7100d01588())) {
            d = &self->field_0x30;
            f = true;
            goto done;
        }
        if (Tracks_b44a2(actor)) {
            if (fn_710117b348(Tracks_b44a2(actor), fn_7100d00b68())) {
                d = &self->field_0x28;
                f = true;
                goto done;
            }
            if (Tracks_b44a2(actor) && fn_710117b348(Tracks_b44a2(actor), fn_7100d01078())) {
                d = &self->field_0x2c;
                f = true;
                goto done;
            }
        }
    }
    f = false;
    d = &self->field_0x24;
done:
    *distance = *d;
    *found = f;
}
