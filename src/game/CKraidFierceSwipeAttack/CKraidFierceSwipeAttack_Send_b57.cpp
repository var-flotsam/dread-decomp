// The CKraidFierceSwipeAttack unit. fn_71005f42c8 runs fn_71005f201c; then, when the owner (+0x8) has its
// sub-object at +0x220, fn_71005f20e4 doesn't hold, the actor the owner acts against (fn_7100d7c990)
// has the flag at +0x3a8 of its +0x200 object set and the sub-object is in a track of the class of
// fn_7100c39158, the sub-object is sent 0xcb39371b86897f5c under the usual track kind with mode 2, its
// first descriptor a local record (-1, then the float 0.3 and its kind 1) in place of the global one
// (as src/game/CScorpiusAIComponent/CScorpiusAIComponent_Send_b56.cpp; otherwise UnkSpitSend,
// src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkKraidSwipeOwner_b57a4 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkKraidSwipe_b57a4 {
    u8 pad_0x0[0x8];
    UnkKraidSwipeOwner_b57a4* owner;
};

struct UnkKraidSwipeState_b57a4 {
    u8 pad_0x0[0x3a8];
    bool field_0x3a8;
};

struct UnkKraidSwipeValue_b57a4 {
    f32 value;
    u32 kind;
};

struct UnkKraidSwipeArg_b57a4 {
    s64 field_0x0;
    UnkKraidSwipeValue_b57a4 value;

    UnkKraidSwipeArg_b57a4(UnkKraidSwipeValue_b57a4 v) : field_0x0(-1), value(v) {}
};

static inline UnkKraidSwipeValue_b57a4 MakeFloat_b57a4(f32 value) {
    UnkKraidSwipeValue_b57a4 v;
    v.value = value;
    v.kind = 1;
    return v;
}

struct UnkSelf_kt41;
// The base attack update, typed as an opaque attack (one declaration for every caller).
struct UnkKraidAttack;
void fn_71005f201c(UnkKraidAttack* self);
bool fn_71005f20e4(UnkSelf_kt41*);

// `dt` (s0) reaches the base update untouched in the original (tools/forwarded_args.py).
void fn_71005f42c8(UnkKraidSwipe_b57a4* self, f32 dt) {
    reinterpret_cast<void (*)(UnkKraidAttack*, f32)>(fn_71005f201c)(reinterpret_cast<UnkKraidAttack*>(self), dt);
    UnkActor* actor = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
    if (!self->owner->field_0x220)
        return;
    bool blocked = fn_71005f20e4(reinterpret_cast<UnkSelf_kt41*>(self));
    if (!actor || blocked)
        return;
    UnkKraidSwipeState_b57a4* state = *reinterpret_cast<UnkKraidSwipeState_b57a4**>(reinterpret_cast<char*>(actor) + 0x200);
    if (!state || !state->field_0x3a8)
        return;
    if (!fn_710117b348(self->owner->field_0x220, fn_7100c39158()))
        return;
    void* target = self->owner->field_0x220;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 2;
    UnkKraidSwipeArg_b57a4 arg(MakeFloat_b57a4(0.3f));
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, 0xcb39371b86897f5c, 0xf4061ff25a2163d6, &mode, &arg, &g_unk_7172769740, &mask);
}

