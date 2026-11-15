// Unit 36 of tools/tu_map.py (the NFP worker): creates the tag's application area from the request's words,
// trying up to five times while the NFP library answers that it is busy and waiting 200 ms between tries.
// Returns the last result. Names and layouts are placeholders.
#include <nn/nfp.h>
#include <nn/os.h>

#include "types.h"

struct UnkUnit36AreaRequest {
    const nn::nfp::DeviceHandle* handle;  // +0x0
    u32 field_0x8;
    u8 pad_0xc[4];
    u64 field_0x10;
    u32 field_0x18;
};

// The block CreateApplicationArea takes.
struct UnkUnit36AreaInfo {
    u32 id;
    u32 size;
    u64 data;
    u64 field_0x10;  // unset in the original
};

namespace nn::nfp {
u64 CreateApplicationArea(const DeviceHandle& handle, const UnkUnit36AreaInfo& info);
}

static bool IsBusy(u64 result) {
    return (result & 0x3ff1ff) == 0xb073;
}

u32 fn_710002c660(UnkUnit36AreaRequest* self) {
    u64 result;
    s32 tries = 0;
    do {
        UnkUnit36AreaInfo info;
        info.id = self->field_0x8;
        info.data = self->field_0x10;
        info.size = self->field_0x18;
        result = nn::nfp::CreateApplicationArea(*self->handle, info);
        if (!IsBusy(result))
            break;
        nn::os::SleepThread(nn::TimeSpan::FromNanoSeconds(200000000));
    } while (++tries < 5);
    return static_cast<u32>(result);
}
