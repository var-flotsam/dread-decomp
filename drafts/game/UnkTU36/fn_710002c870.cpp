// Unit 36 of tools/tu_map.py (the NFP worker): flushes the tag's data, trying up to five times while the NFP
// library answers that it is busy and waiting 200 ms between tries. Returns the last result. Names and layouts
// are placeholders.
#include <nn/os.h>

#include "types.h"

namespace nn::nfp {
u64 Flush(void* handle);
}

struct UnkUnit36Tag {
    void* handle;
};

u32 fn_710002c870(UnkUnit36Tag* self) {
    u64 result;
    for (s32 tries = 0; tries < 5; tries++) {
        result = nn::nfp::Flush(self->handle);
        if ((result & 0x3ff1ff) != 0xb073)
            break;
        nn::os::SleepThread(nn::TimeSpan::FromNanoSeconds(200000000));
    }
    return static_cast<u32>(result);
}
