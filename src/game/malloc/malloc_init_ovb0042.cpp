// tools/tu_map.py unit 110's static initialiser: the SDK heap (nn::mem::StandardAllocator, whose
// destructor fn_71001322a0 is registered with __cxa_atexit) and the screen's aspect ratio and its
// inverse from the width and height words. Names are placeholders.
#include <new>

#include "base/UnkAtExit.h"
#include "types.h"

namespace nn::mem {
class StandardAllocator {
public:
    StandardAllocator();
};
}  // namespace nn::mem

extern nn::mem::StandardAllocator g_unk_710ab70000 __attribute__((visibility("hidden")));
void fn_71001322a0(void* self);
extern u32 g_unk_7101a1be18;
extern u32 g_unk_7101a1be1c;
extern f32 g_unk_710b371028;
extern f32 g_unk_710b37102c;

void fn_7100132b80() {
    new (&g_unk_710ab70000) nn::mem::StandardAllocator();
    fn_7100000250(fn_71001322a0, &g_unk_710ab70000, g_unk_7101a19000);
    g_unk_710b371028 = static_cast<f32>(g_unk_7101a1be18) / static_cast<f32>(g_unk_7101a1be1c);
    g_unk_710b37102c = 1.0f / g_unk_710b371028;
}
