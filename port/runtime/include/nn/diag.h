#pragma once

// nn::diag: assertions and aborts. The game's assert path ends in nn::diag::detail::AbortImpl
// (2 imports in data/functions/); on PC that becomes a message on stderr and a trap. Its
// argument list is not known from the game's uses yet, so nothing is declared: the port's abort
// helper goes here when the first lifted or matched caller shows what it passes (file, line,
// condition and message are the likely arguments).

#include "nn/nn_common.h"

namespace nn::diag {}  // namespace nn::diag
