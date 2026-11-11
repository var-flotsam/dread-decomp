#include "game/CAbilityGhostAura.h"
#include "game/CAbilitySpeedBooster.h"
#include "types.h"

struct UnkUnit435Self;

struct UnkUnit435Other {
    u8 unk_0x0[0xc8];
    u64* id;
};

void fn_710033e41c(UnkUnit435Self* self, UnkUnit435Other* other) {
    if (other) {
        u64* id = other->id;
        u64* current = (u64*)fn_7100308c74();
        if (__builtin_expect(*id == *current, 0))
            fn_710033cd50(self, 1);
    }
}
