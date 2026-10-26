#pragma once

// nn::oe: the application's dealings with the system: performance mode (docked and handheld
// clocks), operation mode, language, the start-up logo, exit requests and the program's active
// time. The boot path, in order: the system start-up fn_71001325d0 (EnterExitRequestHandlingSection,
// Initialize, the two change notifications, SetPerformanceConfiguration, GetOperationMode), the
// per-frame notification loop fn_71001327c0 (TryPopNotificationMessage, GetOperationMode again on
// a mode change), fn_7100132aa0 (SetUserInactivityDetectionTimeExtended), fn_71001d1fd0
// (SetCpuBoostMode), the shutdown fn_7100132730 (LeaveExitRequestHandlingSection); also the
// graphics start-up (src/game_s7/fn_71001587f0.cpp: Initialize, SetPerformanceConfiguration) and
// a timer (src/game_s1/fn_7100080c40.cpp, fn_7100080bd0: GetProgramTotalActiveTime).
//
// `main` imports (data/functions/): Initialize, GetOperationMode, SetPerformanceConfiguration,
// SetCpuBoostMode, GetProgramTotalActiveTime, FinishStartupLogo, GetDesiredLanguage,
// SetOperationModeChangedNotificationEnabled, SetPerformanceModeChangedNotificationEnabled,
// SetUserInactivityDetectionTimeExtended, EnterExitRequestHandlingSection,
// LeaveExitRequestHandlingSection, TryPopNotificationMessage.
// All declared below. Outside the boot slice: GetDesiredLanguage (fn_710119c41c matches it against
// the game's 14 language codes) and FinishStartupLogo (fn_7100adfb20).

#include "nn/nn_common.h"

namespace nn::settings {

// A language as a short string ("en-US", "ja"): eight bytes, returned in x0, compared as one
// word by fn_710119c41c.
struct LanguageCode {
    char string[8];
};
static_assert(sizeof(LanguageCode) == 8, "one register");

}  // namespace nn::settings

namespace nn::oe {

// The console's clock profiles, as a number. The game never names one: it passes
// PerformanceMode(1) with configuration 0x10001 at graphics start-up (fn_71001587f0) and mode 0
// with 0x20003 at system start-up (fn_71001325d0), so the type carries no enumerators. The PC has
// one profile and the port ignores both arguments.
enum PerformanceMode : s32 {};
using PerformanceConfiguration = s32;

// Handheld or docked. fn_71001325d0 stores the value in a global and fn_71001327c0 refreshes it
// on notification 30; the port reports docked, the PC's natural profile.
enum OperationMode : s32 {
    OperationMode_Handheld = 0,
    OperationMode_Console = 1,
};

// A notification the system queues for the application. fn_71001327c0 handles two: 4 (exit
// requested: it sets the quit flag) and 30 (operation mode changed).
using Message = u32;
constexpr Message MessageExitRequest = 4;
constexpr Message MessageOperationModeChanged = 30;

// The CPU boost the game asks for while it loads; no enumerators for the same reason as
// PerformanceMode.
enum CpuBoostMode : s32 {};

void Initialize();
void SetPerformanceConfiguration(PerformanceMode mode, PerformanceConfiguration configuration);
OperationMode GetOperationMode();
void SetOperationModeChangedNotificationEnabled(bool enabled);
void SetPerformanceModeChangedNotificationEnabled(bool enabled);
bool TryPopNotificationMessage(Message* out);
void EnterExitRequestHandlingSection();
void LeaveExitRequestHandlingSection();
void SetUserInactivityDetectionTimeExtended(bool extended);
void SetCpuBoostMode(CpuBoostMode mode);
// How long the program has run, not counting time spent suspended.
TimeSpan GetProgramTotalActiveTime();
// The system language: $DREAD_LANGUAGE (a code the game knows, such as "fr" or "ja"), else en-US.
settings::LanguageCode GetDesiredLanguage();
void FinishStartupLogo();

}  // namespace nn::oe
