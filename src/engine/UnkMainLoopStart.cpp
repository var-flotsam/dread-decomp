// From unit 111 of tools/tu_map.py (0x7100132c10-0x71001395b8): what nnMain (src/engine/nnMain.cpp)
// calls with the game's main function: the system start-up (fn_71001325d0,
// src/engine/SystemStartup.cpp), then it records the main thread, creates the 0x48-byte slots object
// of the render pass list's layout (game/UnkRenderPassSlots.h, allocation flags 3) and calls `main`
// when there is one. Names are placeholders.
#include <new>

#include <nn/os.h>

#include "game/UnkRenderPassSlots.h"
#include "types.h"

void fn_71001325d0();

extern nn::os::ThreadType* g_unk_7101a1be20;  // the main thread
extern UnkRenderPassSlots* g_unk_710b3710a8;

void fn_7100132d30(void (*main)()) {
    fn_71001325d0();
    g_unk_7101a1be20 = nn::os::GetCurrentThread();
    g_unk_710b3710a8 = new (fn_71000839a0(0x48, 3)) UnkRenderPassSlots();
    if (main)
        main();
}
