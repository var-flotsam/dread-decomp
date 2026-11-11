// CChozoCommanderKiCounterAttack's unit (tools/tu_map.py): unless the flag at +0x58 is set, for an actor
// with objects at +0x208 and +0x2f8 (fn_710092b6f8 holding for the latter) and a character class (+0x220):
// when the owner (+0x8) faces the actor (the sign of its matrix element 2 is the sign of the x offset to
// the actor), the actor faces the other way and their x distance is within +0x100 of the tunables at
// +0x50, sends the actor's class a request (descriptors as UnkSpitSend) and sets the flags at +0x58 and
// +0x5b. Returns false. Names are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CMeleeComponent.h"
#include "game/UnkActor.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkKiCounterTunables_b59 {
    u8 pad_0x0[0x100];
    f32 field_0x100;
};

struct UnkKiCounter_b59 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x40];
    UnkKiCounterTunables_b59* field_0x50;
    bool field_0x58;
    u8 pad_0x59[2];
    bool field_0x5b;
};

static inline f32 Sign_b59(f32 x) {
    return (f32)((x > 0.0f) - (x < 0.0f));
}

static inline f32 FacingSign_b59(const u8* xform) {
    return Sign_b59(fn_710002614c(reinterpret_cast<const UnkMatrix44*>(xform)).m[2]);
}

bool fn_710042f570(UnkKiCounter_b59* self, UnkActor* actor) {
    if (self->field_0x58)
        return false;
    if (!actor->field_0x208)
        return false;
    void* object = *reinterpret_cast<void**>(reinterpret_cast<char*>(actor) + 0x2f8);
    if (!object || !fn_710092b6f8(object))
        return false;
    if (!actor->field_0x220)
        return false;
    const u8* actor_xform = actor->field_0x38;
    f32 x = self->owner->field_0x44;
    f32 actor_x = actor->field_0x44;
    f32 facing = FacingSign_b59(self->owner->field_0x38);
    UnkActor* owner = self->owner;
    f32 d = actor->field_0x44 - owner->field_0x44;
    const u8* owner_xform = owner->field_0x38;
    asm("" : "+r"(owner_xform));
    f32 side = Sign_b59(d);
    f32 facing2 = FacingSign_b59(owner_xform);
    f32 actor_facing = FacingSign_b59(actor_xform);
    if (facing != side || facing2 == actor_facing)
        return false;
    f32 distance = __builtin_fabsf(x - actor_x);
    if (!(self->field_0x50->field_0x100 >= distance))
        return false;
    UnkSpitSend(actor->field_0x220, 0x9df37a323f133ca6, 0xf4061ff25a2163d6);
    self->field_0x58 = true;
    self->field_0x5b = true;
    return false;
}
