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
#include "game/CComponent.h"
#include "game/CFXComponent.h"
#include "game/CGrabComponent.h"
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
struct UnkOwner;
struct UnkObject;
struct UnkComponent;
struct UnkPlayer;
struct UnkWorld;
struct UnkEntry;
struct UnkLifeComponent;
struct UnkSamusGunComponent;
struct UnkScriptCaller;
struct UnkSmLockon_A1_b64;
struct UnkSmAbility3_A2_b63;
struct UnkAbilityC_A2_b69;
struct UnkStruct_71002fda50;
struct UnkWaveChild_A2_b72;
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

struct UnkAbilitySet_ovb22;
bool fn_71002fdbc0(UnkAbilitySet_ovb22* self, bool);

struct UnkEnergyWaveObj_ovb22 {
    void** vtable;
    void vf_0x90() { reinterpret_cast<void (*)(UnkEnergyWaveObj_ovb22*)>(vtable[0x90 / 8])(this); }
};

struct UnkEnergyWaveCtl_ovb22 {
    u8 pad_0x0[0x30];
    UnkAbilityOwner_ovb22* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkWaveChild_A2_b72** children;  // 0x40
    u32 count;                       // 0x48
    u8 pad_0x4c[0x1c];
    UnkEnergyWaveObj_ovb22* object;  // 0x68
};

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
void fn_71002fd944(UnkEnergyWaveCtl_ovb22* self) {
    fn_71002fda50(reinterpret_cast<UnkStruct_71002fda50*>(self), true, true);
    for (UnkWaveChild_A2_b72** it = self->children; it != self->children + self->count; ++it)
        fn_71002f8848(*it);
    if (self->object) {
        self->object->vf_0x90();
        self->object = nullptr;
    }
    if (self->owner->field_0x260)
        fn_710113f270(self->owner->field_0x260, nullptr);
    if (self->owner->field_0x210)
        fn_710094f8f0(self->owner->field_0x210, reinterpret_cast<UnkEnergyWaveObject_b49*>(self->object));
    fn_71002fdbc0(reinterpret_cast<UnkAbilitySet_ovb22*>(self), false);
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
void fn_7100872908(void*, void*);

struct UnkEnergyWavePart_ovb22d {
    void** vtable;
    u32 ref_count;  // 0x8
    void vf_0x10() { reinterpret_cast<void (*)(UnkEnergyWavePart_ovb22d*)>(vtable[0x10 / 8])(this); }
};

struct UnkEnergyWaveLinkOwner_ovb22 {
    u8 pad_0x0[0x1d8];
    void* comp;  // 0x1d8
};

struct UnkEnergyWaveLinkTarget_ovb22 {
    u8 pad_0x0[0x30];
    UnkEnergyWaveLinkOwner_ovb22* owner;  // 0x30
};

// The controller as its constructor sees it: the parts array (+0x40) is written out flat.
struct UnkEnergyWaveCtor_ovb22 {
    void** vtable;
    u8 pad_0x8[0x38];
    void* field_0x40;
    u64 field_0x48;
    u32 field_0x50;
    bool field_0x54;
    u32 field_0x58;
    base::UnkAllocator* field_0x60;
    void* field_0x68;
    void* field_0x70;
    void** field_0x78;
    base::UnkNamedEntry field_0x80;
    u64 field_0xb0;
    void* field_0xb8;
    void* field_0xc0;
    bool field_0xc8;
    u32 field_0xcc;
    bool field_0xd0;
    bool field_0xd1;
    bool field_0xd2;
    bool field_0xd3;
    bool field_0xd4;
    f32 field_0xd8;
    u8 pad_0xdc[0xc];
    void* field_0xe8;
    u32 field_0xf0;
    CStrId field_0xf8;
    void* field_0x100;

    void vf_0x18() { reinterpret_cast<void (*)(UnkEnergyWaveCtor_ovb22*)>(vtable[0x18 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkEnergyWaveCtor_ovb22, field_0x100) == 0x100, "UnkEnergyWaveCtor_ovb22 layout");

// The controller as its destructor sees it.
struct UnkEnergyWaveDtor_ovb22 {
    void** vtable;
    u8 pad_0x8[0x38];
    base::UnkVector<UnkEnergyWavePart_ovb22d*> parts;  // 0x40
    void* field_0x68;
    void* field_0x70;
    void** field_0x78;
    base::global::CRntString field_0x80;
    u64 field_0xb0;
    void* field_0xb8;
    UnkEnergyWaveLinkTarget_ovb22* target;  // 0xc0
    u8 pad_0xc8[0x20];
    UnkEnergyWavePart_ovb22d* field_0xe8;
    u32 field_0xf0;
    CStrId field_0xf8;
};
static_assert(__builtin_offsetof(UnkEnergyWaveDtor_ovb22, field_0xf8) == 0xf8, "UnkEnergyWaveDtor_ovb22 layout");

void fn_7100302424(UnkEnergyWaveCtor_ovb22* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a22690 + 2;
    self->field_0x40 = nullptr;
    self->field_0x48 = 0;
    self->field_0x50 = 10;
    self->field_0x54 = true;
    self->field_0x58 = 0xf;
    self->field_0x60 = fn_7100083990();
    self->field_0x68 = nullptr;
    self->field_0x70 = nullptr;
    self->field_0x78 = g_unk_7101a22878 + 2;
    fn_710007e704(&self->field_0x80, "ABILITY_INPUT");
    self->field_0xb0 = 20;
    // An array of 20 0x1c-byte entries, after its element count.
    u64* block = static_cast<u64*>(fn_7100083ba0(0x238, 5));
    *block = 20;
    self->field_0xb8 = block + 1;
    self->field_0xc0 = nullptr;
    self->field_0xc8 = false;
    self->field_0xcc = 0;
    self->field_0xd0 = false;
    self->field_0xd1 = true;
    self->field_0xd2 = true;
    self->field_0xd3 = false;
    self->field_0xd4 = false;
    self->field_0x78 = g_unk_7101a22858 + 2;
    self->field_0xd8 = -1.0f;
    self->field_0xe8 = nullptr;
    self->field_0xf0 = 0;
    new (&self->field_0xf8) CStrId(g_unk_710162e2ed, true);
    self->field_0x100 = nullptr;
    self->vf_0x18();
}

void fn_71002fd4b0(UnkEnergyWaveDtor_ovb22* self) {
    self->vtable = g_unk_7101a22690 + 2;
    for (UnkEnergyWavePart_ovb22d **it = self->parts.data, **end = it + self->parts.count; it != end; ++it)
        if (*it)
            (*it)->vf_0x10();
    self->parts.count = 0;
    self->field_0xf8.Reset();
    if (UnkEnergyWavePart_ovb22d* part = self->field_0xe8) {
        if (--part->ref_count == 0)
            part->vf_0x10();
    }
    self->field_0x78 = g_unk_7101a22858 + 2;
    if (self->target && self->target->owner && self->target->owner->comp)
        fn_7100872908(self->target->owner->comp, &self->field_0x78);
    self->target = nullptr;
    self->field_0x78 = g_unk_7101a22878 + 2;
    if (self->field_0xb8) {
        fn_7100083c30(static_cast<u64*>(self->field_0xb8) - 1);
        self->field_0xb8 = nullptr;
    }
    fn_710007ecd4(&self->field_0x80);
    self->parts.Free();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
