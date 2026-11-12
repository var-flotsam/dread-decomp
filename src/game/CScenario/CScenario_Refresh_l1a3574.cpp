// CScenario unit: for the layer `index` of a scenario, finds the entry stored under `*key`, walks its links
// from the last to the first, resolves each pending link to an actor and, for every live one, takes it off
// the scenario's lists and registers it with the game manager. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/reflection.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkScenarioKey_l1a3574 {
    u8 pad_0x0[0x30];
    u32 lo;  // 0x30
    u32 hi;  // 0x34
};

struct UnkScenarioActorName_l1a3574 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkScenarioActor_l1a3574 {
    u8 pad_0x0[0x30];
    UnkScenarioActorName_l1a3574* name;  // 0x30
    u8 pad_0x38[0x170];
    void* field_0x1a8;
    u8 pad_0x1b0[5];
    u8 field_0x1b5;
};
static_assert(__builtin_offsetof(UnkScenarioActor_l1a3574, field_0x1b5) == 0x1b5, "UnkScenarioActor layout");

struct UnkScenarioActorTarget_l1a3574 {
    u8 pad_0x0[0x10];
    UnkScenarioActor_l1a3574* actor;  // 0x10
};

struct UnkScenarioLink_l1a3574 {
    u8 pad_0x0[8];
    u8 member[0x28];  // 0x8
    s32 state;  // 0x30
    u8 pad_0x34[4];
    UnkScenarioActorTarget_l1a3574* target;  // 0x38
};

struct UnkScenarioEntry_l1a3574 {
    UnkScenarioKey_l1a3574* key;
    u8 links[8];  // 0x8
    u32 count;  // 0x10
    u8 pad_0x14[0x2c];
    UnkScenarioEntry_l1a3574* next;  // 0x40
};

struct UnkScenarioLayer_l1a3574 {
    u8 pad_0x0[0x20];
    u8 field_0x20[0x90];
    UnkScenarioEntry_l1a3574** buckets;  // 0xb0
    u32 modulus;  // 0xb8 (count)
    u32 divisor;  // 0xbc
    u8 pad_0xc0[8];
};
static_assert(sizeof(UnkScenarioLayer_l1a3574) == 0xc8, "UnkScenarioLayer layout");

struct UnkCaterzillaLink_ovb0054;
struct UnkCaterzillaLinks_ovb0054;
void fn_7100e02498(UnkGameManager* manager, void* actor);

u64 fn_71011a3574(UnkScenarioLayer_l1a3574* layers, u32 index, UnkScenarioKey_l1a3574* const* key) {
    fn_710025bda0(layers[index].field_0x20);
    if (layers[index].modulus == 0)
        return 1;
    UnkScenarioKey_l1a3574* k = *key;
    u32 hash = k->lo ^ k->hi;
    u32 slot = hash % layers[index].divisor;
    UnkScenarioEntry_l1a3574** bucket = &layers[index].buckets[slot];
    if (!bucket)
        return 1;
    UnkScenarioEntry_l1a3574* entry = *bucket;
    if (!entry)
        return 1;
    UnkGameManager* manager = g_unk_7101d47310;
    while (entry->key != k) {
        entry = entry->next;
        if (!entry)
            return 1;
    }
    for (s32 i = (s32)entry->count - 1; i != -1; --i) {
        UnkScenarioLink_l1a3574* link = reinterpret_cast<UnkScenarioLink_l1a3574*>(
            fn_7100247340(reinterpret_cast<UnkCaterzillaLinks_ovb0054*>(entry->links), i));
        if (link->state == 1) {
            {
                UnkRefPtr ref = fn_710023b580(link->member);
                fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&link->target),
                              reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            link->state = 2;
        }
        if (!link->target)
            continue;
        UnkScenarioActor_l1a3574* actor = link->target->actor;
        if (!actor || actor->field_0x1b5)
            continue;
        if (actor->field_0x1a8)
            fn_71011a2a24(reinterpret_cast<UnkGameManagerSub_0x608*>(layers), actor);
        u64 id = actor->name->id;
        alignas(8) u32 flags = 4;
        if (fn_7100deba8c(manager, &id, &flags))
            fn_7100e02498(manager, actor);
    }
    return 1;
}
