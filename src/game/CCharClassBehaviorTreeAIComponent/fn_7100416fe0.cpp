// Runs fn_7100413ef8 and, when it succeeds and the path id at 0x500 isn't the empty one,
// passes that id to the global registry (created on first use).
#include "base/UnkVector.h"
#include "game/UnkPathRegistry.h"
#include "types.h"
#include "base/global/CStrId.h"

struct UnkSelf {
    u8 unk_0x0[0x500];
    u64 field_0x500;
};

bool fn_7100413ef8(UnkSelf* self);
void fn_7100db3adc(UnkPathRegistry* registry, u64* id);

bool fn_7100416fe0(UnkSelf* self) {
    bool ok = fn_7100413ef8(self);
    if (ok && self->field_0x500 != (u64)&g_unk_7101d47200->empty) {
        UnkPathRegistry* registry = g_unk_71726b3240;
        if (!registry) {
            registry = static_cast<UnkPathRegistry*>(fn_71000839a0(0x70, 0x3f));
            fn_7100db37ec(registry);
            g_unk_71726b3240 = registry;
        }
        fn_7100db3adc(registry, &self->field_0x500);
    }
    return ok;
}
