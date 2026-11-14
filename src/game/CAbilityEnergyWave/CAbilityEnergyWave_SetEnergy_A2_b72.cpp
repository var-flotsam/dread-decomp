// CAbilityEnergyWave's unit (tools/tu_map.py): stores the energy at +0xd8; going from none to some gives the
// player the key g_unk_7101648d20 (fn_7100d7c828), any energy runs fn_7100301ac8; a pending refresh (+0xd2)
// is cleared and every child (+0x40, count +0x48) gets fn_71002f8848 and slot 0xa0 with true. Names and
// layouts are placeholders.
#include "game/CAbility.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineComponent.h"
#include "game/CAbilityLayout.h"
#include "types.h"

struct UnkPlayer;
extern u64 g_unk_7101648d20;

void fn_7100301ac8(CAbilityComponent* self);

void fn_7100301a08(CAbilityComponent* self, f32 energy) {
    if (energy > 0.0f && self->energy <= 0.0f) {
        if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310))
            fn_7100d7c828(reinterpret_cast<UnkActor*>(player), &g_unk_7101648d20, false);
    }
    // Keeps the second test (the original doesn't thread the first one's result through).
#if defined(__aarch64__)
    asm("" : "+w"(energy));
#endif
    self->energy = energy;
    if (energy > 0.0f)
        fn_7100301ac8(self);
    if (self->field_0xd2) {
        self->field_0xd2 = false;
        for (CAbility **p = self->begin(), **end = self->end(); p != end; ++p) {
            CAbility* child = *p;
            fn_71002f8848(child);
            child->Virtual(0xa0, true);
        }
    }
}
