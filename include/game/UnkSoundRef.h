#pragma once

#include "base/snd/UnkSound.h"
#include "types.h"

struct UnkSoundRef {
    UnkSound* object;
    s64 serial;
};
