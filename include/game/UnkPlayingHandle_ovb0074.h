#pragma once

#include "base/UnkPlayingRef_ovb0074.h"
#include "types.h"

struct UnkPlayingSound_ovb0074;

struct UnkPlayingHandle_ovb0074 {
    UnkPlayingRef_ovb0074* ref;
    u64 serial;

    bool IsValid() const { return ref && serial != ~0ull && ref->serial == serial; }
    UnkPlayingSound_ovb0074* Get() const { return IsValid() ? ref->sound : nullptr; }
};
