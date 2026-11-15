// Unit 36 of tools/tu_map.py (the NFP worker): mounts the tag's ROM area, trying up to five times while the NFP
// library answers that it is busy and waiting 200 ms between tries. Returns the last result. Names and layouts
// are placeholders.
#include <nn/nfp.h>
#include <nn/os.h>

#include "types.h"

struct UnkUnit36Tag {
    const nn::nfp::DeviceHandle* handle;
};

static bool IsBusy(nn::Result result) {
    return (static_cast<u64>(result.GetInnerValueForDebug()) & 0x3ff1ff) == 0xb073;
}

u32 fn_710002c530(UnkUnit36Tag* self) {
    nn::Result result;
    for (s32 tries = 0; tries < 5; tries++) {
        result = nn::nfp::Mount(*self->handle, nn::nfp::ModelType_Amiibo, nn::nfp::MountTarget_Rom);
        if (!IsBusy(result))
            break;
        nn::os::SleepThread(nn::TimeSpan::FromNanoSeconds(200000000));
    }
    return result.GetInnerValueForDebug();
}
