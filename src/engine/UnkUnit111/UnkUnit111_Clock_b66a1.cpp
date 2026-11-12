// tools/tu_map.py unit 111 (no class hint): samples the engine's clock and stores the seconds since
// a start stamp (0 before it). Names are placeholders.
#include "game/UnkTU0.h"
#include "game/UnkTU58.h"
#include "types.h"

struct UnkGfxFrameSource;
struct UnkTimer_7100080c40;

extern s64 g_unk_710b371090;  // start stamp, milliseconds
extern s64 g_unk_710b371098;  // last sample, milliseconds
extern f32 g_unk_710b3710a0;  // seconds since the start stamp

void fn_7100132c60() {
    s64 now = fn_7100080fb4(reinterpret_cast<const UnkTimer_7100080c40*>(g_unk_7101d47328));
    g_unk_710b371098 = now;
    s64 start = g_unk_710b371090;
    f32 seconds;
    if (now > start) {
        seconds = static_cast<f32>(now - start) * 0.001f;
    } else {
        seconds = 0.0f;
    }
    g_unk_710b3710a0 = seconds;
}
