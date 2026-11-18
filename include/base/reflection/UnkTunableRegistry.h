#pragma once

// The global tunable registry (UnkTunableRegistry in base/reflection/UnkReflAttributes.h), created
// on first use by the CTunable* field functions and read by the hand-written tunable initialisers.

#include "base/global/CRntVector.h"
#include "base/reflection/UnkReflAttributes.h"
extern base::reflection::UnkTunableRegistry* g_unk_7101d47350;

namespace base::reflection {

// The global tunable registry, created (0x58 bytes) and constructed on first use: each CTunable* class
// registers its tunable object in it under the class's category.
inline UnkTunableRegistry* TunableRegistry() {
    UnkTunableRegistry* registry = g_unk_7101d47350;
    if (!registry) {
        registry = static_cast<UnkTunableRegistry*>(fn_71000839a0(0x58, 0x3f));
        fn_710013ac14(registry);
        g_unk_7101d47350 = registry;
    }
    return registry;
}

}  // namespace base::reflection
