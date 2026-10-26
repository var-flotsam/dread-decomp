#pragma once

// A read-only three-float vector in .rodata that several units copy as their default (all zero in
// the uses so far: the attack tunables' offsets, the rumble manager's defaults, a facing test's
// reference). The name is a placeholder.

#include "base/math/CVector.h"

extern const base::math::CVector3D g_unk_710164779c;
