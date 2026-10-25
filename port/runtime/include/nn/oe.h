#pragma once

// nn::oe: the application's dealings with the system: performance mode (docked and handheld
// clocks), operation mode, language, the start-up logo, exit requests and the program's active
// time. The boot path calls Initialize and SetPerformanceConfiguration from the graphics
// start-up (src/game_s7/fn_71001587f0.cpp) and GetProgramTotalActiveTime from a timer
// (src/game_s1/fn_7100080c40.cpp, fn_7100080bd0).
//
// `main` imports (data/functions/): Initialize, GetOperationMode, SetPerformanceConfiguration,
// SetCpuBoostMode, GetProgramTotalActiveTime, FinishStartupLogo, GetDesiredLanguage,
// SetOperationModeChangedNotificationEnabled, SetPerformanceModeChangedNotificationEnabled,
// SetUserInactivityDetectionTimeExtended, EnterExitRequestHandlingSection,
// LeaveExitRequestHandlingSection, TryPopNotificationMessage.

#include "nn/nn_common.h"

namespace nn::oe {

// The console's clock profiles, as a number. The game never names one: it passes
// PerformanceMode(1) with configuration 0x10001 at graphics start-up (fn_71001587f0), so the
// type carries no enumerators. The PC has one profile and the port ignores both arguments.
enum PerformanceMode : s32 {};
using PerformanceConfiguration = s32;

void Initialize();
void SetPerformanceConfiguration(PerformanceMode mode, PerformanceConfiguration configuration);
// How long the program has run, not counting time spent suspended.
TimeSpan GetProgramTotalActiveTime();

}  // namespace nn::oe
