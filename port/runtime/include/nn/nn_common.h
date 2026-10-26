#pragma once

// Types shared by the port's nn:: declarations: the result code and the time span. Everything
// under port/runtime/include/nn/ is written for the port from the game's own uses (src/) and
// the imported symbol names recorded in data/functions/; nothing comes from lib/nnsdk.

#include "types.h"

namespace nn {

// A status code as the SDK's functions return it: zero is success. The port raises its own
// failures with a value no SDK module uses, so a caller that prints one can tell them apart.
struct Result {
    u32 value;

    constexpr bool IsSuccess() const { return value == 0; }
    constexpr bool IsFailure() const { return value != 0; }
};

constexpr Result ResultSuccess() { return Result{0}; }
// The service exists on the console and is not implemented by the port yet.
constexpr Result ResultNotPorted() { return Result{0xffffffffu}; }

// A signed duration in nanoseconds. The game reads the member directly
// (src/game_s1/fn_7100080c40.cpp), so it is a plain field, not an accessor. The one conversion
// helper src/ names is FromNanoSeconds (src/base/snd/CSoundSystemATK.cpp); the struct stays an
// aggregate.
struct TimeSpan {
    s64 nanoseconds;

    static constexpr TimeSpan FromNanoSeconds(s64 ns) { return TimeSpan{ns}; }
};
static_assert(sizeof(TimeSpan) == 8, "a time span is one 64-bit word in the game's objects");

}  // namespace nn
