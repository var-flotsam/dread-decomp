#pragma once

// The movement component (tools/tu_map.py: the CMovementComponent unit) as its derived components'
// updates call it. Names are placeholders.

#include "types.h"

struct UnkMovementComponent;

// The base per-frame update, with the step (s0), which the derived updates pass on.
void fn_71010d1a90(UnkMovementComponent* self, f32 dt);
