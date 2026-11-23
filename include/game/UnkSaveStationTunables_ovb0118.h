#pragma once

#include "types.h"

// The save station's tunables: the time to wait for the music at +0x24.
struct UnkSaveStationTunables_ovb0118 {
    u8 pad_0x0[0x24];
    f32 music_wait;  // 0x24
};
