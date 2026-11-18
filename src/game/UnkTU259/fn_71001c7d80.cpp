// Unit 259: the loading screen thread. While the graphics system runs it waits for the loading
// screen flag, then draws a frame under the device mutex every 30 ms until the flag clears or the
// draw callback reports it is done. Names and layouts are placeholders.
#include "base/global/timeline/CEvent.h"
#include "engine/UnkGraphicsSystem.h"
#include "game/UnkTU259.h"
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU177.h"
#include "types.h"

#define HIDDEN __attribute__((visibility("hidden")))

namespace nn::oe {
void LeaveExitRequestHandlingSection();
}  // namespace nn::oe

void fn_710006669c(nn::os::LightEventType* event);
void fn_71001b3df0();
void fn_71001b3e00();
void fn_7100132890();
bool fn_7100132b10();
void fn_7100133030();

struct UnkGfxResolutionScale {
    f32 x;
    f32 y;
};
// u8, not bool as src/engine/UnkGraphicsSystem.cpp declares it: the original compares the byte with 1.

// The loading screen flag, with its light event 0x13c and its mutex 0x14c bytes after it (unit
// statics laid out together).
extern HIDDEN nn::os::MutexType g_unk_716e595cb8;  // the same mutex, named on its own

static inline nn::os::LightEventType* LoadingEvent() {
    return reinterpret_cast<nn::os::LightEventType*>(reinterpret_cast<char*>(g_unk_716e595b6c) + 0x13c);
}
static inline nn::os::MutexType* LoadingMutex() {
    return reinterpret_cast<nn::os::MutexType*>(reinterpret_cast<char*>(g_unk_716e595b6c) + 0x14c);
}

void fn_71001c7d80(void* arg) {
    if (!fn_71001598c0())
        return;
    while (g_unk_716e595b68 == 1) {
        fn_710006669c(LoadingEvent());
        bool first = true;
        while (g_unk_716e595b6c[0]) {
            fn_71001b3df0();
            nn::os::LockMutex(LoadingMutex());
            if (g_unk_716e595b6c[0]) {
                if (first)
                    g_unk_7101a1e60c = true;
                fn_7100159920();
                if (void (*before)() = reinterpret_cast<void (*)()>(g_unk_716e554928))
                    before();
                fn_7100132890();
                if (fn_7100132b10())
                    nn::oe::LeaveExitRequestHandlingSection();
                fn_7100159ac0();
                f32 display_width = (f32)fn_7100132f80();
                s32 width = (s32)(g_unk_7101a1e75c.x * display_width);
                f32 display_height = (f32)fn_7100132f90();
                s32 height = (s32)(g_unk_7101a1e75c.x * display_height);
                fn_710015bd90(0, 0, width, height);
                fn_7100159bf0(0, 1);
                fn_7100133030();
                bool (*after)() = reinterpret_cast<bool (*)()>(g_unk_716e554930);
                if (after && after()) {
                    fn_7100066920(&g_unk_716e595cb8);
                    fn_71001b3e00();
                    fn_7100066854(arg, 30);
                    return;
                }
                first = false;
            }
            fn_7100066920(LoadingMutex());
            fn_71001b3e00();
            fn_7100066854(arg, 30);
        }
    }
}
