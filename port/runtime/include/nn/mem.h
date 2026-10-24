#pragma once

// nn::mem: the SDK's general-purpose heap (StandardAllocator: the constructor, Initialize,
// Finalize, Allocate, Free, Reallocate; 6 imports in data/functions/). The game carves its
// 1.82 GB .bss with its own dlmalloc mspaces (include/base/UnkDefaultAllocator.h), so this heap
// serves the SDK libraries themselves (nn::fs's allocator, nn::atk's work memory). On PC: the
// host's malloc. Nothing is declared until the first caller needs it (M1).

#include "nn/nn_common.h"

namespace nn::mem {}  // namespace nn::mem
