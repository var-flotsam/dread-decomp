// Unit 36 of tools/tu_map.py (the NFP worker): restores the tag's data, trying up to five times while the NFP
// library answers that it is busy and waiting 200 ms between tries. Returns the last result. Names and layouts
// are placeholders.
#include <nn/os.h>

#include "types.h"

namespace nn::nfp {
u64 Restore(void* handle);
}

struct UnkUnit36Tag {
    void* handle;
};

static bool IsBusy(u64 result) {
    return (result & 0x3ff1ff) == 0xb073;
}

u32 fn_710002c960(UnkUnit36Tag* self) {
    for (s32 tries = 0; tries < 4; tries++) {
        u64 result = nn::nfp::Restore(self->handle);
        if (!IsBusy(result))
            return static_cast<u32>(result);
        nn::os::SleepThread(nn::TimeSpan::FromNanoSeconds(200000000));
    }
    u64 result = nn::nfp::Restore(self->handle);
    if (IsBusy(result))
        nn::os::SleepThread(nn::TimeSpan::FromNanoSeconds(200000000));
    return static_cast<u32>(result);
}
