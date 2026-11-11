// CAbilityEnergyWave's unit (tools/tu_map.py): stores the energy at +0xd8; going from none to some gives the
// player the key g_unk_7101648d20 (fn_7100d7c828), any energy runs fn_7100301ac8; a pending refresh (+0xd2)
// is cleared and every child (+0x40, count +0x48) gets fn_71002f8848 and slot 0xa0 with true. Names and
// layouts are placeholders.
#include "game/CAbility.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
extern u64 g_unk_7101648d20;

struct UnkWaveChild_A2_b72 {
    void** vtable;
    void vf_0xa0(bool on) {
        reinterpret_cast<void (*)(UnkWaveChild_A2_b72*, bool)>(vtable[0xa0 / 8])(this, on);
    }
};

struct UnkEnergyWaveSet_A2_b72 {
    u8 pad_0x0[0x40];
    UnkWaveChild_A2_b72** children;
    u32 count;
    u8 pad_0x4c[0x86];
    bool field_0xd2;
    u8 pad_0xd3[0x5];
    f32 energy;  // 0xd8
};
void fn_7100301ac8(UnkEnergyWaveSet_A2_b72* self);

void fn_7100301a08(UnkEnergyWaveSet_A2_b72* self, f32 energy) {
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
        for (UnkWaveChild_A2_b72 **p = self->children, **end = p + self->count; p != end; ++p) {
            UnkWaveChild_A2_b72* child = *p;
            fn_71002f8848(child);
            child->vf_0xa0(true);
        }
    }
}
