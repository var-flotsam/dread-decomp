// CDaivoSpitAttack's unit (tools/tu_map.py): whether the spit attack may start. After the base check
// (fn_7100360b84) the target (fn_7100d7c990) must be on the side the actor faces: the sign of the
// x offset to it is within 0.1 of the sign of the actor's facing (its matrix's +0x8). Then the Daivo
// AI component (+0x240) has the last word (fn_71004df920). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "types.h"

using base::reflection::UnkReflObject;

bool fn_71004df920(UnkReflObject* ai);

static inline f32 Sign(f32 v) {
    return static_cast<f32>((0.0f < v) - (v < 0.0f));
}

bool fn_71004dc0d8(CAttack* self, void* arg, u32 mode) {
    bool ok = fn_7100360b84(self, arg, mode);
    UnkReflObject* ai = static_cast<UnkReflObject*>(self->field_0x8->field_0x240);
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_71004dec74()))
        ai = nullptr;
    UnkActor* target = fn_7100d7c990(self->field_0x8);
    if (ok && target) {
        UnkActor* actor = self->field_0x8;
        f32 x = target->field_0x44;
        const UnkMatrix44* xf = reinterpret_cast<const UnkMatrix44*>(actor->field_0x38);
        f32 side = Sign(x - actor->field_0x44);
        f32 facing = Sign(fn_710002614c(xf).m[2]);
        ok = side - 0.1f <= facing && side + 0.1f >= facing;
    }
    if (ai && ok)
        return fn_71004df920(ai);
    return ok;
}
