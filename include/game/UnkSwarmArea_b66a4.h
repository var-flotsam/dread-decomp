#pragma once

#include "base/math/CVector.h"

struct UnkSwarmArea_b66a4 {
    void** vtable;
    bool Contains(const base::math::CVector2D* p) {
        return reinterpret_cast<bool (*)(UnkSwarmArea_b66a4*, const base::math::CVector2D*)>(vtable[0x50 / 8])(this, p);
    }
};
