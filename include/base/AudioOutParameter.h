#pragma once

#include "types.h"

namespace nn {
namespace audio {

struct AudioOutParameter {
    u32 sample_rate;
    u16 channel_count;
    u16 reserved;
};

}  // namespace audio
}  // namespace nn
