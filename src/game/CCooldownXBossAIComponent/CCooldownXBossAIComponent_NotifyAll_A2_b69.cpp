// CCooldownXBossAIComponent's unit: hands each non-null object of the part at +0x149a0's list (+0xc40, count
// +0xc48) to fn_7100709f20 with the owner (+0x30), the key g_unk_7101649ba0 and 60, as the CHydrogigaAIComponent
// message handler does for one (src/game/CHydrogigaAIComponent/CHydrogigaAIComponent_Spawn_A3_b62.cpp).
// Names and layouts are placeholders.
#include "game/CTentacle.h"
#include "types.h"

struct UnkHydrogigaKey_b62a3;
struct UnkHydrogigaOwner_b62a3;
struct UnkHydrogigaTarget_b62a3;

extern UnkHydrogigaKey_b62a3 g_unk_7101649ba0 __attribute__((visibility("hidden")));

struct UnkCooldownXBossPart_A2_b69 {
    u8 pad_0x0[0xc40];
    UnkHydrogigaTarget_b62a3** targets;  // +0xc40
    u32 count;                           // +0xc48
};

struct UnkCooldownXBossAI_A2_b69 {
    u8 pad_0x0[0x30];
    UnkHydrogigaOwner_b62a3* owner;  // +0x30
    u8 pad_0x38[0x149a0 - 0x38];
    UnkCooldownXBossPart_A2_b69* part;  // +0x149a0
};

void fn_71004d499c(UnkCooldownXBossAI_A2_b69* self) {
    UnkCooldownXBossPart_A2_b69* part = self->part;
    u32 count = part->count;
    if (count == 0)
        return;
    UnkHydrogigaTarget_b62a3** it = part->targets;
    UnkHydrogigaTarget_b62a3** end = it + count;
    for (; it != end; ++it) {
        UnkHydrogigaTarget_b62a3* target = *it;
        if (target)
            fn_7100709f20(target, self->owner, &g_unk_7101649ba0, 60.0f);
    }
}
