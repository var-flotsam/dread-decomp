#pragma once

// nv: the graphics driver's bootstrap. The system start-up fn_71001325d0 gives it an allocator
// (SetGraphicsAllocator: the game's StandardAllocator through the callbacks fn_71001322b0,
// fn_71001322d0, fn_71001322e0) and 8 MB of memory (InitializeGraphics); the shutdown
// fn_7100132730 calls FinalizeGraphics. After that the game reaches the driver through NVN
// (nvnBootstrapLoader), which is the NVN shim's (port/nvn/, M2). The port keeps what it is given
// for the shim.
//
// `main` imports (data/functions/): SetGraphicsAllocator, InitializeGraphics, FinalizeGraphics.

#include "types.h"

namespace nv {

using AllocateFunction = void* (*)(u64 size, u64 alignment, void* user);
using FreeFunction = void (*)(void* pointer, void* user);
using ReallocateFunction = void* (*)(void* pointer, u64 size, void* user);

void SetGraphicsAllocator(AllocateFunction allocate, FreeFunction free, ReallocateFunction reallocate, void* user);
void InitializeGraphics(void* memory, u64 size);
void FinalizeGraphics();

// Not imports: what the NVN shim reads back.
struct GraphicsSetup {
    AllocateFunction allocate;
    FreeFunction free;
    ReallocateFunction reallocate;
    void* user;
    void* memory;
    u64 memory_size;
    bool initialized;
};
const GraphicsSetup& GetGraphicsSetupForPort();

}  // namespace nv
