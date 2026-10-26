#pragma once

// nn::mem: the SDK's general-purpose heap. The game has one, in its .bss at 0x710ab70000: a
// static initialiser (fn_7100132b80) constructs it, the system start-up fn_71001325d0 initialises
// it over a 32 MB region of the game's own and hands it to nv as the graphics allocator (the
// callbacks fn_71001322b0, fn_71001322d0, fn_71001322e0 forward to Allocate, Free and
// Reallocate with the allocator as `this`), and the shutdown (fn_7100132730 through
// fn_71001322a0) finalises it. The game carves the rest of its .bss with its own dlmalloc
// mspaces (include/base/UnkDefaultAllocator.h).
//
// `main` imports (data/functions/): StandardAllocator::StandardAllocator, Initialize, Finalize,
// Allocate, Free, Reallocate. All declared below.

#include "nn/nn_common.h"

namespace nn::mem {

// The port allocates on the host's heap, not in the region Initialize names: the region's users
// are the graphics driver's (nv::SetGraphicsAllocator), which the NVN shim replaces, so its
// layout does not matter. The object's size is the game's (its .bss slot); the port uses its
// first word only, a tag that says it was initialised.
class StandardAllocator {
public:
    StandardAllocator();
    void Initialize(void* address, u64 size);
    void Finalize();
    void* Allocate(u64 size, u64 alignment);
    void Free(void* pointer);
    void* Reallocate(void* pointer, u64 size);

private:
    u64 m_state;
};

}  // namespace nn::mem
