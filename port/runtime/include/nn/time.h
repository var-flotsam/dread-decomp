#pragma once

// nn::time: the calendar clock, used for save-file time stamps and the play report. The system
// start-up fn_71001325d0 calls Initialize, the shutdown fn_7100132730 Finalize; fn_710006bc44
// allocates a 0xd0-byte ClockSnapshot, constructs it and fills it from the system clock, and
// fn_710006bfac, fn_710006bfc4 and fn_710006bfe0 read its calendar time (fn_710006bfc4 shifts the
// result right by 16 for the month and day). On PC: the host's local time.
//
// `main` imports (data/functions/): Initialize, Finalize, ClockSnapshot::ClockSnapshot,
// ClockSnapshot::CreateWithStandardSystemClock, ClockSnapshot::GetStandardUserSystemClockCalendarTime.
// All declared below.

#include "nn/nn_common.h"

namespace nn::time {

// A date and time in one word, as the readers take it from x0: the year in bits 0-15, then month,
// day, hour, minute and second a byte each.
struct CalendarTime {
    s16 year;
    s8 month;
    s8 day;
    s8 hour;
    s8 minute;
    s8 second;
};
static_assert(sizeof(CalendarTime) == 8, "one register");

void Initialize();
void Finalize();

// The game allocates 0xd0 bytes for one (fn_710006bc44); the port keeps the host time in its
// first word.
class ClockSnapshot {
public:
    ClockSnapshot();
    static void CreateWithStandardSystemClock(ClockSnapshot* out);
    CalendarTime GetStandardUserSystemClockCalendarTime() const;

private:
    u8 m_storage[0xd0];
};
static_assert(sizeof(ClockSnapshot) == 0xd0, "fn_710006bc44 allocates 0xd0 bytes");

}  // namespace nn::time
