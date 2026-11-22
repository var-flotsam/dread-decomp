#pragma once

#include "base/reflection/UnkReflCast.h"

struct UnkWaterEntry_b57 {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkWaterEntry_b57*)>(vtable[0])(this);
    }
};
