// CCharClassShootActivatorComponent unit (tools/tu_map.py, the name from reflection): counts the
// activation (+0x44 of the actor's +0x30 entity), calls the scenario script's
// "OnShootActivationCompleted" with the entity's name (+0x10, "" when unset) and the flag, then
// notifies one of two deferred listener sets (g_unk_717272c9c0, the second at +0x80 when the flag is
// clear) with this component. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "game/UnkTU0.h"
#include "types.h"

extern char g_unk_717272c9c0[];

struct UnkClone_fn_710082d0ac;
using UnkShootListeners = base::UnkDeferredSet<base::UnkDeferredCallback<u64>>;
extern template void base::refl_DeferredInvoke<UnkClone_fn_710082d0ac, u64>(UnkShootListeners*, const u64*);

struct UnkShootEntity {
    u8 unk_0x0[0x10];
    const char* name;  // 0x10
    u8 unk_0x18[0x2c];
    union {
        u32 activations;  // 0x44 (no type-based alias info: the original reloads after the count)
    };
};

struct UnkShootActor {
    u8 unk_0x0[0x30];
    UnkShootEntity* entity;  // 0x30
};

struct UnkShootActivatorComponent {
    u8 unk_0x0[0x30];
    UnkShootActor* actor;  // 0x30
};

void fn_71008926ec(UnkShootActivatorComponent* self, bool flag) {
    UnkShootActor* actor = self->actor;
    actor->entity->activations++;
    const char* name = actor->entity->name;
    fn_7100000790("CurrentScenario.OnShootActivationCompleted", "sb", name ? name : "", flag);
    if (flag) {
        u64 arg = reinterpret_cast<u64>(self);
        base::refl_DeferredInvoke<UnkClone_fn_710082d0ac, u64>(
            reinterpret_cast<UnkShootListeners*>(g_unk_717272c9c0), &arg);
    } else {
        u64 arg = reinterpret_cast<u64>(self);
        base::refl_DeferredInvoke<UnkClone_fn_710082d0ac, u64>(
            reinterpret_cast<UnkShootListeners*>(g_unk_717272c9c0 + 0x80), &arg);
    }
}
