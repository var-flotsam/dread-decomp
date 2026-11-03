#include "types.h"

extern u32 g_unk_7101a50bb8;  // the number of slots the counter cycles through

struct CHydrogigaAIComponent {
    u8 unk_0x0[0x149d4];
    u32 counter;
};

// Returns the counter and advances it, wrapping to 0 at g_unk_7101a50bb8.
u32 fn_71005c11cc(CHydrogigaAIComponent* self) {
    u32 current = self->counter;
    u32 next = current + 1;
    self->counter = next >= g_unk_7101a50bb8 ? 0 : next;
    return current;
}

struct UnkHydrogigaTuning {
    u8 unk_0x0[0x554];
    f32 field_0x554;
};

struct UnkHydrogigaPhase {
    UnkHydrogigaTuning* tuning;
    u8 unk_0x8[0x20];
    f32 field_0x28;
};

struct UnkHydrogigaAIComponent2 {
    u8 unk_0x0[0x149a0];
    UnkHydrogigaPhase phase;
};

// Sets the phase value from the tuning when `enabled`, else to -1.
void fn_71005c023c(UnkHydrogigaAIComponent2* self, bool enabled) {
    UnkHydrogigaPhase* phase = &self->phase;
    phase->field_0x28 = enabled ? phase->tuning->field_0x554 : -1.0f;
}
