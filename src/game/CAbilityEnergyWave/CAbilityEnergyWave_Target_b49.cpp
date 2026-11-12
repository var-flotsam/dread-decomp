// CAbilityEnergyWave (tools/tu_map.py): fn_71002fe160 retargets the wave: the old target (+0xc0) drops
// the wave's link (+0x78) from its component (+0x30, +0x1d8; fn_7100872908), the new one (+0x1d8 of the
// argument) takes it (fn_7100872860), the children (+0x40, count +0x48) get slot 0x68 with the new
// target, and the owner's world (+0x30, +0x280) has the object with hash 0xb1489dc93c1d38dd reset
// (fn_710114ffb8). fn_71002fe24c runs fn_71002fe318, gives every child slot 0x70 with the argument, then
// resets the player's world (+0x280; fn_710115372c) and calls fn_71011738f8 with g_unk_7101648d28 on
// two components (+0x228 of fn_7100defcd4's object, +0x228 of the manager's +0x29a0 +0x10). Names and
// layouts are placeholders.
#include "game/CAbilityEnergyWave.h"
#include "game/CFXComponent.h"
#include "game/CInputComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkWorld;
struct UnkPart;
struct UnkParent;
struct UnkPlayer;
struct UnkOwner;
struct UnkRinkaControl_ruc;

UnkPart* fn_7101152b90(UnkWorld* world, u64 hash);
void fn_7100872908(void*, void*);

struct UnkEnergyWaveTargetComp_b49;
struct UnkEnergyWaveFxComp_b49;
extern char g_unk_7101648d28[];

struct UnkEnergyWaveTargetOwner_b49 {
    u8 pad_0x0[0x1d8];
    UnkEnergyWaveTargetComp_b49* comp;
};

struct UnkEnergyWaveTarget_b49 {
    u8 pad_0x0[0x30];
    UnkEnergyWaveTargetOwner_b49* owner;
};

struct UnkEnergyWaveArg_b49 {
    u8 pad_0x0[0x1d8];
    UnkEnergyWaveTarget_b49* target;
};

struct UnkEnergyWaveChild2_b49 {
    void** vtable;
    void vf_0x68(void* a) { reinterpret_cast<void (*)(UnkEnergyWaveChild2_b49*, void*)>(vtable[0x68 / 8])(this, a); }
    void vf_0x70(void* a) { reinterpret_cast<void (*)(UnkEnergyWaveChild2_b49*, void*)>(vtable[0x70 / 8])(this, a); }
};

struct UnkEnergyWaveOwner2_b49 {
    u8 pad_0x0[0x280];
    UnkWorld* world;
};

struct UnkEnergyWave2_b49 {
    u8 pad_0x0[0x30];
    UnkEnergyWaveOwner2_b49* owner;
    u8 pad_0x38[0x8];
    UnkEnergyWaveChild2_b49** children;
    u32 count;
    u8 pad_0x4c[0x2c];
    u8 field_0x78[0x48];
    UnkEnergyWaveTarget_b49* target;
};

struct UnkEnergyWavePlayer_b49 {
    u8 pad_0x0[0x280];
    UnkWorld* world;
};

struct UnkEnergyWaveFxHolder_b49 {
    u8 pad_0x0[0x228];
    UnkEnergyWaveFxComp_b49* comp;
};
struct UnkEnergyWaveFxLink_b49 {
    u8 pad_0x0[0x10];
    UnkEnergyWaveFxHolder_b49* holder;
};

void fn_71002fe160(UnkEnergyWave2_b49* self, UnkEnergyWaveArg_b49* arg) {
    void* link = self->field_0x78;
    if (self->target && self->target->owner && self->target->owner->comp)
        fn_7100872908(self->target->owner->comp, link);
    self->target = nullptr;
    if (UnkEnergyWaveTarget_b49* target = arg->target) {
        if (target->owner && target->owner->comp)
            fn_7100872860(target->owner->comp, link);
        self->target = target;
    }
    for (UnkEnergyWaveChild2_b49 **it = self->children, **end = it + self->count; it != end; ++it)
        (*it)->vf_0x68(arg);
    if (UnkWorld* world = self->owner->world) {
        if (fn_7101152b90(world, 0xb1489dc93c1d38ddull))
            fn_710114ffb8(reinterpret_cast<UnkRinkaControl_ruc*>(world), (void*)0xb1489dc93c1d38ddull, 0, 0);
    }
}

void fn_71002fe24c(UnkEnergyWave2_b49* self, void* arg) {
    fn_71002fe318(reinterpret_cast<UnkParent*>(self));
    for (UnkEnergyWaveChild2_b49 **it = self->children, **end = it + self->count; it != end; ++it)
        (*it)->vf_0x70(arg);
    if (UnkWorld* world = reinterpret_cast<UnkEnergyWavePlayer_b49*>(fn_7100dec788(g_unk_7101d47310))->world)
        fn_710115372c(world);
    if (UnkEnergyWaveFxComp_b49* comp = reinterpret_cast<UnkEnergyWaveFxHolder_b49*>(fn_7100defcd4(g_unk_7101d47310))->comp)
        fn_71011738f8(comp, g_unk_7101648d28, 0);
    if (UnkEnergyWaveFxLink_b49* link = *reinterpret_cast<UnkEnergyWaveFxLink_b49**>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x29a0)) {
        if (UnkEnergyWaveFxHolder_b49* holder = link->holder) {
            if (holder->comp)
                fn_71011738f8(holder->comp, g_unk_7101648d28, 0);
        }
    }
}
