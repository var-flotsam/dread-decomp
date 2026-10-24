#pragma once

// nn::audio: the low-level audio renderer that the statically linked nn::atk drives (149
// imports, the largest SDK surface after NVN). The boot path opens the renderer
// (OpenAudioRenderer with the parameters and work buffers sized by the Get*BufferSize calls),
// a final mix and the device sink, then renders a frame per tick; voices, sub-mixes, effects
// (delay, reverb, I3DL2 reverb, aux) and the resampler follow as sounds play. Nothing on the
// title screen needs sound, so this is M4 (docs/PORT_PLAN.md: implement over a PC mixer and keep
// nn::atk lifted, or replace nn::atk). The full import list is in data/functions/ (name_source
// symbol, `nn::audio::*`).

#include "nn/nn_common.h"

namespace nn::audio {}  // namespace nn::audio
