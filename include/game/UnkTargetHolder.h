#pragma once

#include "base/UnkRefHandle.h"
#include "types.h"

struct UnkTarget;

// The weak handle the holder refers to (base::UnkRefCounted's layout); its target is at +0x10.
struct UnkTargetHolder : base::UnkRefCounted {
    UnkTarget* field_0x10;
};
