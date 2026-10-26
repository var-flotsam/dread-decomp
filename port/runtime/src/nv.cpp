// nv for the port: what the system start-up gives the graphics driver is kept for the NVN shim
// (port/nvn/, M2), which allocates its own objects through the same callbacks if it wants the
// game's accounting. Every stub names the boot-path function that depends on it in a TODO.

#include "nv.h"

namespace nv {

namespace {

GraphicsSetup g_setup{};

}  // namespace

void SetGraphicsAllocator(AllocateFunction allocate, FreeFunction free, ReallocateFunction reallocate, void* user) {
    // TODO(M1): fn_71001325d0 passes fn_71001322b0, fn_71001322d0, fn_71001322e0 and its
    // StandardAllocator.
    g_setup.allocate = allocate;
    g_setup.free = free;
    g_setup.reallocate = reallocate;
    g_setup.user = user;
}

void InitializeGraphics(void* memory, u64 size) {
    // TODO(M2): fn_71001325d0 gives the driver 8 MB of its own; the NVN shim decides what to do
    // with it (nothing, so far).
    g_setup.memory = memory;
    g_setup.memory_size = size;
    g_setup.initialized = true;
}

void FinalizeGraphics() {
    // TODO(M2): the shutdown fn_7100132730.
    g_setup.initialized = false;
}

const GraphicsSetup& GetGraphicsSetupForPort() {
    return g_setup;
}

}  // namespace nv
