// CChozoCommanderHyperDashAttack's unit (unit 649): whether the hyper dash may start. The base test
// (fn_710042bb38) must hold; with a CChozoCommanderAIComponent, a target and its state (+0x1f0), and the
// AI's flag at +0x15ed0 clear, the target's flag at +0xf9 must be clear, the AI's flag at +0x15ed1 set, and
// the AI's distance (fn_71010fc68c) less the horizontal gap to the target at least the tunable's +0x100.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkHypersparkAttack3_ovb0023;
struct UnkGoliathAI_b58a3;

bool fn_710042bb38(UnkHypersparkAttack3_ovb0023* self);

struct UnkCommanderAI_l42c1f0 {
    void** vtable;
    u8 pad_0x8[0x15ec8];
    bool field_0x15ed0;
    bool field_0x15ed1;

    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkCommanderAI_l42c1f0*)>(vtable[0])(this);
    }
};
static_assert(__builtin_offsetof(UnkCommanderAI_l42c1f0, field_0x15ed0) == 0x15ed0, "layout");

struct UnkTargetState_l42c1f0 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
};

struct UnkTarget_l42c1f0 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0x1a8];
    UnkTargetState_l42c1f0* state;  // 0x1f0
};
static_assert(__builtin_offsetof(UnkTarget_l42c1f0, state) == 0x1f0, "layout");

struct UnkHyperDashTunable_l42c1f0 {
    u8 pad_0x0[0x100];
    f32 field_0x100;
};

struct UnkHyperDashAttack_l42c1f0 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x40];
    UnkHyperDashTunable_l42c1f0* tunable;  // 0x50
};

bool fn_710042c1f0(UnkHyperDashAttack_l42c1f0* self, void* arg, u32 mode) {
    if (!fn_710042bb38(reinterpret_cast<UnkHypersparkAttack3_ovb0023*>(self)))
        return false;
    auto* ai = static_cast<UnkCommanderAI_l42c1f0*>(self->owner->field_0x240);
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_710044e708()))
        ai = nullptr;
    auto* target = reinterpret_cast<UnkTarget_l42c1f0*>(fn_7100d7c990(self->owner));
    UnkTargetState_l42c1f0* state = target ? target->state : nullptr;
    if (!ai || !state || ai->field_0x15ed0)
        return true;
    if (state->field_0xf9 || !ai->field_0x15ed1)
        return false;
    f32 reach;
    fn_71010fc68c(reinterpret_cast<UnkGoliathAI_b58a3*>(ai), &reach);
    return reach - __builtin_fabsf(target->x - self->owner->field_0x44) >= self->tunable->field_0x100;
}
