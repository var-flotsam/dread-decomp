// From unit 259 of tools/tu_map.py (0x71001c6960-0x71001d948c, the graphics system's unit): turns
// the CPU boost on or off when the graphics system is up (fn_71001598c0) and the request changes it:
// the flag is kept under a mutex, a light event (fn_7100066698) is signalled when turning it on,
// then the SDK is told. The flag and the mutex are unit statics laid out together (0x14c apart);
// the event is reached on its own. Names are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "types.h"

#define HIDDEN __attribute__((visibility("hidden")))

namespace nn::oe {
enum CpuBoostMode {
    CpuBoostMode_Disabled = 0,
    CpuBoostMode_Boost = 1,
};
void SetCpuBoostMode(CpuBoostMode mode);
}  // namespace nn::oe

bool fn_71001598c0();
void fn_7100066698(nn::os::LightEventType* event);

// The flag; the mutex is 0x14c bytes after it (8-byte aligned there, so not a member of one struct).
extern bool g_unk_716e595b6c[] HIDDEN;
extern u8 g_unk_716e595ca8[] HIDDEN;

static inline nn::os::MutexType* BoostMutex() {
    return reinterpret_cast<nn::os::MutexType*>(reinterpret_cast<char*>(g_unk_716e595b6c) + 0x14c);
}

void fn_71001d1fd0(void* self, bool boost) {
    if (!fn_71001598c0() || g_unk_716e595b6c[0] == boost)
        return;
    nn::os::LockMutex(BoostMutex());
    g_unk_716e595b6c[0] = boost;
    fn_7100066920(BoostMutex());
    if (boost)
        fn_7100066698(reinterpret_cast<nn::os::LightEventType*>(g_unk_716e595ca8));
    nn::oe::SetCpuBoostMode(static_cast<nn::oe::CpuBoostMode>(boost));
}
