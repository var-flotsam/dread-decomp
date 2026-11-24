// CAIManager::CTunableAIManager's unit: initialises the manager's parts (each reports whether it
// succeeded) and fills its slot arrays: for each of the three kinds, 64 models of each of the two paths
// of that kind are created, put on layer 0 and given back as free slots (fn_7100da8f00). Names and
// layouts are placeholders.
#include <new>

#include "base/UnkStaticStrId.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkModelManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU1110.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkPoisonFlyReset_b66a4;
struct UnkVulkranRefList_b66a4;
struct UnkUnit1055Object_b66a4;
struct UnkU1038Self_A4_b71;
bool fn_7100639ec0(UnkPoisonFlyReset_b66a4* self);
bool CVulkranAttackManager_Clear(UnkVulkranRefList_b66a4* self);
bool fn_71006bd074(UnkUnit1055Object_b66a4* self);
bool fn_71006a1e04(UnkU1038Self_A4_b71* self);

struct UnkSonarModel_ovb0085;
void* fn_7100339900(UnkRenderPool* pool, const CFilePathStrId* path, UnkSonarModel_ovb0085** out,
                    const CStrId* name);

struct UnkSwitchObject;

// The slots of src/game/CAIManager/CTunableAIManager/CTunableAIManager_Lists_ovb0025.cpp
// (UnkSlot_da8a10), with the values a new slot starts with.
struct UnkSlot_da8a10 {
    UnkSwitchObject* object = nullptr;
    bool free = true;
    bool field_0x9 = false;
    u16 field_0xa = 0;
    u32 field_0xc = 0;
} __attribute__((packed, aligned(4)));

struct UnkSlotArray_da8a10 {
    u32 count;
    UnkSlot_da8a10 slots[64];
};
static_assert(sizeof(UnkSlotArray_da8a10) == 0x404, "UnkSlotArray_da8a10 layout");

struct UnkSlotOwner_b0025;
void fn_7100da8f00(UnkSlotOwner_b0025* self, UnkSwitchObject* object);

// The paths of the models of each kind, two per kind.
extern base::UnkStaticPathId g_unk_71727bc5e0[3] __attribute__((visibility("hidden")));
extern base::UnkStaticPathId g_unk_71727bc5f8[3] __attribute__((visibility("hidden")));

struct UnkAIManagerParts_da8a10 {
    u8 unk_0x0[0x80];
    u8 field_0x80[0x368 - 0x80];
    u8 field_0x368[0x3f0 - 0x368];
    u8 field_0x3f0[0x470 - 0x3f0];
    u8 field_0x470[0x5e0 - 0x470];
    u8 field_0x5e0[0x928 - 0x5e0];
    u8 field_0x928[0xbf8 - 0x928];
    UnkSlotArray_da8a10 groups[2][3];
};
static_assert(__builtin_offsetof(UnkAIManagerParts_da8a10, groups[1][0]) == 0x1804, "layout");

static inline void ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
}

// The slot at `i`, the array growing with free slots up to it.
static inline UnkSlot_da8a10& At(UnkSlotArray_da8a10& array, u64 i) {
    if (i >= array.count) {
        for (u64 n = array.count; n <= i; ++n)
            array.slots[n] = UnkSlot_da8a10();
        array.count = i + 1;
    }
    return array.slots[i];
}

bool fn_7100da8a10(UnkAIManagerParts_da8a10* self) {
    bool poison_fly = fn_7100639ec0(reinterpret_cast<UnkPoisonFlyReset_b66a4*>(self->field_0x80));
    bool vulkran = CVulkranAttackManager_Clear(reinterpret_cast<UnkVulkranRefList_b66a4*>(self->field_0x368));
    bool unit1055_a = fn_71006bd074(reinterpret_cast<UnkUnit1055Object_b66a4*>(self->field_0x3f0));
    bool unit1055_b = fn_71006bd074(reinterpret_cast<UnkUnit1055Object_b66a4*>(self->field_0x470));
    bool unit1038 = fn_71006a1e04(reinterpret_cast<UnkU1038Self_A4_b71*>(self->field_0x5e0));
    bool other = fn_71006f345c(self->field_0x928);
    for (u64 kind = 0; kind < 3; ++kind) {
        for (u64 i = 0; i < 64; ++i) {
            for (u32 side = 0; side < 2; ++side) {
                UnkSlotArray_da8a10& array = self->groups[side][kind];
                const base::UnkStaticPathId& path = side == 0 ? g_unk_71727bc5e0[kind] : g_unk_71727bc5f8[kind];
                new (&array.slots[array.count]) UnkSlot_da8a10;
                ++array.count;
                ModelManager();
                UnkRenderManager* render = g_unk_7101d47300;
                UnkSonarModel_ovb0085* model = nullptr;
                fn_7100339900(render->field_0x218, reinterpret_cast<const CFilePathStrId*>(&path), &model,
                              &g_unk_7101d470e0);
                auto* overlay = reinterpret_cast<UnkOverlayModel*>(model);
                fn_7100219ae0(render, overlay ? &overlay->node : nullptr, 0);
                At(array, i).object = reinterpret_cast<UnkSwitchObject*>(model);
                fn_7100da8f00(reinterpret_cast<UnkSlotOwner_b0025*>(self), At(array, i).object);
            }
        }
    }
    return poison_fly & vulkran & unit1055_a & unit1055_b & unit1038 & other;
}
