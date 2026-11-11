// CChozoCommanderKiGrabAttack's unit (tools/tu_map.py): adds the time step to the timer at +0x140; when the
// owner's (+0x8) AI component (+0x240) is a CChozoCommanderAIComponent and the actor the owner acts against
// has a character class (+0x220): picks one of the component's two heights at +0x15d1c/+0x15d24 by the
// sign of the owner's matrix element 2 and, when the actor's x is within half its reach (fn_7100977b90 of
// its +0x1f8) plus 5 of it, sends the class a request with mode 1 (descriptors as UnkSpitSend). Names are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharacterMovement.h"
#include "game/UnkActor.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

// As src/game/CAIComponent/CAIComponent_ReachRadius.cpp has it.
struct UnkAIReach {
    f32 radius;
    f32 field_0x4;

    UnkAIReach() : radius(0.0f), field_0x4(0.0f) {}
    UnkAIReach(const UnkAIReach& other);
};

struct UnkKiGrabCommander_b59 {
    u8 pad_0x0[0x15d1c];
    f32 field_0x15d1c[3];
};

struct UnkKiGrab_b59 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x130];
    f32 timer;
};

void fn_71004331d0(UnkKiGrab_b59* self, f32 dt) {
    self->timer += dt;
    auto* component = static_cast<base::reflection::UnkReflObject*>(self->owner->field_0x240);
    auto* commander = reinterpret_cast<UnkKiGrabCommander_b59*>(
        component && fn_710009d50c(component->vf_0x0(), fn_710044e708()) ? component : nullptr);
    UnkActor* actor = fn_7100d7c990(self->owner);
    if (!actor)
        return;
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!commander)
        return;
    void* anim = target->field_0x220;
    if (!anim)
        return;
    f32* heights = commander->field_0x15d1c;
    UnkAIReach reach = fn_7100977b90(actor->field_0x1f8);
    f32 limit = reach.radius * 0.5f + 5.0f;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    f32* height = m.m[2] > 0.0f ? &heights[2] : heights;
    if (!(__builtin_fabsf(*height - actor->field_0x44) <= limit))
        return;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 1;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(anim, 0x65447d710ba06d77, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}
