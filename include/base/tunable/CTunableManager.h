#pragma once

// base::tunable::CTunableManager, the global every CTunable* class registers its tunable object with (the
// class's reflection names it, and its one reflected member, hashTunables). It is created on first use: 0x58
// bytes, its reflected size, from the allocation function with flags 0x3f, then constructed by
// CTunableManager_Construct (data/names.csv), which every user calls as a plain function.
//
// The constructor builds the two dictionaries (the first with allocation flags 0x3e, the second with 5 and room
// for ten groups) and leaves the word between them unset. What the second dictionary holds comes from the
// functions that grow and search it (src/base/UnkTunableRegistry.cpp); nothing gives it or the word a name.

#include <cstddef>

#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkTunableGroup;  // 0x80 bytes: a named group's members (src/base/UnkTunableRegistry.cpp)

namespace base::tunable {

class CTunable;

struct CTunableManager {
    // The registered tunables by "<category>|<short class name>".
    global::CRntSmallDictionary<global::CRntString, CTunable*> hashTunables;
    u64 field_0x28;  // unset by the constructor
    // The groups of tunables by name.
    global::CRntSmallDictionary<global::CStrId, UnkTunableGroup> field_0x30;
};
static_assert(sizeof(CTunableManager) == 0x58, "base::tunable::CTunableManager size");
static_assert(offsetof(CTunableManager, field_0x30) == 0x30, "base::tunable::CTunableManager::field_0x30");

}  // namespace base::tunable

void CTunableManager_Construct(base::tunable::CTunableManager* self);
// The one manager, null until something needs it.
extern base::tunable::CTunableManager* g_unk_7101d47350;

namespace base::tunable {

// The global manager, created (0x58 bytes) and constructed on first use.
inline CTunableManager* TunableManager() {
    CTunableManager* manager = g_unk_7101d47350;
    if (!manager) {
        manager = static_cast<CTunableManager*>(fn_71000839a0(0x58, 0x3f));
        CTunableManager_Construct(manager);
        g_unk_7101d47350 = manager;
    }
    return manager;
}

}  // namespace base::tunable
