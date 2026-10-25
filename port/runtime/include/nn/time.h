#pragma once

// nn::time: the calendar clock, used for save-file time stamps and the play report. The boot
// path calls Initialize once; the rest is a clock snapshot (ClockSnapshot, CreateWithStandard-
// SystemClock, GetStandardUserSystemClockCalendarTime) when a save is written. On PC: the host's
// local time.
//
// `main` imports (data/functions/): Initialize, Finalize, ClockSnapshot::ClockSnapshot,
// ClockSnapshot::CreateWithStandardSystemClock, ClockSnapshot::GetStandardUserSystemClockCalendarTime.
// Nothing is declared until the first matched or lifted caller needs it (M1 for Initialize).

#include "nn/nn_common.h"

namespace nn::time {}  // namespace nn::time
