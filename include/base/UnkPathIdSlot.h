#pragma once

#include "base/snd/UnkSoundSystemATK.h"
#include "types.h"

// Storage for a file-path id whose release is written out by hand.
union UnkPathIdSlot {
    base::global::CFilePathStrId id;
    UnkPathIdSlot() {}
    ~UnkPathIdSlot() {}
};
