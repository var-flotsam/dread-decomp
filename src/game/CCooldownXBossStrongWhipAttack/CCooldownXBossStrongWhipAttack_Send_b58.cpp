// CCooldownXBossStrongWhipAttack's unit (tools/tu_map.py): after the base update, while the timer at
// +0x70 is not negative it counts down (only when fn_7101161088 holds for the actor's +0x1c0 object);
// once it runs out (or straight away otherwise) the owner's (+0x8) sub-object at +0x220 is sent a zero
// (refl_EmitU32) and the timer is set to -1. Names are placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "game/CCooldownXBossAttack.h"
#include "game/CLifeComponent.h"
#include "game/UnkActor.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"


struct UnkWhipSend_b58;

struct UnkWhipSendOwner_b58 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkWhipSend_b58 {
    u8 pad_0x0[0x8];
    UnkWhipSendOwner_b58* owner;
    u8 pad_0x10[0x60];
    f32 timer;
};

void fn_71004ca7c4(UnkWhipSend_b58* self, f32 dt) {
    fn_71004c1968(self, dt);
    if (!(self->timer >= 0.0f))
        return;
    UnkActor* actor = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
    if (fn_7101161088(*reinterpret_cast<void**>(reinterpret_cast<char*>(actor) + 0x1c0))) {
        f32 t = self->timer - dt;
        self->timer = t;
        if (!(t <= 0.0f))
            return;
    }
    void* target = self->owner->field_0x220;
    if (!target)
        return;
    base::reflection::refl_EmitU32(target, 0xf589ba7b0e4e0272, 0xf4061ff25a2163d6, 0, &g_unk_710164b958,
                                   &g_unk_7172769740);
    self->timer = -1.0f;
}
