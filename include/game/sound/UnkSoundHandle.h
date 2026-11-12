#pragma once

// A handle on a playing sound: the sound manager's reference block for it and the serial the block had
// when the handle was taken. The sound is gone once the block's serial differs; -1 is no sound. Two
// words, returned in two registers. Each file that looks inside the block has its own view of it.
// Names are placeholders.
#include "types.h"

struct UnkSoundRef;

struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;
};
