#pragma once

#include "types.h"

struct UnkHyperBeamSoundRef;

struct UnkHyperBeamSoundHandle {
    UnkHyperBeamSoundRef* ref;
    s64 serial;
};
