// nn::oe for the port: no clock profiles and no system notifications; the program's active time
// is wall time since the runtime first read the clock.

#include "nn/oe.h"

#include "nn/os.h"

namespace nn::oe {

void Initialize() {
    // TODO(M2): the graphics start-up (fn_71001587f0) calls this just before
    // SetPerformanceConfiguration. Nothing to set up on PC; reading the tick fixes its origin.
    (void)os::GetSystemTick();
}

void SetPerformanceConfiguration(PerformanceMode, PerformanceConfiguration) {
    // TODO(M2): fn_71001587f0 asks for boost mode with configuration 0x10001. The PC has no
    // clock profiles, so there is nothing to do.
}

TimeSpan GetProgramTotalActiveTime() {
    // TODO(M1): fn_7100080bd0 reports the program's active time in milliseconds. The console
    // leaves out time spent suspended; the port counts wall time since the tick's origin.
    return os::ConvertToTimeSpan(os::GetSystemTick());
}

}  // namespace nn::oe
