// The CHydrogigaAttack unit's update: after the base update, when the owner's AI component (+0x240 of
// the owner at +0x8) is a CHydrogigaAIComponent (fn_71005ba098), the owner's animation (+0x220) is
// there and the AI is in state 2 (fn_71005c0f2c) or its timer at +0x149bc has run out, sends the
// sources id of the track record that a fn_7100c22518 query finds (with mode 1, descriptors as
// UnkSpitSend, src/game_s2/UnkSpitSend.h) and sets the flag at +0x50. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"


struct UnkHydrogigaAI_b57a3 {
    void** vtable;
    u8 pad_0x8[0x149bc - 0x8];
    f32 field_0x149bc;

    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkHydrogigaAI_b57a3*)>(vtable[0])(this);
    }
};

s32 fn_71005c0f2c(UnkHydrogigaAI_b57a3* self);

struct UnkHydrogigaAttack_b57a3 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x40];
    bool field_0x50;
};

// `dt` (s0) reaches the base update untouched in the original (tools/forwarded_args.py).
void fn_71005b2d14(UnkHydrogigaAttack_b57a3* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    auto* comp = static_cast<UnkHydrogigaAI_b57a3*>(self->owner->field_0x240);
    if (!comp)
        return;
    bool is = fn_710009d50c(comp->vf_0x0(), fn_71005ba098());
    UnkHydrogigaAI_b57a3* ai = is ? comp : nullptr;
    if (!is)
        return;
    auto* animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (!animation)
        return;
    if (fn_71005c0f2c(ai) != 2 && !(ai->field_0x149bc <= 0.0f))
        return;
    UnkTrackQuery query = {fn_7100c22518(), nullptr, -1.0f};
    fn_710117b08c(animation, &query);
    if (!query.result)
        return;
    u64 hash = *reinterpret_cast<u64*>(reinterpret_cast<char*>(query.result->sources.data) + 0x30);
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 1;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
    self->field_0x50 = true;
}
