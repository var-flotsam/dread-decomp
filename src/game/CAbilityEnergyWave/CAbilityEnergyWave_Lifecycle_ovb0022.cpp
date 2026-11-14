// CAbilityEnergyWave's unit (tools/tu_map.py): the wave controller's constructor, destructor and shutdown.
// Names and layouts are placeholders.
#include <new>
#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbility.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityLayout.h"
#include "game/CComponent.h"
#include "game/CFXComponent.h"
#include "game/CGrabComponent.h"
#include "game/CInputComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU1428.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflValue;

struct UnkSelf;
struct UnkObject;
struct UnkComponent;
struct UnkPlayer;
struct UnkWorld;
struct UnkLifeComponent;
struct UnkSamusGunComponent;
struct UnkScriptCaller;
struct UnkSmLockon_A1_b64;
struct UnkSmAbility3_A2_b63;
struct UnkAbilityC_A2_b69;
struct UnkStateObject;
struct UnkEnergyWaveCompA_b49;
struct UnkEnergyWaveCompB_b49;
struct UnkEnergyWaveObject_b49;
struct UnkEnergyWaveFxComp_b49;

extern char g_unk_7101648d28[];
extern const u64 g_unk_7101648d18;

struct UnkAbilityOwner_ovb22 {
    u8 pad_0x0[0x210];
    UnkEnergyWaveCompB_b49* field_0x210;
    u8 pad_0x218[0x48];
    UnkEnergyWaveCompA_b49* field_0x260;
};

bool fn_71002fdbc0(CAbilityComponent* self, bool);

struct UnkEnergyWavePlayerWorld_ovb22 {
    u8 pad_0x0[0x280];
    UnkWorld* world;
};

struct UnkEnergyWaveFxHolder_ovb22 {
    u8 pad_0x0[0x228];
    UnkEnergyWaveFxComp_b49* comp;
};

struct UnkEnergyWaveFxLink_ovb22 {
    u8 pad_0x0[0x10];
    UnkEnergyWaveFxHolder_ovb22* holder;
};

// Shuts the wave down: the items, the children, the selected object and the components, the default
// selection (fn_71002fdbc0), then the player's world and the two effect components.
void fn_71002fd944(CAbilityComponent* self) {
    fn_71002fda50(self, true, true);
    for (CAbility** it = self->begin(); it != self->end(); ++it)
        fn_71002f8848(*it);
    if (self->current) {
        self->current->Virtual(0x90);
        self->current = nullptr;
    }
    if (self->Owner<UnkAbilityOwner_ovb22>()->field_0x260)
        fn_710113f270(self->Owner<UnkAbilityOwner_ovb22>()->field_0x260, nullptr);
    if (self->Owner<UnkAbilityOwner_ovb22>()->field_0x210)
        fn_710094f8f0(self->Owner<UnkAbilityOwner_ovb22>()->field_0x210,
                      reinterpret_cast<UnkEnergyWaveObject_b49*>(self->current));
    fn_71002fdbc0(self, false);
    if (UnkWorld* world = reinterpret_cast<UnkEnergyWavePlayerWorld_ovb22*>(fn_7100dec788(g_unk_7101d47310))->world)
        fn_710115372c(world);
    if (UnkEnergyWaveFxComp_b49* comp =
            reinterpret_cast<UnkEnergyWaveFxHolder_ovb22*>(fn_7100defcd4(g_unk_7101d47310))->comp)
        fn_71011738f8(comp, g_unk_7101648d28, 0);
    if (UnkEnergyWaveFxLink_ovb22* link =
            *reinterpret_cast<UnkEnergyWaveFxLink_ovb22**>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x29a0)) {
        if (UnkEnergyWaveFxHolder_ovb22* holder = link->holder) {
            if (holder->comp)
                fn_71011738f8(holder->comp, g_unk_7101648d28, 0);
        }
    }
}

extern void* g_unk_7101a22690[];

struct UnkEnergyWaveLinkOwner_ovb22 {
    u8 pad_0x0[0x1d8];
    void* comp;  // 0x1d8
};



// The input listener's component, as the destructor reaches it: the input component (+0x1d8) of the target's
// actor.
static inline void* InputOf(UnkBaseComponent* component) {
    return OwnerOf<UnkEnergyWaveLinkOwner_ovb22>(component)->comp;
}

void fn_7100302424(CAbilityComponent* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a22690 + 2;
    new (&self->abilities) base::UnkVector<CAbility*>(0xf);
    self->current = nullptr;
    self->hold = {0, 0.0f};
    self->input.vtable = g_unk_7101a22878 + 2;
    fn_710007e704(&self->input.name, "ABILITY_INPUT");
    self->input.capacity = 20;
    // An array of 20 0x1c-byte entries, after its element count.
    u64* block = static_cast<u64*>(fn_7100083ba0(0x238, 5));
    *block = 20;
    self->input.entries = block + 1;
    self->target = nullptr;
    self->field_0xc8 = false;
    self->field_0xcc = 0.0f;
    self->field_0xd0 = false;
    self->field_0xd1 = true;
    self->field_0xd2 = true;
    self->field_0xd3 = false;
    self->field_0xd4 = false;
    self->input.vtable = g_unk_7101a22858 + 2;
    self->energy = -1.0f;
    self->life = nullptr;
    self->field_0xf0 = 0;
    new (&self->field_0xf8) CStrId(g_unk_710162e2ed, true);
    self->field_0x100 = nullptr;
    self->Virtual(0x18);
}

void fn_71002fd4b0(CAbilityComponent* self) {
    self->vtable = g_unk_7101a22690 + 2;
    for (CAbility **it = self->abilities.data, **end = it + self->abilities.count; it != end; ++it)
        if (*it)
            (*it)->Virtual(0x10);
    self->abilities.count = 0;
    self->field_0xf8.Reset();
    if (UnkWeakHandle* life = self->life) {
        if (--life->ref_count == 0)
            life->vf_0x10();
    }
    self->input.vtable = g_unk_7101a22858 + 2;
    if (self->target && self->target->field_0x30 && InputOf(self->target))
        fn_7100872908(InputOf(self->target), &self->input);
    self->target = nullptr;
    self->input.vtable = g_unk_7101a22878 + 2;
    if (self->input.entries) {
        fn_7100083c30(static_cast<u64*>(self->input.entries) - 1);
        self->input.entries = nullptr;
    }
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&self->input.name));
    self->abilities.Free();
    fn_71010c4194(self);
}
