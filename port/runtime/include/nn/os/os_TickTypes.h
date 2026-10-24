#pragma once

// The system tick: a 64-bit counter at a fixed rate. src/game_s1/fn_7100080c40.cpp includes this
// header alone and calls GetSystemTick and ConvertToTimeSpan, so the conversions are declared
// here, not in nn/os.h.

#include "nn/nn_common.h"

namespace nn::os {

// The rate the port's counter runs at. It is internal to the runtime: no game code depends on
// the value (nothing in src/ or include/ names it, and the only use of the tick is
// ConvertToTimeSpan(GetSystemTick()).nanoseconds, in fn_7100080c40 and fn_7100080f2c), so it is
// not a layout or behaviour constraint. 19.2 MHz is the console's rate, kept so that tick values
// have the same scale as there; any rate the two conversions agree on would do.
constexpr s64 SystemTickFrequency = 19200000;

// One tick count, as the import returns it in a register. No source in src/ stores or reads a
// Tick (both callers convert it to a time span at once), so it is a plain word with a constructor.
struct Tick {
    s64 value;

    constexpr explicit Tick(s64 v = 0) : value(v) {}
};
static_assert(sizeof(Tick) == 8, "a tick is one 64-bit word");

Tick GetSystemTick();
TimeSpan ConvertToTimeSpan(Tick tick);
// Imported by main (data/functions/) but called by no source yet; the stubs use it themselves.
Tick ConvertToTick(TimeSpan span);

}  // namespace nn::os
