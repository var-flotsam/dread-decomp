// nn::oe for the port: no clock profiles and no system notifications but the exit request the
// port's own window raises; the operation mode is docked; the program's active time is wall time
// since the runtime first read the clock.

#include "nn/oe.h"

#include "dread_port.h"
#include "nn/os.h"

#include <atomic>
#include <cstdlib>
#include <cstring>

namespace nn::oe {

namespace {

std::atomic<bool> g_exit_requested{false};

}  // namespace

void Initialize() {
    // TODO(M1): the system start-up (fn_71001325d0) and the graphics start-up (fn_71001587f0)
    // call this. Nothing to set up on PC; reading the tick fixes its origin.
    (void)os::GetSystemTick();
}

void SetPerformanceConfiguration(PerformanceMode, PerformanceConfiguration) {
    // TODO(M1): fn_71001325d0 (mode 0, 0x20003) and fn_71001587f0 (mode 1, 0x10001). The PC has
    // no clock profiles, so there is nothing to do.
}

OperationMode GetOperationMode() {
    // TODO(M1): fn_71001325d0 stores the mode at start-up, fn_71001327c0 on every mode change.
    return OperationMode_Console;
}

void SetOperationModeChangedNotificationEnabled(bool) {
    // TODO(M1): fn_71001325d0 turns it on. The port's mode never changes, so nothing is sent.
}

void SetPerformanceModeChangedNotificationEnabled(bool) {
    // TODO(M1): fn_71001325d0 turns it on; as the operation mode.
}

bool TryPopNotificationMessage(Message* out) {
    // TODO(M1): the notification loop fn_71001327c0 polls this every frame and quits on 4. The
    // only message the port sends is that one, once, after dread_port_request_exit.
    if (!g_exit_requested.exchange(false))
        return false;
    *out = MessageExitRequest;
    return true;
}

void EnterExitRequestHandlingSection() {
    // TODO(M1): fn_71001325d0. On the console this asks for the exit request as a notification
    // instead of being terminated; on PC that is the only way the port asks the game to exit.
}

void LeaveExitRequestHandlingSection() {
    // TODO(M1): the shutdown fn_7100132730.
}

void SetUserInactivityDetectionTimeExtended(bool) {
    // TODO(M1): fn_7100132aa0 (the screen dimming delay during cutscenes). Nothing on PC.
}

void SetCpuBoostMode(CpuBoostMode) {
    // TODO(M1): fn_71001d1fd0 boosts the CPU while it loads. Nothing on PC.
}

settings::LanguageCode GetDesiredLanguage() {
    // fn_710119c41c looks the code up in the game's table and falls back when it is not there.
    settings::LanguageCode code{};
    const char* language = std::getenv("DREAD_LANGUAGE");
    if (!language || !*language || std::strlen(language) >= sizeof(code.string))
        language = "en-US";
    std::memcpy(code.string, language, std::strlen(language));
    return code;
}

void FinishStartupLogo() {
    // fn_7100adfb20 ends the console's boot logo. The port shows none.
}

TimeSpan GetProgramTotalActiveTime() {
    // TODO(M1): fn_7100080bd0 reports the program's active time in milliseconds. The console
    // leaves out time spent suspended; the port counts wall time since the tick's origin.
    return os::ConvertToTimeSpan(os::GetSystemTick());
}

}  // namespace nn::oe

extern "C" void dread_port_request_exit() {
    nn::oe::g_exit_requested.store(true);
}
