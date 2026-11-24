#pragma once

// One object that 3 files viewed in part (fn_7100d7c614 returns it, and all three call or define that function; all views are 16 bytes). Names and layouts are placeholders.
#include "types.h"

struct UnkHyperBeamEffectRef;

struct UnkHyperBeamEffectHandle {
    UnkHyperBeamEffectRef* ref;
    s64 serial;

    void* Get() const;
};
