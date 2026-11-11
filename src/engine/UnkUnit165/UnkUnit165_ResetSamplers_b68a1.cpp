// tools/tu_map.py unit 165 (no class hint): fn_71001505a4 resets the sampler slots of the device layer
// (src/engine/UnkGfxSamplerSlots.cpp) for each of the object's `count` (+0x40) slots: mode 1 for slot 0
// and 2 for the others through fn_7100160ab0 and fn_7100160990, value 1 through fn_7100160d50, all in
// group 1; then fn_710015a3e0(0). Names and layouts are placeholders.
#include "game/UnkTU177.h"
#include "game/UnkTU182.h"
#include "types.h"

// Signatures as their definitions in src/engine/UnkGfxSamplerSlots.cpp and the declarations in
// src/engine/UnkGraphicsSystem.cpp give them.

struct UnkSamplerOwner165_b68a1 {
    u8 pad_0x0[0x40];
    u32 count;  // 0x40
};

void fn_71001505a4(UnkSamplerOwner165_b68a1* self) {
    u32 n = self->count;
    for (u32 i = 0; i < n; ++i) {
        s32 mode = i != 0 ? 2 : 1;
        fn_7100160ab0(i, mode, 1);
        fn_7100160990(i, mode, true);
        fn_7100160d50(i, 1, 1);
    }
    fn_710015a3e0(0);
}
