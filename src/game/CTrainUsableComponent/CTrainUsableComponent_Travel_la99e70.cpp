// CTrainUsableComponent unit (tools/tu_map.py unit 1604): shows or hides the train's dialog; on show it
// picks the travel kind (+0x54c, when unset), tells the unit's listeners, stores true under the usable's
// property in the scenario's blackboard section, sets the loading screen's kind and starts the travel to
// the three names at +0x550..+0x560. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/CUsableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2557.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkClone_fn_710082d0ac;
using UnkTrainListeners = base::UnkDeferredSet<base::UnkDeferredCallback<u64>>;
extern template void base::refl_DeferredInvoke<UnkClone_fn_710082d0ac, u64>(UnkTrainListeners*, const u64*);

// The unit's listeners (built by its static initialiser).
struct UnkLinkGroups;

void fn_7100e10d78(UnkLoadingScreen* self, u32 kind);

struct UnkTrainValue_la99e70 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkTrainUsable_la99e70 {
    u8 pad_0x0[0x4d0];
    UnkTrainValue_la99e70* values;  // 0x4d0
    u64* keys;  // 0x4d8
    u32 count;  // 0x4e0
    u8 pad_0x4e4[0x64];
    u32 field_0x548;  // 0x548
    u32 kind;  // 0x54c
    CStrId scenario;  // 0x550
    CStrId field_0x558;  // 0x558
    CStrId field_0x560;  // 0x560
    bool field_0x568;  // 0x568
};

// Stores the bool at `value` under `name` in the blackboard section `section` names (unless the
// blackboard is locked).
static inline void SetSectionBool(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name, bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

// The loading screen, created on first use.
static inline UnkLoadingScreen* LoadingScreen() {
    UnkLoadingScreen* screen = g_unk_7101d47380;
    if (!screen) {
        screen = static_cast<UnkLoadingScreen*>(fn_71000839a0(0x220, 0x3f));
        fn_7100e0f89c(screen);
        g_unk_7101d47380 = screen;
    }
    return screen;
}

// The id's whole string, counting the read.
static inline const CRntString& UseName(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

void CTrainUsableComponent_SetLoadingScreenAndTravelToTarget(UnkTrainUsable_la99e70* self, bool show) {
    fn_7100aa2570(reinterpret_cast<UnkUsableDialog_A6_b62*>(self), show);
    if (!show)
        return;
    if (self->kind == 0) {
        if (self->field_0x548 == 0)
            self->kind = self->field_0x568 ? 8 : 7;
        else
            self->kind = self->field_0x568 ? 10 : 9;
    }
    {
        u64 arg = reinterpret_cast<u64>(self);
        base::refl_DeferredInvoke<UnkClone_fn_710082d0ac, u64>(
            reinterpret_cast<UnkTrainListeners*>(&g_unk_7172753260), &arg);
    }
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    {
        u64 index = 0;
        u32 count = self->count;
        if (count) {
            u64* keys = self->keys;
            u64 key = (u64)reinterpret_cast<UnkProperty*>(g_unk_7172753148)->name.data;
            for (; index != count; ++index) {
                if (keys[index] == key)
                    break;
            }
        }
        UnkTrainValue_la99e70* values = self->values;
        bool used = true;
        SetSectionBool(blackboard, section_id, &values[index].name, &used);
    }
    fn_7100e10d78(LoadingScreen(), self->kind);
    UnkGameManager* gm = g_unk_7101d47310;
    const CRntString& a = UseName(self->scenario);
    const CRntString& b = UseName(self->field_0x558);
    const CRntString& c = UseName(self->field_0x560);
    fn_7100df28d0(gm, a, b, c, CStrId(g_unk_710162e2ed, true), true, CRntString(g_unk_710162e2ed),
                  base::global::CStrId::Empty(), 0, nullptr, 0.0f);
}
