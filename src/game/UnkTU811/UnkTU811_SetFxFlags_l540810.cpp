// Unit 811 (CEmmyAIComponent hint): tells the object's own slot 0x860 the flag, then (unless it is the
// proto E.M.M.I. AI) sets six effect switches of the owner's effect component from the flag and the state
// at +0x27720 / +0x29ef0. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CModelUpdaterComponent.h"
#include "types.h"

extern u64 g_unk_7101649ef0;
__attribute__((visibility("hidden"))) extern u64 g_unk_7101649ef8;
__attribute__((visibility("hidden"))) extern u64 g_unk_7101649f00[2];
__attribute__((visibility("hidden"))) extern u64 g_unk_7101649f10[2];

struct UnkOwner_540810 {
    u8 pad_0x0[0x228];
    UnkEnergyWaveFxComp_b49* effects;  // 0x228
};

struct UnkEmmyActor_afc {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkOwner_540810* owner;  // 0x30
    u8 pad_0x38[0x276e8];
    u8 active;  // 0x27720
    u8 pad_0x27721[0x27cf];
    s32 state;  // 0x29ef0
};

void fn_7100540810(UnkEmmyActor_afc* self, bool flag) {
    u8 value = flag;
    typedef void (*SlotFn)(UnkEmmyActor_afc*, const u8*);
    reinterpret_cast<SlotFn>(self->vtable[0x860 / 8])(self, &value);
    if (self && fn_710009d50c(reinterpret_cast<base::reflection::UnkReflObject*>(self)->vf_0x0(), fn_71004fa488()))
        return;
    UnkEnergyWaveFxComp_b49* fx = self->owner->effects;
    if (!fx)
        return;
    fn_71011738f8(fx, &g_unk_7101649ef0, value);
    fn_71011738f8(fx, &g_unk_7101649ef8, !value);
    bool on = self->active ? true : self->state != 1;
    fn_71011738f8(fx, &g_unk_7101649f00[0], on & (value != 0));
    fn_71011738f8(fx, &g_unk_7101649f00[1], on & (value != 0));
    fn_71011738f8(fx, &g_unk_7101649f10[0], on & (value == 0));
    fn_71011738f8(fx, &g_unk_7101649f10[1], on & (value == 0));
}
