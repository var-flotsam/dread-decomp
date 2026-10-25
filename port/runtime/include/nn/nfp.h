#pragma once

// nn::nfp: amiibo. The game initialises the library at boot (Initialize, ListDevices, the
// Attach*Event calls) and polls tag state from a menu. On PC there is no reader: the port reports
// no devices and every tag call fails, which the game already handles as "no amiibo".
//
// `main` imports (data/functions/): Initialize, Finalize, ListDevices, GetState, GetDeviceState,
// GetNpadId, StartDetection, StopDetection, AttachActivateEvent, AttachDeactivateEvent,
// AttachAvailabilityChangeEvent, Mount, Unmount, Flush, Restore, GetTagInfo, GetCommonInfo,
// GetModelInfo, GetRegisterInfo, OpenApplicationArea, CreateApplicationArea, GetApplicationArea,
// SetApplicationArea, StartGameDataEraser, StartNicknameAndOwnerSettings, StartRestorer.
// Nothing is declared until the first matched or lifted caller needs it (stubs, M1).

#include "nn/nn_common.h"

namespace nn::nfp {}  // namespace nn::nfp
