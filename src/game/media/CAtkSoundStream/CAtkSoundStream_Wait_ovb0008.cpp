// media::CAtkSoundStream's unit (tools/tu_map.py, unit 318): fn_710021b2ac, the entry of the
// "AtkSoundStream Thread" (started by fn_710021b09c), waits until the stream's state at +0x1e4,
// read with acquire ordering, reaches 3, sleeping 10 between reads (fn_71000810b0). Names and
// layouts are placeholders.
#include "game/UnkTU58.h"
#include "types.h"

struct UnkAtkSoundStream_ovb0008 {
    u8 pad_0x0[0x1e4];
    s32 state;  // 0x1e4
};

void fn_710021b2ac(UnkAtkSoundStream_ovb0008* self) {
    while (__atomic_load_n(&self->state, __ATOMIC_ACQUIRE) != 3)
        // Declared with a 32-bit argument (game/UnkTU58.h); this caller passes the value in a full 64-bit register.
        reinterpret_cast<void* (*)(u64)>(fn_71000810b0)(10);
}
