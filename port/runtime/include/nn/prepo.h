#pragma once

// nn::prepo: play reports (telemetry sent to the console's servers). The game builds one with
// PlayReport::SetBuffer and Add and submits it with Save. On PC the port accepts and drops them;
// nothing is sent anywhere.
//
// `main` imports (data/functions/): PlayReport::PlayReport, PlayReport::SetBuffer,
// PlayReport::Add, PlayReport::Save.
// Nothing is declared until the first matched or lifted caller needs it (stubs, M1).

#include "nn/nn_common.h"

namespace nn::prepo {}  // namespace nn::prepo
