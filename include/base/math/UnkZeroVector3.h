#pragma once

// Two read-only three-float vectors in .rodata, side by side and both zero, that several units copy
// as their default: g_unk_710164779c (the attack tunables' offsets, a facing test's reference) and
// g_unk_7101647790 (the rumble players' offsets, a movement's velocity reset, the transform defaults'
// angles). The names are placeholders.

#include "base/math/CVector.h"

extern const base::math::CVector3D g_unk_7101647790;
extern const base::math::CVector3D g_unk_710164779c;
