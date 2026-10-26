// nn::time for the port: the host's local time. Every stub names the boot-path function that
// depends on it in a TODO.

#include "nn/time.h"

#include <cstring>
#include <ctime>

namespace nn::time {

void Initialize() {
    // TODO(M1): the system start-up fn_71001325d0.
}

void Finalize() {
    // TODO(M1): the shutdown fn_7100132730.
}

ClockSnapshot::ClockSnapshot() : m_storage{} {
    // No caller in the boot slice: fn_710006bc44 constructs one in 0xd0 bytes of its own.
}

void ClockSnapshot::CreateWithStandardSystemClock(ClockSnapshot* out) {
    // fn_710006bc44, right after the constructor.
    const std::time_t now = std::time(nullptr);
    std::memcpy(out->m_storage, &now, sizeof(now));
}

CalendarTime ClockSnapshot::GetStandardUserSystemClockCalendarTime() const {
    // fn_710006bfac, fn_710006bfc4 (month and day), fn_710006bfe0: save time stamps.
    std::time_t t;
    std::memcpy(&t, m_storage, sizeof(t));
    std::tm local{};
#if defined(_WIN32)
    localtime_s(&local, &t);
#else
    localtime_r(&t, &local);
#endif
    return CalendarTime{s16(local.tm_year + 1900), s8(local.tm_mon + 1), s8(local.tm_mday), s8(local.tm_hour),
                        s8(local.tm_min), s8(local.tm_sec)};
}

}  // namespace nn::time
