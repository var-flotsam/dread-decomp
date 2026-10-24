#pragma once

// The system tick: a 64-bit counter at a fixed rate. The console's counter runs at 19.2 MHz; the
// port counts the same unit from a monotonic host clock, so tick values the game stores keep
// their meaning. src/game_s1/fn_7100080c40.cpp includes this header alone and calls
// GetSystemTick and ConvertToTimeSpan, so the conversions are declared here, not in nn/os.h.

#include "nn/nn_common.h"

namespace nn::os {

constexpr s64 SystemTickFrequency = 19200000;

struct Tick {
    s64 value;

    constexpr explicit Tick(s64 v = 0) : value(v) {}
    constexpr s64 GetInt64Value() const { return value; }
};
static_assert(sizeof(Tick) == 8, "a tick is one 64-bit word in the game's objects");

Tick GetSystemTick();
TimeSpan ConvertToTimeSpan(Tick tick);
Tick ConvertToTick(TimeSpan span);

}  // namespace nn::os
