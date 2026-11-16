// The sub-area setup state's unit (tools/tu_map.py, unit 2611): hands every switched-on kind over
// between the setups two ids name, each both ways (fn_7100fad6b0). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// As src/game/UnkSubareaSetupState.cpp defines them.
struct UnkSubareaSetupState;
struct UnkSubareaSetup;
void fn_7100facbe8(UnkSubareaSetupState* self, u32 index, const UnkSubareaSetup* from,
                   const UnkSubareaSetup* to, bool enable);

// A setup as it is found by name: its id at +0x18.
struct UnkSetupEntry_lfad6b0 {
    u8 pad_0x0[0x18];
    CStrId id;
};

// The parts of the setup state read here: its setups by name and the switched-on kinds.
struct UnkSetupState_lfad6b0 {
    u8 pad_0x0[0x10];
    UnkSetupEntry_lfad6b0** setups;  // 0x10
    u32 setup_count;                 // 0x18
    u8 pad_0x1c[0x160];
    u32 enabled;  // 0x17c: kinds that are switched on
};
static_assert(__builtin_offsetof(UnkSetupState_lfad6b0, enabled) == 0x17c, "enabled");

// The first setup named `id`, or null; empty slots are skipped.
inline UnkSetupEntry_lfad6b0* FindSetup_lfad6b0(UnkSetupState_lfad6b0* self, const CStrId& id) {
    UnkSetupEntry_lfad6b0** it = self->setups;
    UnkSetupEntry_lfad6b0** end = it + self->setup_count;
    const base::global::UnkStringData* want = id.data;
    for (; it != end; ++it) {
        UnkSetupEntry_lfad6b0* entry = *it;
        if (entry && entry->id.data == want)
            return entry;
    }
    return nullptr;
}

void fn_7100fad6b0(UnkSubareaSetupState* state, const CStrId& from, const CStrId& to) {
    UnkSetupState_lfad6b0* self = reinterpret_cast<UnkSetupState_lfad6b0*>(state);
    if (!self->enabled)
        return;
    const UnkSubareaSetup* a = reinterpret_cast<const UnkSubareaSetup*>(FindSetup_lfad6b0(self, from));
    const UnkSubareaSetup* b = reinterpret_cast<const UnkSubareaSetup*>(FindSetup_lfad6b0(self, to));
    for (u32 kind = 0; kind < 9; ++kind) {
        if (self->enabled & (1u << kind)) {
            fn_7100facbe8(state, kind, a, b, true);
            fn_7100facbe8(state, kind, a, b, false);
        }
    }
}
