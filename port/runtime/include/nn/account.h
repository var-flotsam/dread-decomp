#pragma once

// nn::account: the console user whose save data the game opens. The boot path calls Initialize
// and TryOpenPreselectedUser, reads the user id (GetUserId) to mount the save, then CloseUser.
// On PC: one fixed local user, so the save directory is always the same.
//
// `main` imports (data/functions/): Initialize, TryOpenPreselectedUser, GetUserId, CloseUser.
// Nothing is declared until the first matched or lifted caller needs it (M1).

#include "nn/nn_common.h"

namespace nn::account {}  // namespace nn::account
