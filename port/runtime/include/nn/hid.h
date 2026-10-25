#pragma once

// nn::hid: controllers and vibration. The boot path initialises the Npad layer (InitializeNpad,
// SetSupportedNpadStyleSet, SetSupportedNpadIdType, SetNpadJoyHoldType) and the title screen
// polls GetNpadStyleSet and GetNpadState every frame; vibration (the VibrationPlayer, nodes
// and mixer that src/game_s1/fn_71000707dc.cpp wraps) is gameplay. On PC: SDL game controllers
// and the keyboard.
//
// `main` imports (data/functions/): InitializeNpad, SetSupportedNpadStyleSet,
// SetSupportedNpadIdType, SetNpadJoyHoldType, GetNpadStyleSet, GetNpadState,
// GetVibrationDeviceHandles, GetVibrationDeviceInfo, InitializeVibrationDevice,
// ShowControllerSupport, ControllerSupportArg::SetDefault, VibrationMixer (ctor, dtor),
// VibrationNode::Update, VibrationNodeConnection (ctor, dtor, Connect, SetModulation),
// VibrationPlayer (ctor, dtor, Load, Unload, Play, Stop, IsPlaying, IsLoop, SetLoop,
// GetCurrentPosition, GetFileInfo, OnNextSampleRequired), VibrationTarget (ctor, dtor,
// SetVibrationDeviceHandle).
// Nothing is declared until the first matched or lifted caller needs it (M2 for input).

#include "nn/nn_common.h"

namespace nn::hid {}  // namespace nn::hid
