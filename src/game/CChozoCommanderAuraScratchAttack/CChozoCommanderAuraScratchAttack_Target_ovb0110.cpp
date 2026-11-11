// CChozoCommanderAuraScratchAttack's unit (tools/tu_map.py unit 645): where the scratch lands. Names and
// layouts are placeholders.
#include <math.h>

#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::reflection::CClass;

struct UnkChozoWarriorAI_b66a2;

struct UnkScratchComponent_ov110 {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkScratchComponent_ov110*)>(vtable[0])(this); }
};

struct UnkScratchTunables_ov110 {
    u8 pad_0x0[0xf4];
    f32 field_0xf4;
};

struct UnkScratchAttack_ov110 {
    u8 pad_0x0[8];
    UnkActor* actor;  // 0x8
    u8 pad_0x10[0x40];
    UnkScratchTunables_ov110* tunables;  // 0x50
};

struct UnkScratchPos_ov110 {
    f32 x, y, z;
};

static inline u8 Flag_ov110(UnkScratchComponent_ov110* ai) {
    return reinterpret_cast<u8*>(ai)[0x15ed0];
}

// The point to scratch at: the actor's position with x moved to whichever of the commander's two
// bounds (fn_71010fc31c, fn_71010fc284) is nearer to its target.
bool fn_71004280e0(UnkScratchAttack_ov110* self, UnkScratchPos_ov110* out) {
    UnkScratchComponent_ov110* ai = static_cast<UnkScratchComponent_ov110*>(self->actor->field_0x240);
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_710044e708()))
        return false;
    if (Flag_ov110(ai))
        return false;
    if (!(self->tunables->field_0xf4 > 0.0f))
        return false;
    UnkActor* target = fn_7100d7c990(self->actor);
    if (!target)
        return false;
    UnkActor* actor = self->actor;
    f32 px = actor->field_0x44;
    f32 py = actor->field_0x54;
    f32 pz = actor->field_0x64;
    out->x = px;
    out->y = py;
    out->z = pz;
    f32 first = px;
    fn_71010fc31c(reinterpret_cast<UnkChozoWarriorAI_b66a2*>(ai), &first);
    f32 second = out->x;
    fn_71010fc284(reinterpret_cast<UnkChozoWarriorAI_b66a2*>(ai), &second);
    f32 x = target->field_0x44;
    out->x = fabsf(x - first) > fabsf(x - second) ? first : second;
    return true;
}
