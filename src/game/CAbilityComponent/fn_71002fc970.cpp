// CAbilityComponent's unit: the abilities' definitions, a table of seven built on first use (the class
// id, the class name, three tunables and the item that unlocks it), looked up by class id. Names and
// layouts are placeholders.
#include <new>
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

// An ability's definition (0x30 bytes).
struct UnkAbilityDef_l2fc970 {
    CStrId id;
    const char* name;  // 0x8
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    UnkQualifiedId item;  // 0x20: the item that unlocks the ability

};

// The abilities' class ids.
extern CStrId g_unk_71726aec58 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726aec60 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726aec68 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726aec70 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726aec78 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726aec80 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726aec88 __attribute__((visibility("hidden")));
extern u64 g_unk_71726aec90 __attribute__((visibility("hidden")));  // the table's guard
extern UnkAbilityDef_l2fc970 g_unk_71726aef08[7] __attribute__((visibility("hidden")));

void fn_71002fce70();  // destroys the table

static inline const UnkQualifiedId& ItemId_l2fc970(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// Builds a definition in place; the item's id is built empty (fn_71002ef5bc) or copied from the item.
static inline void InitDef(UnkAbilityDef_l2fc970* def, const CStrId& id, const char* name, f32 a, f32 b, f32 c) {
    new (&def->id) CStrId(id);
    def->field_0x18 = c;
    def->name = name;
    def->field_0x10 = a;
    def->field_0x14 = b;
    fn_71002ef5bc(&def->item);
}
static inline void InitDef(UnkAbilityDef_l2fc970* def, const CStrId& id, const char* name, f32 a, f32 b, f32 c,
                           UnkPropertyEx<>& item) {
    new (&def->id) CStrId(id);
    def->name = name;
    def->field_0x10 = a;
    def->field_0x14 = b;
    def->field_0x18 = c;
    new (&def->item) UnkQualifiedId(ItemId_l2fc970(item));
}

static inline UnkAbilityDef_l2fc970* AbilityDefs() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726aec90), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726aec90)) {
            UnkAbilityDef_l2fc970* defs = g_unk_71726aef08;
            InitDef(&defs[0], g_unk_71726aec58, "CAbilityEnergyWave", 1.0f, 0.75f, 0.0f);
            InitDef(&defs[1], g_unk_71726aec60, "CAbilityOpticCamouflage", 0.2f, 0.2f, 0.2f, g_unk_717275aa58);
            InitDef(&defs[2], g_unk_71726aec68, "CAbilitySpeedBooster", 0.2f, 0.2f, 0.2f, g_unk_717275aa98);
            InitDef(&defs[3], g_unk_71726aec70, "CAbilityGhostAura", 0.2f, 0.2f, 0.2f, g_unk_717275aad8);
            InitDef(&defs[4], g_unk_71726aec78, "CAbilityShinespark", 0.2f, 0.2f, 0.2f, g_unk_717275aa98);
            InitDef(&defs[5], g_unk_71726aec80, "CAbilitySonar", 0.2f, 0.2f, 0.2f, g_unk_717275ab58);
            InitDef(&defs[6], g_unk_71726aec88, "CAbilityMultiLockon", 0.2f, 0.2f, 0.2f, g_unk_717275ab98);
            fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_71002fce70), nullptr, g_unk_7101a19000);
            __cxa_guard_release(&g_unk_71726aec90);
        }
    }
    return g_unk_71726aef08;
}

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
const UnkAbilityDef_l2fc970* fn_71002fc970(const CStrId& id) {
    UnkAbilityDef_l2fc970* defs = AbilityDefs();
    for (s32 i = 0; i < 7; i++) {
        if (defs[i].id.data == id.data)
            return &defs[i];
    }
    return nullptr;
}
