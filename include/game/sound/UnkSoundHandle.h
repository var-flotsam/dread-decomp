#pragma once

// A handle on a playing sound: the sound manager's reference block for it and the serial the block had
// when the handle was taken. The sound is gone once the block's serial differs; -1 is no sound. Two
// words, returned in two registers. Each file that looks inside the block has its own view of the block;
// the handle is one object (the same 16 bytes in every view, handed between files by the functions that
// take or return it). Get() is defined where the block is complete. Names are placeholders.
#include "types.h"
#include "base/snd/UnkSound.h"

struct UnkSoundRef;

struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;

    UnkSound* Get() const;
};
